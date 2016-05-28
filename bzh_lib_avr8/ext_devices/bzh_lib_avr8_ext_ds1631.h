/*
* bzh_lib_avr8_ext_ds1631.h
*
* Created: 2015/10/28 20:57:59
*  Author: 42477
*/


#ifndef BZH_LIB_AVR8_EXT_DS1631_H_
#define BZH_LIB_AVR8_EXT_DS1631_H_

#include "bzh_lib_avr8_twi.h"

#define BZH_LIB_AVR8_EXT_DS1631_ADDRESS_BASE 0x90
#define BZH_LIB_AVR8_EXT_DS1631_ADDRESS_A0 0
#define BZH_LIB_AVR8_EXT_DS1631_ADDRESS_A1 0
#define BZH_LIB_AVR8_EXT_DS1631_ADDRESS_A2 0

#define BZH_LIB_AVR8_EXT_DS1631_CMD_START_CONVERT 0x51
#define BZH_LIB_AVR8_EXT_DS1631_CMD_STOP_CONVERT 0x22
#define BZH_LIB_AVR8_EXT_DS1631_CMD_READ_TEMPERATURE 0xaa
#define BZH_LIB_AVR8_EXT_DS1631_CMD_ACCESS_TH 0xa1
#define BZH_LIB_AVR8_EXT_DS1631_CMD_ACCESS_TL 0xa2
#define BZH_LIB_AVR8_EXT_DS1631_CMD_ACCESS_CONFIG 0xac
#define BZH_LIB_AVR8_EXT_DS1631_CMD_SOFTWARE_POR 0x54

#define BZH_LIB_AVR8_EXT_DS1631_CFG_REG_DONE 7
#define BZH_LIB_AVR8_EXT_DS1631_CFG_REG_THF 6
#define BZH_LIB_AVR8_EXT_DS1631_CFG_REG_TLF 5
#define BZH_LIB_AVR8_EXT_DS1631_CFG_REG_NVB 4
#define BZH_LIB_AVR8_EXT_DS1631_CFG_REG_R1 3
#define BZH_LIB_AVR8_EXT_DS1631_CFG_REG_R0 2
#define BZH_LIB_AVR8_EXT_DS1631_CFG_REG_POL 1
#define BZH_LIB_AVR8_EXT_DS1631_CFG_REG_1SHOT 0

//unsigned int, true for high 8bit, false for low 8bit
unsigned char bzh_lib_avr8_ext_ds1631_uint_to_h_or_l_uchar(unsigned int uint, bool hl)
{
	if (hl)
	{
		return (uint>>8);
	} 
	else
	{
		return ((uint<<8)>>8);
	}
}

//high 8bit, low 8bit
unsigned int bzh_lib_avr8_ext_ds1631_h_and_l_uchar_to_uint(unsigned char high, unsigned char low)
{
	return ((high<<8) | low);
}

//true for write, false for read
unsigned char bzh_lib_avr8_ext_ds1631_addr(bool rw)
{
	unsigned char addr = BZH_LIB_AVR8_EXT_DS1631_ADDRESS_BASE | (BZH_LIB_AVR8_EXT_DS1631_ADDRESS_A2<<3) | (BZH_LIB_AVR8_EXT_DS1631_ADDRESS_A1<<2) | (BZH_LIB_AVR8_EXT_DS1631_ADDRESS_A0<<1);
	if (!rw)
	{
		addr |= 0x01;
	}
	if (rw)
	{
		addr &= ~0x01;
	}
	return addr;
}

void bzh_lib_avr8_ext_ds1631_init()
{
	bzh_lib_avr8_int_twi_init(1,1);
}

bool bzh_lib_avr8_ext_ds1631_reset()
{
	bool result = false;
	
	//start
	bzh_lib_avr8_int_twi_cmd_start();
	bzh_lib_avr8_int_twi_wait_for_int();
	if (bzh_lib_avr8_int_twi_check_start())
	{
		//sla+w
		bzh_lib_avr8_int_twi_data_put(bzh_lib_avr8_ext_ds1631_addr(true));
		bzh_lib_avr8_int_twi_wait_for_int();
		if (bzh_lib_avr8_int_twi_check_gen(TW_MT_SLA_ACK))
		{
			//reset
			bzh_lib_avr8_int_twi_data_put(BZH_LIB_AVR8_EXT_DS1631_CMD_SOFTWARE_POR);
			bzh_lib_avr8_int_twi_wait_for_int();
			if (bzh_lib_avr8_int_twi_check_gen(TW_MT_DATA_NACK))
			{
				result = true;
			}
		}
	}

	//stop
	bzh_lib_avr8_int_twi_cmd_stop();
	return result;
	
}

