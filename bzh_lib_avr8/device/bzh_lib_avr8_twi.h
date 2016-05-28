/*
 * bzh_lib_avr8_int_twi.h
 *
 * Created: 2015/10/27 11:25:10
 *  Author: 42477
 */ 


#ifndef BZH_LIB_AVR8_INT_TWI_H_
#define BZH_LIB_AVR8_INT_TWI_H_

#include <stdbool.h>
#include <util/twi.h>

#include <util/delay.h>

#define BZH_LIB_AVR8_INT_TWI_DELAY_US 10
#define BZH_LIB_AVR8_INT_TWI_DELAY_MS 1

//prescaler[1,4,16,64],frequency
void bzh_lib_avr8_int_twi_init(unsigned int prescaler,unsigned int frequency)
{
	switch (prescaler)
	{
		case 1:
		TWSR &= ~(1<<TWPS1) & ~(1<<TWPS0);//Prescaler 1
		break;
		
		case 4:
		TWSR |= (1<<TWPS0);//Prescaler 4
		TWSR &= ~(1<<TWPS1);
		break;
		
		case 16:
		TWSR |= (1<<TWPS1);//Prescaler 16
		TWSR &= ~(1<<TWPS0);
		break;
		
		case 64:
		TWSR |= (1<<TWPS1) | (1<<TWPS0);//Prescaler 64
		break;
		
		default:
		break;
	}
	
	//TWBR = 0x0C;//prescaler 1,400khz
	TWBR = 0x48;//prescaler 1,100khz
	
	TWCR &= ~(1<<TWIE);
	TWCR |= (1<<TWEN);
	
}

void bzh_lib_avr8_int_twi_emit()
{
	TWCR |= (1<<TWINT) | (1<<TWEN);
}

/*
 * first arg for select bit
 * s = start
 * r = restart
 * p = stop
 * a = ack
 * second arg for set bit
 * true = 1
 * false = 0
 */
void bzh_lib_avr8_int_twi_cmd_set(unsigned char cmd, bool bit)
{
	switch (cmd)
	{
		case 's':
		if (bit)
		{
			TWCR |= (1<<TWSTA);
		}
		else
		{
			TWCR &= ~(1<<TWSTA) & ~(1<<TWINT);
		}
		break;
		
		case 'r':
		if (bit)
		{
			TWCR |= (1<<TWSTA);
		}
		else
		{
			TWCR &= ~(1<<TWSTA) & ~(1<<TWINT);
		}
		break;
		
		case 'p':
		if (bit)
		{
			TWCR &= ~(1<<TWSTA) & ~(1<<TWINT);
			TWCR |= (1<<TWSTO);
		}
		else
		{
			TWCR &= ~(1<<TWSTO);
		}
		break;
		
		case 'a':
		if (bit)
		{
			TWCR |= (1<<TWEA);
		}
		else
		{
			TWCR &= ~(1<<TWEA);
		}
		break;
		
		default:
		break;
	}
}

void bzh_lib_avr8_int_twi_cmd_start()
{
	bzh_lib_avr8_int_twi_cmd_set('s',true);
	bzh_lib_avr8_int_twi_emit();
	_delay_us(BZH_LIB_AVR8_INT_TWI_DELAY_US);
	bzh_lib_avr8_int_twi_cmd_set('s',false);
}

void bzh_lib_avr8_int_twi_cmd_restart()
{
	bzh_lib_avr8_int_twi_cmd_set('r',true);
	bzh_lib_avr8_int_twi_emit();
	_delay_us(BZH_LIB_AVR8_INT_TWI_DELAY_US);
	bzh_lib_avr8_int_twi_cmd_set('r',false);
}

void bzh_lib_avr8_int_twi_cmd_stop()
{
	bzh_lib_avr8_int_twi_cmd_set('p',true);
	bzh_lib_avr8_int_twi_emit();
	//bzh_lib_avr8_int_twi_cmd_set('p',false);
}

void bzh_lib_avr8_int_twi_cmd_ack()
{
	bzh_lib_avr8_int_twi_cmd_set('a',true);
	bzh_lib_avr8_int_twi_emit();
}

void bzh_lib_avr8_int_twi_cmd_nack()
{
	bzh_lib_avr8_int_twi_cmd_set('a',false);
	bzh_lib_avr8_int_twi_emit();
}

#warning bzh_lib_avr8_int_twi_wait_for_int touched PORTA
//wait for int flag
void bzh_lib_avr8_int_twi_wait_for_int()
{
	PORTA |= (1<<PINA7);
	
	while (!(TWCR & (1<<TWINT)))
	{
		asm volatile ("nop");
	}
	
	PORTA &= ~(1<<PINA7);
}

unsigned char bzh_lib_avr8_int_twi_stat_get()
{
	return TWSR & 0xf8;
}

bool bzh_lib_avr8_int_twi_check_start()
{
	if (bzh_lib_avr8_int_twi_stat_get() == TW_START)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool bzh_lib_avr8_int_twi_check_restart()
{
	if (bzh_lib_avr8_int_twi_stat_get() == TW_REP_START)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool bzh_lib_avr8_int_twi_check_stop()
{
	return true;
}

bool bzh_lib_avr8_int_twi_check_gen(unsigned char status_code)
{
	if (bzh_lib_avr8_int_twi_stat_get() == status_code)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void bzh_lib_avr8_int_twi_data_put(unsigned char data)
{
	TWDR = data;
	bzh_lib_avr8_int_twi_emit();
}

unsigned char bzh_lib_avr8_int_twi_data_get()
{
	bzh_lib_avr8_int_twi_emit();
	unsigned char data = TWDR;
	return data;
}

#endif /* BZH_LIB_AVR8_INT_TWI_H_ */