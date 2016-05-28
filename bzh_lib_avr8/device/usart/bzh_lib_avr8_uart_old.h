/*
 * bzh_lib_avr8_uart.h
 *
 * Created: 2013/2/22 20:27:38
 *  Author: Adam
 */ 


#ifndef BZH_LIB_AVR8_UART_H_
#define BZH_LIB_AVR8_UART_H_

//////////////////////////////////////////////////
// UART 0
//////////////////////////////////////////////////
void uart_0_init(void)
{
	UCSR0A = (0<<U2X0)|(0<<MPCM0);//关闭倍速模式和多处理器通信模式
	UCSR0B = (1<<TXEN0)|(1<<RXEN0);//开启rx和tx
	//UCSR0C默认值
	//UBRR0H默认值
	//UBRR0L = 0x07;//波特率57600,7.3728m
	UBRR1L = 0x0b;//波特率38400,7.3728m
}

//参数:无符号字符变量
void uart_0_send_char(unsigned char data)
{
	/* 等待发送缓冲器为空 */
	while ( !( UCSR0A & (1<<UDRE0)) )
	;
	/* 将数据放入缓冲器，发送数据*/
	UDR0 = data;
}

unsigned char uart_0_recv_char()
{
	/* 等待接收数据*/
	while ( !(UCSR0A & (1<<RXC0)) )
	;
	/* 从缓冲器中获取并返回数据*/
	return UDR0;
}

//参数:字符串首地址,字符串长度(使用sizeof计算),光标归位标志位,换行标志位
void uart_0_send_string_ex(char data_string[],int string_size,int r,int n)
{
	for (int i = 0;i < string_size;i++)
	{
		uart_0_send_char(data_string[i]);
	}
	
	if (r == 0)
	{
		asm volatile ("nop");
	}
	else
	{
		uart_0_send_char('\r');//光标归位
	}
	
	if (n == 0)
	{
		asm volatile ("nop");
	} 
	else
	{
		uart_0_send_char('\n');//换行
	}
}

void uart_0_send_string(char data_string[],int string_size)
{
	uart_0_send_string_ex(data_string,string_size,1,1);
}



//////////////////////////////////////////////////
// UART 1
//////////////////////////////////////////////////
void uart_1_init(void)
{
	UCSR1A = (0<<U2X1)|(0<<MPCM1);//关闭倍速模式和多处理器通信模式
	UCSR1B = (1<<TXEN1)|(1<<RXEN1);//开启rx和tx
	//UCSR1C默认值
	//UBRR1H默认值
	//UBRR0L = 0x07;//波特率57600,7.3728m
	UBRR1L = 0x0b;//波特率38400,7.3728m
}

//参数:无符号字符变量
void uart_1_send_char(unsigned char data)
{
	/* 等待发送缓冲器为空 */
	while ( !( UCSR1A & (1<<UDRE1)) )
	;
	/* 将数据放入缓冲器，发送数据*/
	UDR1 = data;
}

unsigned char uart_1_recv_char()
{
	/* 等待接收数据*/
	while ( !(UCSR1A & (1<<RXC1)) )
	;
	/* 从缓冲器中获取并返回数据*/
	return UDR1;
}

//参数:字符串首地址,字符串长度(使用sizeof计算),光标归位标志位,换行标志位
void uart_1_send_string_ex(char data_string[],int string_size,int r,int n)
{
	for (int i = 0;i < string_size;i++)
	{
		uart_1_send_char(data_string[i]);
	}
	
	if (r == 0)
	{
		asm volatile ("nop");
	}
	else
	{
		uart_1_send_char('\r');//光标归位
	}
	
	if (n == 0)
	{
		asm volatile ("nop");
	} 
	else
	{
		uart_1_send_char('\n');//换行
	}
}

void uart_1_send_string(char data_string[],int string_size)
{
	uart_1_send_string_ex(data_string,string_size,1,1);
}

#endif /* BZH_LIB_AVR8_UART_H_ */