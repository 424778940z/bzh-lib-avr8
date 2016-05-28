/*
 * bzh_lib_avr8_nrf24l01p.h
 *
 * Created: 2013/3/1 15:35:26
 *  Author: Adam
 */ 


#ifndef BZH_LIB_AVR8_NRF24L01P_H_
#define BZH_LIB_AVR8_NRF24L01P_H_

#include "D:\works\avr\bzh_avr8_lib\system\lib_delay.h"

#include "lib_nrf24l01p_pin.h"
#include "lib_nrf24l01p_define.h"
#include "lib_nrf24l01p_irq_handle.h"

//库是否定义判定,防止出错
#ifndef BZH_LIB_AVR8_NRF24L01P_PIN_H_
# error "not define LIB_NRF24L01P_PIN_H_ ; LIB_NRF24L01P_H_ can not work"
#endif

#ifndef BZH_LIB_AVR8_NRF24L01P_DEFINE_H_
# error "not define LIB_NRF24L01P_DEFINE_H_ ; LIB_NRF24L01P_H_ can not work"
#endif

#ifndef BZH_LIB_AVR8_NRF24L01P_IRQ_HANDLE_H_
# error "not define LIB_NRF24L01P_IRQ_HANDLE_H_ ; LIB_NRF24L01P_H_ can not work"
#endif

#ifndef BZH_LIB_AVR8_VIRTUAL_SPI_H_
# error "not define LIB_VIRTUAL_SPI_H_ ; LIB_NRF24L01P_H_ can not work"
#endif

#ifndef BZH_LIB_AVR8_DELAY_H_
# error "not define LIB_DELAY_H_ ; LIB_NRF24L01P_H_ can not work"
#endif

//声明所有函数

char nrf24l01p_reg_read(char nrf24l01p_reg_addr,char reg_value);
void nrf24l01p_reg_write(char nrf24l01p_reg_addr);

void nrf24l01p_buffer_read(char nrf24l01p_reg_addr,char* buffer,char num);
void nrf24l01p_buffer_write(char nrf24l01p_reg_addr,char* buffer,char num);

void nrf24l01p_mode_set_rx();
void nrf24l01p_mode_set_tx();
//void nrf24l01p_mode_set_standby_1();
//void nrf24l01p_mode_set_standby_2();
void nrf24l01p_mode_set_power_down();

void nrf24l01p_flush_rx_fifo();
void nrf24l01p_flush_tx_fifo();

void nrf24l01p_irq_clear(char irq_code);
void nrf24l01p_irq_handle();

extern void nrf24l01p_irq_tx_ds_handle();
extern void nrf24l01p_irq_rx_dr_handle();
extern void nrf24l01p_irq_max_rt_handle();

//实现所有函数

/*************************************************************************
功能:读nRF24L01的寄存器
参数:需要读取的寄存器地址
返回:读取到得寄存器值
*************************************************************************/
unsigned char nrf24l01p_reg_read(unsigned char nrf24l01p_reg_addr)
{
	unsigned char reg_value = 0x00;
	
	spi_csn_bit_control(0);//片选拉低传送数据
	
	spi_rw(nrf24l01p_reg_addr|R_REGSITER);//传送读命令以及读取的寄存器地址
	reg_value = spi_rw(0x00);//将得到的数据存入变量
	
	spi_csn_bit_control(1);//片选拉高传送完毕
	return reg_value;
}

/*************************************************************************
功能:写nRF24L01的寄存器
参数:寄存器地址和要写入得字节数
*************************************************************************/
void nrf24l01p_reg_write(unsigned char nrf24l01p_reg_addr,unsigned char reg_value)
{
	spi_csn_bit_control(0);//片选拉低传送数据
	
	spi_rw(nrf24l01p_reg_addr|W_REGSITER);//传送写命令以及写入的寄存器地址
	spi_rw(reg_value);//将变量的值写入寄存器
	
	spi_csn_bit_control(1);//片选拉高传送完毕
}

/*************************************************************************
功能:读nRF24L01的数据缓冲区
参数:需要读取的缓冲区地址,目标数组指针,需要读取的字节数
*************************************************************************/
void nrf24l01p_buffer_read(char nrf24l01p_reg_addr,char* buffer,char num)
{
	spi_csn_bit_control(0);//片选拉低传送数据
	
	//spi_rw(nrf24l01p_reg_addr|R_REGSITER);//选择要操作的地址
	spi_rw(nrf24l01p_reg_addr);//选择要操作的地址
	for (int i = 0;i<num;i++)
	{
		buffer[i] = spi_rw(NOP);
	}
	
	spi_csn_bit_control(1);//片选拉高传送完毕
	
	nrf24l01p_flush_rx_fifo();
}

/*************************************************************************
功能:写nRF24L01的数据缓冲区
参数:需要写入的缓冲区地址,目标数组指针,需要写入的字节数
*************************************************************************/
void nrf24l01p_buffer_write(char nrf24l01p_reg_addr,char* buffer,char num)
{
	nrf24l01p_flush_tx_fifo();
	
	spi_csn_bit_control(0);//片选拉低传送数据
	
	//spi_rw(nrf24l01p_reg_addr|W_REGSITER);//选择要操作的地址
	spi_rw(nrf24l01p_reg_addr);//选择要操作的地址
	for (int i = 0;i<num;i++)
	{
		spi_rw(buffer[i]);
	}
	
	spi_csn_bit_control(1);//片选拉高传送完毕
	//delay_reduce(100);
}