unsigned char bzh_lib_avr8_ext_ds1631_cfg_reg_get()
{
	unsigned char cfg_reg = 0x00;
	//start
	bzh_lib_avr8_int_twi_cmd_start();
	bzh_lib_avr8_int_twi_wait_for_int();
	if (bzh_lib_avr8_int_twi_check_start())
	{
		//sla+w
		bzh_lib_avr8_int_twi_data_put(bzh_lib_avr8_ext_ds1631_addr(true));
		bzh_lib_avr8_int_twi_wait_for_int();
		if (bzh_lib_avr8_int_twi_check_gen(TW_MT_SLA_ACK))
		{
			//cmd
			bzh_lib_avr8_int_twi_data_put(BZH_LIB_AVR8_EXT_DS1631_CMD_ACCESS_CONFIG);
			bzh_lib_avr8_int_twi_wait_for_int();
			if (bzh_lib_avr8_int_twi_check_gen(TW_MT_DATA_ACK))
			{
				//restart
				bzh_lib_avr8_int_twi_cmd_restart();
				bzh_lib_avr8_int_twi_wait_for_int();
				if (bzh_lib_avr8_int_twi_check_restart())
				{
					//sla+r
					bzh_lib_avr8_int_twi_data_put(bzh_lib_avr8_ext_ds1631_addr(false));
					bzh_lib_avr8_int_twi_wait_for_int();
					if (bzh_lib_avr8_int_twi_check_gen(TW_MR_SLA_ACK))
					{
						//disable ack
						bzh_lib_avr8_int_twi_cmd_set('a',false);
						
						//read data
						bzh_lib_avr8_int_twi_data_get();
						bzh_lib_avr8_int_twi_wait_for_int();
						if (bzh_lib_avr8_int_twi_check_gen(TW_MR_DATA_NACK))
						{
							cfg_reg = TWDR;
						}
					}
				}
			}
		}
	}
	
	//stop
	bzh_lib_avr8_int_twi_cmd_stop();
	return cfg_reg;
}

bool bzh_lib_avr8_ext_ds1631_cfg_reg_put(unsigned char cfg_reg)
{
		bool result = false;
		//start
		bzh_lib_avr8_int_twi_cmd_start();
		bzh_lib_avr8_int_twi_wait_for_int();
		if (bzh_lib_avr8_int_twi_check_start())
		{
			//sla+w
			bzh_lib_avr8_int_twi_data_put(bzh_lib_avr8_ext_ds1631_addr(true));
			bzh_lib_avr8_int_twi_wait_for_int();
			if (bzh_lib_avr8_int_twi_check_gen(TW_MT_SLA_ACK))
			{
				//cmd
				bzh_lib_avr8_int_twi_data_put(BZH_LIB_AVR8_EXT_DS1631_CMD_ACCESS_CONFIG);
				bzh_lib_avr8_int_twi_wait_for_int();
				if (bzh_lib_avr8_int_twi_check_gen(TW_MT_DATA_ACK))
				{
					//data write
					bzh_lib_avr8_int_twi_data_put(cfg_reg);
					bzh_lib_avr8_int_twi_wait_for_int();
					if (bzh_lib_avr8_int_twi_check_gen(TW_MT_DATA_ACK))
					{
						result = true;
					}
				}
			}
		}
		
		//stop
		bzh_lib_avr8_int_twi_cmd_stop();
		return result;
}

// true for start, false for stop
bool bzh_lib_avr8_ext_ds1631_convert_ctl(bool sp)
{
	bool result = false;
	
	//start
	bzh_lib_avr8_int_twi_cmd_start();
	bzh_lib_avr8_int_twi_wait_for_int();
	if (bzh_lib_avr8_int_twi_check_start())
	{
		//sla+w
		bzh_lib_avr8_int_twi_data_put(bzh_lib_avr8_ext_ds1631_addr(true));
		bzh_lib_avr8_int_twi_wait_for_int();
		if (bzh_lib_avr8_int_twi_check_gen(TW_MT_SLA_ACK))
		{
			//cmd
			if (sp)
			{
				bzh_lib_avr8_int_twi_data_put(BZH_LIB_AVR8_EXT_DS1631_CMD_START_CONVERT);
			}
			else
			{
				bzh_lib_avr8_int_twi_data_put(BZH_LIB_AVR8_EXT_DS1631_CMD_STOP_CONVERT);
			}
			bzh_lib_avr8_int_twi_wait_for_int();
			if (bzh_lib_avr8_int_twi_check_gen(TW_MT_DATA_ACK))
			{
				result = true;
			}
		}
	}
	
	//stop
	bzh_lib_avr8_int_twi_cmd_stop();
	return result;
}

