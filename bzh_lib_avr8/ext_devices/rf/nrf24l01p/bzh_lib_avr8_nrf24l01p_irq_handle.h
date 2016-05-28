/*
 * bzh_lib_avr8_nrf24l01p_irq_handle.h
 *
 * Created: 2013/3/2 20:26:21
 *  Author: Adam
 */ 


#ifndef BZH_LIB_AVR8_NRF24L01P_IRQ_HANDLE_H_
#define BZH_LIB_AVR8_NRF24L01P_IRQ_HANDLE_H_

extern void nrf24l01p_buffer_read(char nrf24l01p_reg_addr,char* buffer,char num);
extern void nrf24l01p_buffer_write(char nrf24l01p_reg_addr,char* buffer,char num);
extern char test_send_data[32];

/*
本头文件内为自定义中断处理函数,外围框架代码自动清除中断和fifo
因此,这里的函数只需要编写想做的事情的代码就可以
如果需要其他变量或者函数请加上extern然后引用即可

示例代码框架

void nrf24l01p_irq_tx_ds_handle()
{
	
}

void nrf24l01p_irq_rx_dr_handle()
{
	
}

void nrf24l01p_irq_max_rt_handle()
{
	
}

*/

void nrf24l01p_irq_tx_ds_handle()
{
	char usart_string[] = "IRQ TX_DS";
	usart_1_send_string(usart_string,sizeof(usart_string));
	led_bit_flash(1,1,100);
	
	nrf24l01p_buffer_write(W_TX_PAYLOAD,test_send_data,32);
}

void nrf24l01p_irq_rx_dr_handle()
{
	char usart_string[] = "IRQ RX_DR";
	usart_1_send_string(usart_string,sizeof(usart_string));
	led_bit_flash(2,1,100);
}

void nrf24l01p_irq_max_rt_handle()
{
	char usart_string[] = "IRQ MAX_RT";
	usart_1_send_string(usart_string,sizeof(usart_string));
	led_bit_flash(3,1,100);
	
	nrf24l01p_buffer_write(W_TX_PAYLOAD,test_send_data,32);
}

#endif /* BZH_LIB_AVR8_NRF24L01P_IRQ_HANDLE_H_ */