/*************************************************************************
功能:将nRF24L01置为接收模式
*************************************************************************/
void nrf24l01p_mode_set_rx()
{
	nrf24l01p_ce_bit_control(0);
	unsigned char reg_config_data = 0x00;
	reg_config_data = nrf24l01p_reg_read(REG_CONFIG);
	reg_config_data |= (1<<PRIM_RX);
	nrf24l01p_reg_write(REG_CONFIG,reg_config_data);
	nrf24l01p_ce_bit_control(1);
}

/*************************************************************************
功能:将nRF24L01置为发送模式
*************************************************************************/
void nrf24l01p_mode_set_tx()
{
	nrf24l01p_ce_bit_control(0);
	unsigned char reg_config_data = 0x00;
	reg_config_data = nrf24l01p_reg_read(REG_CONFIG);
	reg_config_data &= ~(1<<PRIM_RX);
	nrf24l01p_reg_write(REG_CONFIG,reg_config_data);
	nrf24l01p_ce_bit_control(1);
}

/*************************************************************************
功能:将nRF24L01置为上电模式
*************************************************************************/
void nrf24l01p_mode_set_power_up()
{
	nrf24l01p_ce_bit_control(0);
	unsigned char reg_config_data = 0x00;
	reg_config_data = nrf24l01p_reg_read(REG_CONFIG);
	reg_config_data |= (1<<PWR_UP);
	nrf24l01p_reg_write(REG_CONFIG,reg_config_data);
	nrf24l01p_ce_bit_control(1);
	
}

/*************************************************************************
功能:将nRF24L01置为掉电模式
*************************************************************************/
void nrf24l01p_mode_set_power_down()
{
	nrf24l01p_ce_bit_control(0);
	unsigned char reg_config_data = 0x00;
	reg_config_data = nrf24l01p_reg_read(REG_CONFIG);
	reg_config_data |= (1<<PWR_UP);
	nrf24l01p_reg_write(REG_CONFIG,reg_config_data);
	nrf24l01p_ce_bit_control(1);
}

/*************************************************************************
功能:清除RX FIFO寄存器
*************************************************************************/
void nrf24l01p_flush_rx_fifo()
{
	spi_csn_bit_control(0);
	spi_rw(FLUSH_RX);//清空rx fifo寄存器
	spi_csn_bit_control(1);
}

/*************************************************************************
功能:清除TX FIFO寄存器
*************************************************************************/
void nrf24l01p_flush_tx_fifo()
{
	spi_csn_bit_control(0);
	spi_rw(FLUSH_TX);
	spi_csn_bit_control(1);
}

/*************************************************************************
功能:清除指定中断
参数:TX_DS清除选项,RX_DR清除选项,MAX_RT清除选项
选项:1,清除,2,不清除
*************************************************************************/
void nrf24l01p_irq_clear(int cls_irq_tx_ds,int cls_irq_rx_dr,int cls_irq_max_rt)
{
	char reg_status_data = 0x00;
	if (cls_irq_tx_ds == 1)
	{
		reg_status_data |= (1<<TX_DS);
	}
	if (cls_irq_rx_dr == 1)
	{
		reg_status_data |= (1<<RX_DR);
	}
	if (cls_irq_max_rt == 1)
	{
		reg_status_data |= (1<<MAX_RT);
	}
	nrf24l01p_reg_write(REG_STATUS,reg_status_data);
}

/*************************************************************************
功能:中断处理接口
注意:以下函数需要自己在外部文件lib_nrf24l01p_irq_handle.h中实现
nrf24l01p_irq_tx_ds_handle();
nrf24l01p_irq_rx_dr_handle();
nrf24l01p_irq_max_rt_handle();
*************************************************************************/
void nrf24l01p_irq_handle()
{
	unsigned char irq_status;
	irq_status = nrf24l01p_reg_read(REG_STATUS);
	
	if (irq_status&(1<<TX_DS))
	{
		nrf24l01p_irq_clear(1,0,0);//清除TX_DS中断
		nrf24l01p_irq_tx_ds_handle();
		nrf24l01p_flush_tx_fifo();//清除tx fifo
	}
	if (irq_status&(1<<RX_DR))
	{
		nrf24l01p_irq_clear(0,1,0);//清除RX_DR中断
		nrf24l01p_irq_rx_dr_handle();
		nrf24l01p_flush_rx_fifo();//清除rx fifo
	}
	if (irq_status&(1<<MAX_RT))
	{
		nrf24l01p_irq_clear(0,0,1);//清除MAX_RT中断
		nrf24l01p_irq_max_rt_handle();
		nrf24l01p_flush_tx_fifo();//清除tx fifo
	}
}

#endif /* BZH_LIB_AVR8_NRF24L01P_H_ */