unsigned int bzh_lib_avr8_ext_ds1631_temp_reg_get()
{
	unsigned int temp_reg_h = 0x00;
	unsigned int temp_reg_l = 0x00;
	unsigned int temp_reg =  0x0000;
	
	//start
	bzh_lib_avr8_int_twi_cmd_start();
	bzh_lib_avr8_int_twi_wait_for_int();
	if (bzh_lib_avr8_int_twi_check_start())
	{
		//sla+w
		bzh_lib_avr8_int_twi_data_put(bzh_lib_avr8_ext_ds1631_addr(true));
		bzh_lib_avr8_int_twi_wait_for_int();
		if (bzh_lib_avr8_int_twi_check_gen(TW_MT_SLA_ACK))
		{
			//cmd
			bzh_lib_avr8_int_twi_data_put(BZH_LIB_AVR8_EXT_DS1631_CMD_READ_TEMPERATURE);
			bzh_lib_avr8_int_twi_wait_for_int();
			if (bzh_lib_avr8_int_twi_check_gen(TW_MT_DATA_ACK))
			{
				//restart
				bzh_lib_avr8_int_twi_cmd_restart();
				bzh_lib_avr8_int_twi_wait_for_int();
				if (bzh_lib_avr8_int_twi_check_restart())
				{
					//sla+r
					bzh_lib_avr8_int_twi_data_put(bzh_lib_avr8_ext_ds1631_addr(false));
					bzh_lib_avr8_int_twi_wait_for_int();
					if (bzh_lib_avr8_int_twi_check_gen(TW_MR_SLA_ACK))
					{
						//enable ack
						bzh_lib_avr8_int_twi_cmd_set('a',true);
						
						//read data
						bzh_lib_avr8_int_twi_data_get();
						bzh_lib_avr8_int_twi_wait_for_int();
						if (bzh_lib_avr8_int_twi_check_gen(TW_MR_DATA_ACK))
						{
							temp_reg_h = TWDR;
							
							//disable ack
							bzh_lib_avr8_int_twi_cmd_set('a',false);
							
							//read data
							bzh_lib_avr8_int_twi_data_get();
							bzh_lib_avr8_int_twi_wait_for_int();
							if (bzh_lib_avr8_int_twi_check_gen(TW_MR_DATA_NACK))
							{
								temp_reg_l = TWDR;
								temp_reg = bzh_lib_avr8_ext_ds1631_h_and_l_uchar_to_uint(temp_reg_h,temp_reg_l);
							}
						}
					}
				}
			}
		}
	}
	
	//stop
	bzh_lib_avr8_int_twi_cmd_stop();
	return temp_reg;
}

bool bzh_lib_avr8_ext_ds1631_th_reg_put(unsigned int th_reg)
{
	bool result = false;
	//start
	bzh_lib_avr8_int_twi_cmd_start();
	bzh_lib_avr8_int_twi_wait_for_int();
	if (bzh_lib_avr8_int_twi_check_start())
	{
		//sla+w
		bzh_lib_avr8_int_twi_data_put(bzh_lib_avr8_ext_ds1631_addr(true));
		bzh_lib_avr8_int_twi_wait_for_int();
		if (bzh_lib_avr8_int_twi_check_gen(TW_MT_SLA_ACK))
		{
			//cmd
			bzh_lib_avr8_int_twi_data_put(BZH_LIB_AVR8_EXT_DS1631_CMD_ACCESS_TH);
			bzh_lib_avr8_int_twi_wait_for_int();
			if (bzh_lib_avr8_int_twi_check_gen(TW_MT_DATA_ACK))
			{
				//msb data write
				bzh_lib_avr8_int_twi_data_put(bzh_lib_avr8_ext_ds1631_uint_to_h_or_l_uchar(th_reg,true));
				bzh_lib_avr8_int_twi_wait_for_int();
				if (bzh_lib_avr8_int_twi_check_gen(TW_MT_DATA_ACK))
				{
					//lsb data write
					bzh_lib_avr8_int_twi_data_put(bzh_lib_avr8_ext_ds1631_uint_to_h_or_l_uchar(th_reg,false));
					bzh_lib_avr8_int_twi_wait_for_int();
					if (bzh_lib_avr8_int_twi_check_gen(TW_MT_DATA_ACK))
					{
						result = true;
					}
				}
			}
		}
	}
	
	//stop
	bzh_lib_avr8_int_twi_cmd_stop();
	return result;
}

