/*
 * bzh_lib_avr8_speaker.h
 *
 * Created: 2011/12/31 21:42:59
 *  Author: Adam 铭
 */ 


#ifndef BZH_LIB_AVR8_SPEAKER_H_
#define BZH_LIB_AVR8_SPEAKER_H_

#ifndef BZH_LIB_AVR8_DELAY_H_
# error "not define LIB_DELAY_H_ ; LIB_SPEAK_H_ can not work"
#endif

#define SPEAK_DDR_CONF DDRL
#define SPEAK_PORT_CONF PORTL

void lib_speak_init()
{
	SPEAK_DDR_CONF = 0xff;
	SPEAK_PORT_CONF = 0xff;
	SPEAK_PORT_CONF = 0x00;
}

void lib_speak_do(unsigned long int delay_sound,unsigned long int delay_empty,unsigned int time)
{
	while(time--)
	{
		SPEAK_PORT_CONF |= 0x80;	//端口=1
		delay_reduce(delay_sound);	//延时
		SPEAK_PORT_CONF &= ~0x80;	//端口=0
		delay_reduce(delay_empty);	//延时
	}	
}


#endif /* BZH_LIB_AVR8_SPEAKER_H_ */