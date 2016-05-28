/*
 * bzh_lib_avr8_adc.h
 *
 * Created: 2013/7/26 10:11:53
 *  Author: Adam
 */ 


#ifndef BZH_LIB_AVR8_ADC_H_
#define BZH_LIB_AVR8_ADC_H_

#include <stdlib.h>

#include "d:/works/avr/bzh_avr8_lib/system/lib_delay.h"
#include "D:\works\avr\bzh_avr8_lib\device\usart\lib_usart.h"

//vin = adc * vref /1024
//Vin=(参考电压/256)*ADC（左对齐模式下）

double adc_vref = 4.95;
volatile unsigned int adc_vin = 0x0000;
volatile double adc_cache_v[16];
volatile int adc_cache_index = 0;
volatile double adc_v = 0;
char adc_string_buffer[10];

/*
ISR(ADC_vect,ISR_BLOCK)
{
	lib_adc_irq();
}
*/

void lib_adc_init()
{
	//ADC PORT DDR INIT
	DDRA = 0x00;
	PORTA = 0x00;
	
	//ADC REG INIT
	ADCSRA = 0x00;
	ADMUX = 0x00;
	
	ADCSRA |= ( (1<<ADIE) | (1<<ADEN) | (1<<ADPS0) | (1<<ADPS1) | (1<<ADPS2) );//ADC启用,中断使能,128分频
	
	//ADMUX |= ( (1<<REFS0) | (1<<REFS1) );//2.56V 电源
	ADMUX |= (1<<REFS0);//AVCC电源
	
	//ADMUX |= ( (1<<MUX0) | (1<<MUX1) | (1<<MUX2) | (1<<MUX3) | (1<<MUX4) );//GND通道
	//ADMUX |= ( (1<<MUX1) | (1<<MUX2) | (1<<MUX3) | (1<<MUX4) );//1.22 V (Vbg)通道
}

double lib_adc_count()
{
	adc_v = 0;
	for (int i = 0;i < 16;i++)
	{
		adc_v += adc_cache_v[i];
	}
	
	double adc_temp_min = 0;
	double adc_temp_max = 255;
	int adc_temp_index = 0;
	
	for (adc_temp_index = 0;adc_temp_index < 16;adc_temp_index++)
	{
		if (adc_temp_max > adc_cache_v[adc_temp_index])
		{
			adc_temp_max = adc_cache_v[adc_temp_index];
			adc_temp_index = 0;
		}
	}
	
	for (adc_temp_index = 0;adc_temp_index < 16;adc_temp_index++)
	{
		if (adc_temp_min < adc_cache_v[adc_temp_index])
		{
			adc_temp_min = adc_cache_v[adc_temp_index];
			adc_temp_index = 0;
		}
	}

	adc_v -= adc_temp_max;
	adc_v -= adc_temp_min;
	
	adc_v /= 14;
	
	return adc_v;
}

void lib_adc_handle()
{
	dtostrf(adc_v,1,7,adc_string_buffer);
	lib_usart_0_send_unsafe_string(adc_string_buffer);
}

void lib_adc_irq()
{
	adc_vin = ADC;
	if (adc_cache_index < 16)
	{
		adc_cache_v[adc_cache_index] = (adc_vin * adc_vref) / 1024;
		adc_cache_index++;
	}
	else
	{
		lib_adc_count();
		//lib_adc_handle();
		adc_cache_index = 0;
	}
}

#endif /* BZH_LIB_AVR8_ADC_H_ */