bool bzh_lib_avr8_ext_ds1631_tl_reg_put(unsigned int th_reg)
{
	bool result = false;
	//start
	bzh_lib_avr8_int_twi_cmd_start();
	bzh_lib_avr8_int_twi_wait_for_int();
	if (bzh_lib_avr8_int_twi_check_start())
	{
		//sla+w
		bzh_lib_avr8_int_twi_data_put(bzh_lib_avr8_ext_ds1631_addr(true));
		bzh_lib_avr8_int_twi_wait_for_int();
		if (bzh_lib_avr8_int_twi_check_gen(TW_MT_SLA_ACK))
		{
			//cmd
			bzh_lib_avr8_int_twi_data_put(BZH_LIB_AVR8_EXT_DS1631_CMD_ACCESS_TL);
			bzh_lib_avr8_int_twi_wait_for_int();
			if (bzh_lib_avr8_int_twi_check_gen(TW_MT_DATA_ACK))
			{
				//msb data write
				bzh_lib_avr8_int_twi_data_put(bzh_lib_avr8_ext_ds1631_uint_to_h_or_l_uchar(th_reg,true));
				bzh_lib_avr8_int_twi_wait_for_int();
				if (bzh_lib_avr8_int_twi_check_gen(TW_MT_DATA_ACK))
				{
					//lsb data write
					bzh_lib_avr8_int_twi_data_put(bzh_lib_avr8_ext_ds1631_uint_to_h_or_l_uchar(th_reg,false));
					bzh_lib_avr8_int_twi_wait_for_int();
					if (bzh_lib_avr8_int_twi_check_gen(TW_MT_DATA_ACK))
					{
						result = true;
					}
				}
			}
		}
	}
	
	//stop
	bzh_lib_avr8_int_twi_cmd_stop();
	return result;
}

unsigned int bzh_lib_avr8_ext_ds1631_th_reg_get()
{
	unsigned int temp_reg_h = 0x00;
	unsigned int temp_reg_l = 0x00;
	unsigned int temp_reg =  0x0000;
	
	//start
	bzh_lib_avr8_int_twi_cmd_start();
	bzh_lib_avr8_int_twi_wait_for_int();
	if (bzh_lib_avr8_int_twi_check_start())
	{
		//sla+w
		bzh_lib_avr8_int_twi_data_put(bzh_lib_avr8_ext_ds1631_addr(true));
		bzh_lib_avr8_int_twi_wait_for_int();
		if (bzh_lib_avr8_int_twi_check_gen(TW_MT_SLA_ACK))
		{
			//cmd
			bzh_lib_avr8_int_twi_data_put(BZH_LIB_AVR8_EXT_DS1631_CMD_ACCESS_TH);
			bzh_lib_avr8_int_twi_wait_for_int();
			if (bzh_lib_avr8_int_twi_check_gen(TW_MT_DATA_ACK))
			{
				//restart
				bzh_lib_avr8_int_twi_cmd_restart();
				bzh_lib_avr8_int_twi_wait_for_int();
				if (bzh_lib_avr8_int_twi_check_restart())
				{
					//sla+r
					bzh_lib_avr8_int_twi_data_put(bzh_lib_avr8_ext_ds1631_addr(false));
					bzh_lib_avr8_int_twi_wait_for_int();
					if (bzh_lib_avr8_int_twi_check_gen(TW_MR_SLA_ACK))
					{
						//enable ack
						bzh_lib_avr8_int_twi_cmd_set('a',true);
						
						//read data
						bzh_lib_avr8_int_twi_data_get();
						bzh_lib_avr8_int_twi_wait_for_int();
						if (bzh_lib_avr8_int_twi_check_gen(TW_MR_DATA_ACK))
						{
							temp_reg_h = TWDR;
							
							//disable ack
							bzh_lib_avr8_int_twi_cmd_set('a',false);
							
							//read data
							bzh_lib_avr8_int_twi_data_get();
							bzh_lib_avr8_int_twi_wait_for_int();
							if (bzh_lib_avr8_int_twi_check_gen(TW_MR_DATA_NACK))
							{
								temp_reg_l = TWDR;
								temp_reg = bzh_lib_avr8_ext_ds1631_h_and_l_uchar_to_uint(temp_reg_h,temp_reg_l);
							}
						}
					}
				}
			}
		}
	}
	
	//stop
	bzh_lib_avr8_int_twi_cmd_stop();
	return temp_reg;
}

