/*
 * lib_avr8_74xxxxx.h
 *
 * Created: 2014/9/6 20:37:48
 *  Author: Adam
 */ 


#ifndef BZH_LIB_AVR8_74XXXXX_H_
#define BZH_LIB_AVR8_74XXXXX_H_

#define BZH_LIB_AVR8_74XXXXX_CONFIG_DDR DDRC
#define BZH_LIB_AVR8_74XXXXX_CONFIG_PORT PORTC

#define BZH_LIB_AVR8_74XXXXX_LE PORTC0
#define BZH_LIB_AVR8_74XXXXX_OE PORTC1

void bzh_lib_avr8_74xxxxx_init()
{
	BZH_LIB_AVR8_74XXXXX_CONFIG_DDR = 0x00;
	BZH_LIB_AVR8_74XXXXX_CONFIG_DDR |= ((1<<BZH_LIB_AVR8_74XXXXX_LE)|(1<<BZH_LIB_AVR8_74XXXXX_OE));
	BZH_LIB_AVR8_74XXXXX_CONFIG_PORT = 0x00;
}

unsigned char bzh_lib_avr8_74xxxxx_latch_control(unsigned char bit)
{
	if (bit == 1)
	{
		BZH_LIB_AVR8_74XXXXX_CONFIG_PORT |= (1<<BZH_LIB_AVR8_74XXXXX_LE);
		return 0;
	}
	else if (bit == 0)
	{
		BZH_LIB_AVR8_74XXXXX_CONFIG_PORT &= ~(1<<BZH_LIB_AVR8_74XXXXX_LE);
		return 0;
	}
	else
	{
		return 1;
	}
}

unsigned char bzh_lib_avr8_74xxxxx_output_control(unsigned char bit)
{
	if (bit == 1)
	{
		BZH_LIB_AVR8_74XXXXX_CONFIG_PORT |= (1<<BZH_LIB_AVR8_74XXXXX_OE);
		return 0;
	}
	else if (bit == 0)
	{
		BZH_LIB_AVR8_74XXXXX_CONFIG_PORT &= ~(1<<BZH_LIB_AVR8_74XXXXX_OE);
		return 0;
	}
	else
	{
		return 1;
	}
}

#endif /* BZH_LIB_AVR8_74XXXXX_H_ */