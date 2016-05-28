/*
 * bzh_lib_avr8_virtual_spi.h
 *
 * Created: 2013/2/23 11:31:54
 *  Author: Adam
 */


#ifndef BZH_LIB_AVR8_VIRTUAL_SPI_H_
#define BZH_LIB_AVR8_VIRTUAL_SPI_H_

#ifndef BZH_LIB_AVR8_DELAY_H_
# error "not define LIB_DELAY_H_ ; LIB_VIRTUAL_SPI_H_ can not work"
#endif

#define SPI_DDR		DDRA
#define SPI_PIN		PINA
#define SPI_PORT	PORTA

#define MISO	PA0
#define MOSI	PA1
#define SCK		PA2
#define CSN		PA3

int spi_csn_bit_control(int bit);
int spi_mosi_bit_control(int bit);
int spi_miso_bit_control(int bit);
int spi_sck_bit_control(int bit);

void spi_init();
unsigned char spi_rw(unsigned char send_data);

#if 0
void spi_send(unsigned char send_data);
unsigned char spi_recv();
#endif

long int delay_time = 20;//延迟

int spi_csn_bit_control(int bit)
{
	if (bit == 1)
	{
		SPI_PORT |= (1<<CSN);
		return 0;
	}
	else if (bit == 0)
	{
		SPI_PORT &= ~(1<<CSN);
		return 0;
	}
	else
	{
		return -1;
	}
}

int spi_mosi_bit_control(int bit)
{
	if (bit == 1)
	{
		SPI_PORT |= (1<<MOSI);
		return 0;
	}
	else if (bit == 0)
	{
		SPI_PORT &= ~(1<<MOSI);
		return 0;
	}
	else
	{
		return -1;
	}
}

int spi_miso_bit_control(int bit)
{
	if (bit == 1)
	{
		SPI_PORT |= (1<<MISO);
		return 0;
	}
	else if (bit == 0)
	{
		SPI_PORT &= ~(1<<MISO);
		return 0;
	}
	else
	{
		return -1;
	}
}

int spi_sck_bit_control(int bit)
{
	if (bit == 1)
	{
		SPI_PORT |= (1<<SCK);
		return 0;
	}
	else if (bit == 0)
	{
		SPI_PORT &= ~(1<<SCK);
		return 0;
	}
	else
	{
		return -1;
	}
}

void spi_init()
{
	SPI_DDR = 0x00;
	SPI_DDR |= ((1<<MOSI)|(1<<SCK)|(1<<CSN));//设置MOSI、SCK、CSN输出，其余输入
	SPI_DDR &= ~((1<<MISO));
	
	SPI_PORT = 0x00;
}

unsigned char spi_rw(unsigned char send_data)
{
	unsigned char recv_data = 0x00;

	//SPI0 CPOL=0 CPHA=0
	
	//片选拉低传送数据
	//spi_csn_bit_control(0);
	
	delay_reduce(delay_time);
	
	for (int i = 0;i < 8;i++)
	{
		if (send_data & (0x80 >> i))//高位在前,低位在前改为(0x01<<i)
		{
			spi_mosi_bit_control(1);
		}
		else
		{
			spi_mosi_bit_control(0);
		}
		delay_reduce(delay_time);
		spi_sck_bit_control(1);
		
		if ((SPI_PIN >> MISO) & 0x01)
		{
			recv_data |= (0x80>>i);
		}
		delay_reduce(delay_time);
		spi_sck_bit_control(0);
	}
	delay_reduce(delay_time);
	
	//片选拉高传送完毕
	//spi_csn_bit_control(1);
	
	return recv_data;
}

#if 0

void spi_send(unsigned char send_data)
{
	//片选拉低传送数据
	//spi_csn_bit_control(0);
	
	//SPI0 CPOL=0 CPHA=0
	
	delay_reduce(delay_time);
	
	for (int i = 0;i < 8;i++)
	{
		if (send_data & (0x80 >> i))//高位在前,低位在前改为(0x01<<i)
		{
			spi_mosi_bit_control(1);
			delay_reduce(delay_time);
			spi_sck_bit_control(1);
			delay_reduce(delay_time);
			spi_sck_bit_control(0);
		}
		else
		{
			spi_mosi_bit_control(0);
			delay_reduce(delay_time);
			spi_sck_bit_control(1);
			delay_reduce(delay_time);
			spi_sck_bit_control(0);
		}
	}
	delay_reduce(delay_time);
	//片选拉高传送完毕
	//spi_csn_bit_control(1);
}

unsigned char spi_recv()
{
	unsigned char recv_data = 0x00;
	
	//片选拉低传送数据
	//spi_csn_bit_control(0);
	
	//SPI0 CPOL=0 CPHA=0
	
	delay_reduce(delay_time);
	
	for (int i = 0;i < 8;i++)
	{
		if ((SPI_PIN >> MISO) & 0x01)
		{
			recv_data |= (0x80>>i);
		}
		delay_reduce(delay_time);
		spi_sck_bit_control(1);
		delay_reduce(delay_time);
		spi_sck_bit_control(0);
	}
	delay_reduce(delay_time);
	
	//片选拉高传送完毕
	//spi_csn_bit_control(1);
	return recv_data;
}

#endif

#endif /* BZH_LIB_AVR8_VIRTUAL_SPI_H_ */