unsigned int bzh_lib_avr8_ext_ds1631_tl_reg_get()
{
	unsigned int temp_reg_h = 0x00;
	unsigned int temp_reg_l = 0x00;
	unsigned int temp_reg =  0x0000;
	
	//start
	bzh_lib_avr8_int_twi_cmd_start();
	bzh_lib_avr8_int_twi_wait_for_int();
	if (bzh_lib_avr8_int_twi_check_start())
	{
		//sla+w
		bzh_lib_avr8_int_twi_data_put(bzh_lib_avr8_ext_ds1631_addr(true));
		bzh_lib_avr8_int_twi_wait_for_int();
		if (bzh_lib_avr8_int_twi_check_gen(TW_MT_SLA_ACK))
		{
			//cmd
			bzh_lib_avr8_int_twi_data_put(BZH_LIB_AVR8_EXT_DS1631_CMD_ACCESS_TL);
			bzh_lib_avr8_int_twi_wait_for_int();
			if (bzh_lib_avr8_int_twi_check_gen(TW_MT_DATA_ACK))
			{
				//restart
				bzh_lib_avr8_int_twi_cmd_restart();
				bzh_lib_avr8_int_twi_wait_for_int();
				if (bzh_lib_avr8_int_twi_check_restart())
				{
					//sla+r
					bzh_lib_avr8_int_twi_data_put(bzh_lib_avr8_ext_ds1631_addr(false));
					bzh_lib_avr8_int_twi_wait_for_int();
					if (bzh_lib_avr8_int_twi_check_gen(TW_MR_SLA_ACK))
					{
						//enable ack
						bzh_lib_avr8_int_twi_cmd_set('a',true);
						
						//read data
						bzh_lib_avr8_int_twi_data_get();
						bzh_lib_avr8_int_twi_wait_for_int();
						if (bzh_lib_avr8_int_twi_check_gen(TW_MR_DATA_ACK))
						{
							temp_reg_h = TWDR;
							
							//disable ack
							bzh_lib_avr8_int_twi_cmd_set('a',false);
							
							//read data
							bzh_lib_avr8_int_twi_data_get();
							bzh_lib_avr8_int_twi_wait_for_int();
							if (bzh_lib_avr8_int_twi_check_gen(TW_MR_DATA_NACK))
							{
								temp_reg_l = TWDR;
								temp_reg = bzh_lib_avr8_ext_ds1631_h_and_l_uchar_to_uint(temp_reg_h,temp_reg_l);
							}
						}
					}
				}
			}
		}
	}
	
	//stop
	bzh_lib_avr8_int_twi_cmd_stop();
	return temp_reg;
}

bool bzh_lib_avr8_ext_ds1631_temp_string_maker(char string[10],unsigned int uint)
{
	int temp_int = bzh_lib_avr8_ext_ds1631_uint_to_h_or_l_uchar(uint,true);
	unsigned int temp_float_raw = bzh_lib_avr8_ext_ds1631_uint_to_h_or_l_uchar(uint,false);
	unsigned int temp_float = 0;
	
	temp_float_raw =  temp_float_raw & 0xf0;
	
	if ((temp_float_raw & 0x10) == 0x10)
	{
		temp_float += 625;
	}
	if ((temp_float_raw & 0x20) == 0x20)
	{
		temp_float += 1250;
	}
	if ((temp_float_raw & 0x40) == 0x40)
	{
		temp_float += 2500;
	}
	if ((temp_float_raw & 0x80) == 0x80)
	{
		temp_float += 5000;
	}
	
	int str_len = sprintf(string,"%3d.%04u",temp_int,temp_float);
	
	if ((string[8] == '\0' || string[9] == '\0') && (str_len == 8 || str_len == 9))
	{
		return true;
	} 
	else
	{
		string[10] = '\0';
		return false;
	}
}

#endif /* BZH_LIB_AVR8_EXT_DS1631_H_ */