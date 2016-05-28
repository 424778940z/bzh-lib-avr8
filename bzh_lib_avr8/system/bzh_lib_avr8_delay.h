/*
 * bzh_lib_avr8_delay.h
 *
 * Created: 2011/12/30 11:11:26
 *  Author: Adam
 */ 


#ifndef BZH_LIB_AVR8_DELAY_H_
#define BZH_LIB_AVR8_DELAY_H_

void delay_reduce(long int delay)
{
	/*
    for(;delay > 1;delay--)
	{
		asm volatile ("nop");
	}
	*/
	while (delay)
	{
		delay--;
		asm volatile ("nop");
	}
}

#endif /* BZH_LIB_AVR8_DELAY_H_ */