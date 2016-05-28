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

//���Ƿ����ж�,��ֹ����
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

//�������к���

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

//ʵ�����к���

/*************************************************************************
����:��nRF24L01�ļĴ���
����:��Ҫ��ȡ�ļĴ�����ַ
����:��ȡ���üĴ���ֵ
*************************************************************************/
unsigned char nrf24l01p_reg_read(unsigned char nrf24l01p_reg_addr)
{
	unsigned char reg_value = 0x00;
	
	spi_csn_bit_control(0);//Ƭѡ���ʹ�������
	
	spi_rw(nrf24l01p_reg_addr|R_REGSITER);//���Ͷ������Լ���ȡ�ļĴ�����ַ
	reg_value = spi_rw(0x00);//���õ������ݴ������
	
	spi_csn_bit_control(1);//Ƭѡ���ߴ������
	return reg_value;
}

/*************************************************************************
����:дnRF24L01�ļĴ���
����:�Ĵ�����ַ��Ҫд����ֽ���
*************************************************************************/
void nrf24l01p_reg_write(unsigned char nrf24l01p_reg_addr,unsigned char reg_value)
{
	spi_csn_bit_control(0);//Ƭѡ���ʹ�������
	
	spi_rw(nrf24l01p_reg_addr|W_REGSITER);//����д�����Լ�д��ļĴ�����ַ
	spi_rw(reg_value);//��������ֵд��Ĵ���
	
	spi_csn_bit_control(1);//Ƭѡ���ߴ������
}

/*************************************************************************
����:��nRF24L01�����ݻ�����
����:��Ҫ��ȡ�Ļ�������ַ,Ŀ������ָ��,��Ҫ��ȡ���ֽ���
*************************************************************************/
void nrf24l01p_buffer_read(char nrf24l01p_reg_addr,char* buffer,char num)
{
	spi_csn_bit_control(0);//Ƭѡ���ʹ�������
	
	//spi_rw(nrf24l01p_reg_addr|R_REGSITER);//ѡ��Ҫ�����ĵ�ַ
	spi_rw(nrf24l01p_reg_addr);//ѡ��Ҫ�����ĵ�ַ
	for (int i = 0;i<num;i++)
	{
		buffer[i] = spi_rw(NOP);
	}
	
	spi_csn_bit_control(1);//Ƭѡ���ߴ������
	
	nrf24l01p_flush_rx_fifo();
}

/*************************************************************************
����:дnRF24L01�����ݻ�����
����:��Ҫд��Ļ�������ַ,Ŀ������ָ��,��Ҫд����ֽ���
*************************************************************************/
void nrf24l01p_buffer_write(char nrf24l01p_reg_addr,char* buffer,char num)
{
	nrf24l01p_flush_tx_fifo();
	
	spi_csn_bit_control(0);//Ƭѡ���ʹ�������
	
	//spi_rw(nrf24l01p_reg_addr|W_REGSITER);//ѡ��Ҫ�����ĵ�ַ
	spi_rw(nrf24l01p_reg_addr);//ѡ��Ҫ�����ĵ�ַ
	for (int i = 0;i<num;i++)
	{
		spi_rw(buffer[i]);
	}
	
	spi_csn_bit_control(1);//Ƭѡ���ߴ������
	//delay_reduce(100);
}

/*************************************************************************
����:��nRF24L01��Ϊ����ģʽ
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
����:��nRF24L01��Ϊ����ģʽ
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
����:��nRF24L01��Ϊ�ϵ�ģʽ
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
����:��nRF24L01��Ϊ����ģʽ
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
����:���RX FIFO�Ĵ���
*************************************************************************/
void nrf24l01p_flush_rx_fifo()
{
	spi_csn_bit_control(0);
	spi_rw(FLUSH_RX);//���rx fifo�Ĵ���
	spi_csn_bit_control(1);
}

/*************************************************************************
����:���TX FIFO�Ĵ���
*************************************************************************/
void nrf24l01p_flush_tx_fifo()
{
	spi_csn_bit_control(0);
	spi_rw(FLUSH_TX);
	spi_csn_bit_control(1);
}

/*************************************************************************
����:���ָ���ж�
����:TX_DS���ѡ��,RX_DR���ѡ��,MAX_RT���ѡ��
ѡ��:1,���,2,�����
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
����:�жϴ����ӿ�
ע��:���º�����Ҫ�Լ����ⲿ�ļ�lib_nrf24l01p_irq_handle.h��ʵ��
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
		nrf24l01p_irq_clear(1,0,0);//���TX_DS�ж�
		nrf24l01p_irq_tx_ds_handle();
		nrf24l01p_flush_tx_fifo();//���tx fifo
	}
	if (irq_status&(1<<RX_DR))
	{
		nrf24l01p_irq_clear(0,1,0);//���RX_DR�ж�
		nrf24l01p_irq_rx_dr_handle();
		nrf24l01p_flush_rx_fifo();//���rx fifo
	}
	if (irq_status&(1<<MAX_RT))
	{
		nrf24l01p_irq_clear(0,0,1);//���MAX_RT�ж�
		nrf24l01p_irq_max_rt_handle();
		nrf24l01p_flush_tx_fifo();//���tx fifo
	}
}

#endif /* BZH_LIB_AVR8_NRF24L01P_H_ */