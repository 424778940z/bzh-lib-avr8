/*
 * bzh_lib_avr8_usart.h
 *
 * Created: 2013/2/22 20:27:38
 *  Author: Adam
 */ 


#ifndef BZH_LIB_AVR8_USART_H_
#define BZH_LIB_AVR8_USART_H_

//////////////////////////////////////////////////
// USART 0
//////////////////////////////////////////////////
void lib_usart_0_init()
{
	UCSR0A = (0<<U2X0)|(0<<MPCM0);//�رձ���ģʽ�Ͷദ����ͨ��ģʽ
	UCSR0B = (1<<TXEN0)|(1<<RXEN0);//����rx��tx
	//UCSR0CĬ��ֵ
	//UBRR0HĬ��ֵ
	//UBRR0L = 0x07;//������57600,7.3728m
	UBRR0L = 0x0b;//������38400,7.3728m
	//UBRR0L = 0x0c;//������38400,8.0000m
	//UBRR0L = 0x19;//������38400,16.0000m
}

//����:�޷����ַ�����
void lib_usart_0_send_char(unsigned char data)
{
	/* �ȴ����ͻ�����Ϊ�� */
	while ( !( UCSR0A & (1<<UDRE0)) )
	;
	/* �����ݷ��뻺��������������*/
	UDR0 = data;
}

unsigned char lib_usart_0_recv_char()
{
	/* �ȴ���������*/
	while ( !(UCSR0A & (1<<RXC0)) )
	;
	/* �ӻ������л�ȡ����������*/
	return UDR0;
}

//����:�ַ����׵�ַ,�ַ�������(ʹ��sizeof����),����λ��־λ,���б�־λ
void lib_usart_0_send_string_ex(char data_string[],int string_size,int r,int n)
{
	for (int i = 0;i < string_size;i++)
	{
		lib_usart_0_send_char(data_string[i]);
	}
	
	if (r == 0)
	{
		asm volatile ("nop");
	}
	else
	{
		lib_usart_0_send_char('\r');//����λ
	}
	
	if (n == 0)
	{
		asm volatile ("nop");
	} 
	else
	{
		lib_usart_0_send_char('\n');//����
	}
}

//����:�ַ����׵�ַ,�ַ�������(ʹ��sizeof����)
void lib_usart_0_send_string(char data_string[],int string_size)
{
	lib_usart_0_send_string_ex(data_string,string_size,1,1);
}

//����,�ú�������֤���԰�ȫ,�м�С���ܵ����ڴ����
//����:�ַ����׵�ַ,�ַ�������(ʹ��sizeof����),����λ��־λ,���б�־λ
void lib_usart_0_send_string_unsafe_ex(char data_string[],int r,int n)
{
	int string_index = 0;
	while (1)
	{
		
		if (data_string[string_index] != '\0')
		{
			lib_usart_0_send_char(data_string[string_index]);
		} 
		else
		{
			break;
		}
		string_index++;
	}
	
	if (r == 0)
	{
		asm volatile ("nop");
	}
	else
	{
		lib_usart_0_send_char('\r');//����λ
	}
	
	if (n == 0)
	{
		asm volatile ("nop");
	}
	else
	{
		lib_usart_0_send_char('\n');//����
	}
}

//����,�ú�������֤���԰�ȫ,�м�С���ܵ����ڴ����
//����:�ַ����׵�ַ,�ַ�������(ʹ��sizeof����)
void lib_usart_0_send_unsafe_string(char data_string[])
{
	lib_usart_0_send_string_unsafe_ex(data_string,1,1);
}

//////////////////////////////////////////////////
// USART 1
//////////////////////////////////////////////////
void lib_usart_1_init()
{
	UCSR1A = (0<<U2X1)|(0<<MPCM1);//�رձ���ģʽ�Ͷദ����ͨ��ģʽ
	UCSR1B = (1<<TXEN1)|(1<<RXEN1);//����rx��tx
	//UCSR1CĬ��ֵ
	//UBRR1HĬ��ֵ
	//UBRR1L = 0x07;//������57600,7.3728m
	UBRR1L = 0x0b;//������38400,7.3728m
}

//����:�޷����ַ�����
void lib_usart_1_send_char(unsigned char data)
{
	/* �ȴ����ͻ�����Ϊ�� */
	while ( !( UCSR1A & (1<<UDRE1)) )
	;
	/* �����ݷ��뻺��������������*/
	UDR1 = data;
}

unsigned char usart_1_recv_char()
{
	/* �ȴ���������*/
	while ( !(UCSR1A & (1<<RXC1)) )
	;
	/* �ӻ������л�ȡ����������*/
	return UDR1;
}

//����:�ַ����׵�ַ,�ַ�������(ʹ��sizeof����),����λ��־λ,���б�־λ
void lib_usart_1_send_string_ex(char data_string[],int string_size,int r,int n)
{
	for (int i = 0;i < string_size;i++)
	{
		lib_usart_1_send_char(data_string[i]);
	}
	
	if (r == 0)
	{
		asm volatile ("nop");
	}
	else
	{
		lib_usart_1_send_char('\r');//����λ
	}
	
	if (n == 0)
	{
		asm volatile ("nop");
	} 
	else
	{
		lib_usart_1_send_char('\n');//����
	}
}

//����:�ַ����׵�ַ,�ַ�������(ʹ��sizeof����)
void lib_usart_1_send_string(char data_string[],int string_size)
{
	lib_usart_1_send_string_ex(data_string,string_size,1,1);
}

#endif /* BZH_LIB_AVR8_USART_H_ */