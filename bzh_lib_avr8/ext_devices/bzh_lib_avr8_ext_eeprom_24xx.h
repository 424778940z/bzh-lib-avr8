/*
 * bzh_lib_avr8_ext_eeprom_24xx.h
 *
 * Created: 2015/11/5 21:47:28
 *  Author: 42477
 */ 


#ifndef BZH_LIB_AVR8_EXT_EEPROM_24XX_H_
#define BZH_LIB_AVR8_EXT_EEPROM_24XX_H_

#define BZH_LIB_AVR8_EXT_EEPROM_24XX_BASE_ADDRESS 0xa0
#define BZH_LIB_AVR8_EXT_EEPROM_24XX_A0 0
#define BZH_LIB_AVR8_EXT_EEPROM_24XX_A1 0
#define BZH_LIB_AVR8_EXT_EEPROM_24XX_A2 0

unsigned char bzh_lib_avr8_ext_eeprom_24xx_addr(bool rw)
{
	unsigned char addr = BZH_LIB_AVR8_EXT_EEPROM_24XX_BASE_ADDRESS | (BZH_LIB_AVR8_EXT_EEPROM_24XX_A2<<3) | (BZH_LIB_AVR8_EXT_EEPROM_24XX_A1<<2) | (BZH_LIB_AVR8_EXT_EEPROM_24XX_A2<<1);
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

bool bzh_lib_avr8_ext_eeprom_24xx_write_byte(unsigned char device_addr, unsigned char io_addr_h, unsigned char io_addr_l, unsigned char data)
{
	//start
	bzh_lib_avr8_int_twi_cmd_start();
	bzh_lib_avr8_int_twi_wait_for_int();
	if (bzh_lib_avr8_int_twi_check_start())
	{
		//sla+w
		bzh_lib_avr8_int_twi_data_put(device_addr);
		bzh_lib_avr8_int_twi_wait_for_int();
		if (bzh_lib_avr8_int_twi_check_gen(TW_MT_SLA_ACK))
		{
			//write H address
			bzh_lib_avr8_int_twi_data_put(io_addr_h);
			bzh_lib_avr8_int_twi_wait_for_int();
			if (bzh_lib_avr8_int_twi_check_gen(TW_MT_DATA_ACK))
			{
				//write L address
				bzh_lib_avr8_int_twi_data_put(io_addr_l);
				bzh_lib_avr8_int_twi_wait_for_int();
				if (bzh_lib_avr8_int_twi_check_gen(TW_MT_DATA_ACK))
				{
					//write data
					bzh_lib_avr8_int_twi_data_put(data);
					bzh_lib_avr8_int_twi_wait_for_int();
					if (bzh_lib_avr8_int_twi_check_gen(TW_MT_DATA_ACK))
					{
						return true;
					}
					else
					{
						return false;
					}
				}
				else
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
	
	//stop
	bzh_lib_avr8_int_twi_cmd_stop();
}

#endif /* BZH_LIB_AVR8_EXT_EEPROM_24XX_H_ */