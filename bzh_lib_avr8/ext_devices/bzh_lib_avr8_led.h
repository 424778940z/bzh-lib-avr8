/*
 * bzh_lib_avr8_led.h
 *
 * Created: 2011/12/30 10:14:48
 *  Author: Adam 铭
 */ 


#ifndef BZH_LIB_AVR8_LED_H_
#define BZH_LIB_AVR8_LED_H_

#ifndef BZH_LIB_AVR8_DELAY_H_
# error "not define BZH_LIB_AVR8_DELAY_H_ ; BZH_LIB_AVR8_LED_H_ can not work"
#endif

/*
#define LED_DDR_CONF DDRB
#define LED_PORT_CONF PORTB
*/

#define LED_DDR_CONF DDRC
#define LED_PORT_CONF PORTC

#ifndef LED_DDR_CONF
# error "not define LED_DDR_CONF ; BZH_LIB_AVR8_LED_H_ can not work,must define DDR and PORT"
#endif

#ifndef LED_PORT_CONF
# error "not define LED_PORT_CONF ; BZH_LIB_AVR8_LED_H_ can not work,must define DDR and PORT"
#endif

static unsigned char led_config[] = {0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};
static unsigned char led_status = 0x00;

//初始化
void lib_led_init()
{
	LED_DDR_CONF = 0xff;
	LED_PORT_CONF = 0x00;
	//led_status = 0x00;
}

//端口置位
void lib_led_apply(unsigned char led_status)
{
	LED_PORT_CONF = led_status;
}

//调试用,不明,忘记了
void lib_led_ststus_display(unsigned long int delay)
{
	lib_led_init();
	unsigned int i;
	for (i = 0;i < 8;i++)
	{
		LED_PORT_CONF = led_config[i];
		delay_reduce(delay);
	}
}

//单独控制
//灯号,1开,0关
int lib_led_bit_control(unsigned int bit,unsigned int control)
{
	if (bit > 7||bit < 0||control > 1||control < 0)
	{
		return -1;
	}
	
	if (control == 0)
	{
		led_status = led_status&led_config[bit];
	}
	else if (control == 1)
	{
		led_status = led_status|~led_config[bit];
	}
	lib_led_apply(led_status);
	return 0;
}

//单独闪烁
//灯号,闪烁次数,延迟
void lib_led_bit_flash(unsigned int bit,int time,unsigned long int delay)
{
	for (;time >= 0;time--)
	{
		lib_led_bit_control(bit,1);
		delay_reduce(delay);
		lib_led_bit_control(bit,0);
		delay_reduce(delay);
	}
}

#endif /* BZH_LIB_AVR8_LED_H_ */