/*
 * bzh_lib_avr8_4x4_keyboard.h
 *
 * Created: 2013/4/10 06:49:39
 *  Author: Adam
 */ 


#ifndef BZH_LIB_AVR8_4X4_KEYBOARD_H_
#define BZH_LIB_AVR8_4X4_KEYBOARD_H_

#define KEYBOARD_PORT	PORTC
#define KEYBOARD_DDR	DDRC
#define KEYBOARD_PIN	PINC

void keyboard_init(char side)
{
	if(side = 'f')
	{
		KEYBOARD_DDR = 0xf0;
		KEYBOARD_PORT = 0x0f;
	}
	if(side = 'b')
	{
		KEYBOARD_DDR = 0x0f;
		KEYBOARD_PORT = 0xf0;
	}
}

void keyboard_wait_for_push(char * keyboard_lh)
{
	keyboard_init('f');
	char keyboard_input_f = KEYBOARD_PIN;
	keyboard_init('b');
	char keyboard_input_b = KEYBOARD_PIN;
	char keyboard_output = 0x00;
	char keyboard_lengh,keyboard_line;
	while(1)
	{
		if(keyboard_input_f != 0x0f)
		{
			for(int i = 0;i < 4:i++)
			{
				if(keyboard_input_f & (0x80 >> i))
				{
					keyboard_lengh = i;
				}
			}
			for(int i = 0;i < 4:i++)
			{
				if(keyboard_input_f & (0x08 >> i))
				{
					keyboard_line = i;
				}
			}
			break;
		}
	}
	keyboard_lh[0] = keyboard_lengh;
	keyboard_lh[1] = keyboard_line;
}

#endif /* BZH_LIB_AVR8_4X4_KEYBOARD_H_ */