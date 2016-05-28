/*
 * bzh_lib_avr8_lcd_1602.h
 *
 * Created: 2011/12/30 10:03:38
 *  Author: Adam 铭
 */ 

#ifndef BZH_LIB_AVR8_LCD_1602_H_
#define BZH_LIB_AVR8_LCD_1602_H_

#include <string.h>//不引用则提示strlen隐式声明

#ifndef BZH_LIB_AVR8_DELAY_H_
# error "Not define LIB_DELAY_H_ ; LIB_LCD_1602_H_ can not work"
#endif

#define F_CPU_DELAY 1

//引脚定义
#define LCD_1602_CONFIG_DDR DDRA
#define LCD_1602_DATA_DDR DDRK
#define LCD_1602_CONFIG_PORT PORTA
#define LCD_1602_DATA_PORT PORTK
#define LCD_1602_STATUS_PIN PINK
#define LCD_1602_RS PA2
#define LCD_1602_RW PA1
#define LCD_1602_EN PA0

/*
 * This library must define :
 * LCD_1602_CONFIG_DDR
 * LCD_1602_DATA_DDR
 * LCD_1602_CONFIG_PORT
 * LCD_1602_DATA_PORT
 * LCD_1602_STATUS_PIN
 * LCD_1602_RS
 * LCD_1602_RW
 * LCD_1602_EN
 */

#ifndef LCD_1602_CONFIG_DDR
# error "Not define LCD_1602_CONFIG_DDR"
#endif

#ifndef LCD_1602_DATA_DDR
# error "Not define LCD_1602_DATA_DDR"
#endif

#ifndef LCD_1602_CONFIG_PORT
# error "Not define LCD_1602_CONFIG_PORT"
#endif

#ifndef LCD_1602_DATA_PORT
# error "Not define LCD_1602_DATA_PORT"
#endif

#ifndef LCD_1602_STATUS_PIN
# error "Not define LCD_1602_STATUS_PIN"
#endif

#ifndef LCD_1602_RS
# error "Not define LCD_1602_RS"
#endif

#ifndef LCD_1602_RW
# error "Not define LCD_1602_RW"
#endif

#ifndef LCD_1602_EN
# error "Not define LCD_1602_EN"
#endif

void lcd_1602_busy_check(void);
void lcd_1602_send_cmd(unsigned char cmd);
void lcd_1602_init();
void lcd_1602_write_data(unsigned char data);
unsigned char lcd_1602_read_data(void);
void lcd_1602_write_CGRAM(unsigned int	num, const unsigned int	*pBuffer);
void lcd_1602_write_string(unsigned char row,unsigned char col,unsigned int num,unsigned char *pBuffer);
void lcd_1602_write_string_unsafe(unsigned char row,unsigned char col,char *pBuffer);

//忙显状态查询
void lcd_1602_busy_check(void)
{
	//声明循环标记位
	unsigned char busy_loop_bit = 1;
	delay_reduce(F_CPU_DELAY*500);
	LCD_1602_CONFIG_PORT &= ~(1 << LCD_1602_RS);    //LCD_1602_RS=0
	delay_reduce(F_CPU_DELAY*500);
	LCD_1602_CONFIG_PORT |= (1 << LCD_1602_RW);     //LCD_1602_RW=1
	delay_reduce(F_CPU_DELAY*500);
	while(busy_loop_bit)
	{
		LCD_1602_CONFIG_PORT |= (1 << LCD_1602_EN); //LCD_1602_EN=1
		delay_reduce(F_CPU_DELAY*500);
		LCD_1602_DATA_DDR = 0x00;      //LCD_1602_DATA_PORT输入
		LCD_1602_DATA_PORT = 0xff;     //LCD_1602_DATA_PORT上拉使能
		delay_reduce(F_CPU_DELAY*500);
		busy_loop_bit = LCD_1602_STATUS_PIN&0x80;    //读取LCD_1602_DATA_PORT
		delay_reduce(F_CPU_DELAY*500);
		LCD_1602_DATA_DDR = 0xff;
		LCD_1602_DATA_PORT = 0xff;        //LCD_1602_DATA_PORT变输出
		delay_reduce(F_CPU_DELAY*500);
		LCD_1602_CONFIG_PORT &= ~(1 << LCD_1602_EN);   //LCD_1602_EN=0
		delay_reduce(F_CPU_DELAY*500);
	}
}

//发送指令
void lcd_1602_send_cmd(unsigned char cmd)
{
	lcd_1602_busy_check();
	delay_reduce(F_CPU_DELAY*500);
	LCD_1602_CONFIG_PORT &= ~(1 << LCD_1602_RS);   //LCD_1602_RS=0
	delay_reduce(F_CPU_DELAY*500);
	LCD_1602_CONFIG_PORT &= ~(1 << LCD_1602_RW);   //LCD_1602_RW=0
	delay_reduce(F_CPU_DELAY*500);
	LCD_1602_CONFIG_PORT |= (1 << LCD_1602_EN);    //LCD_1602_EN=1
	delay_reduce(F_CPU_DELAY*500);
	LCD_1602_DATA_PORT = cmd;       //输出指令
	delay_reduce(F_CPU_DELAY*500);
	LCD_1602_CONFIG_PORT &= ~(1 << LCD_1602_EN);   //LCD_1602_EN=0
	delay_reduce(F_CPU_DELAY*500);
}

//1602 LCD初始化
void lcd_1602_init()
{
	LCD_1602_CONFIG_DDR = 0xff;
	LCD_1602_DATA_DDR = 0xff;
	LCD_1602_CONFIG_PORT = 0x00;
	LCD_1602_DATA_PORT = 0x00;
	delay_reduce(F_CPU_DELAY*1000);
	lcd_1602_send_cmd(0x38);
	delay_reduce(F_CPU_DELAY*1000);
	lcd_1602_send_cmd(0x08);//关闭显示
	delay_reduce(F_CPU_DELAY*1000);
	lcd_1602_send_cmd(0x01);//显示清屏
	delay_reduce(F_CPU_DELAY*7000);
	lcd_1602_send_cmd(0x06);//光标移动设置
	delay_reduce(F_CPU_DELAY*1000);
	lcd_1602_send_cmd(0x0c);//显示开及光标设置
	delay_reduce(F_CPU_DELAY*1000);
	lcd_1602_send_cmd(0x38);
	delay_reduce(F_CPU_DELAY*1000);
}
/*
void lcd_1602_init()
{
	PORTA = ~0x04;
	LCD_1602_CONFIG_DDR = 0xff;
	LCD_1602_DATA_DDR = 0xff;
	LCD_1602_CONFIG_PORT = 0x00;
	LCD_1602_DATA_PORT = 0x00;
	delay_reduce(F_CPU_DELAY*1000);
	lcd_1602_send_cmd(0x38);
	delay_reduce(F_CPU_DELAY*1000);
	lcd_1602_send_cmd(0x01);
	delay_reduce(F_CPU_DELAY*7000);
	lcd_1602_send_cmd(0x02);
	delay_reduce(F_CPU_DELAY*1000);
	lcd_1602_send_cmd(0x06);
	delay_reduce(F_CPU_DELAY*1000);
	lcd_1602_send_cmd(0x0c);
	delay_reduce(F_CPU_DELAY*1000);
	lcd_1602_send_cmd(0x38);
	delay_reduce(F_CPU_DELAY*1000);
	PORTA = ~0x00;
}
*/

//写数据
void lcd_1602_write_data(unsigned char data)
{
	lcd_1602_busy_check();
	delay_reduce(F_CPU_DELAY*500);
	LCD_1602_CONFIG_PORT |= (1 << LCD_1602_RS);   //LCD_1602_RS=1
	delay_reduce(F_CPU_DELAY*500);
	LCD_1602_CONFIG_PORT &= ~(1 << LCD_1602_RW);   //LCD_1602_RW=0
	delay_reduce(F_CPU_DELAY*500);
	LCD_1602_CONFIG_PORT |= (1 << LCD_1602_EN);    //LCD_1602_EN=1
	delay_reduce(F_CPU_DELAY*500);
	LCD_1602_DATA_PORT = data;      //输出数据
	delay_reduce(F_CPU_DELAY*500);
	LCD_1602_CONFIG_PORT &= ~(1 << LCD_1602_EN);   //LCD_1602_EN=0
	delay_reduce(F_CPU_DELAY*500);
}

//读数据
unsigned char lcd_1602_read_data(void)
{
	unsigned char data_temp;
	lcd_1602_busy_check();
	delay_reduce(F_CPU_DELAY*500);
	LCD_1602_CONFIG_PORT |= (1 << LCD_1602_RS);  //LCD_1602_RS=1
	delay_reduce(F_CPU_DELAY*500);
	LCD_1602_CONFIG_PORT |= (1 << LCD_1602_RW);  //LCD_1602_RW=1
	delay_reduce(F_CPU_DELAY*500);
	LCD_1602_CONFIG_PORT |= (1 << LCD_1602_EN);  //LCD_1602_EN=1
	delay_reduce(F_CPU_DELAY*500);
	LCD_1602_DATA_DDR = 0x00;       //B端口变输入
	delay_reduce(F_CPU_DELAY*500);
	data_temp = LCD_1602_STATUS_PIN;     //读B端口
	delay_reduce(F_CPU_DELAY*500);
	LCD_1602_DATA_DDR = 0xff;       //B端口变输出
	delay_reduce(F_CPU_DELAY*500);
	LCD_1602_CONFIG_PORT &= ~(1 << LCD_1602_EN); //LCD_1602_EN=0
	delay_reduce(F_CPU_DELAY*350);
	return data_temp;	
}

/*
//=================================================
// 描述： 写LCD内部CGRAM函数
// 入口： ‘num’要写的数据个数
//        ‘pbuffer’要写的数据的首地址
// 出口： 无
//================================================
*/
void lcd_1602_write_CGRAM(unsigned int	num, const unsigned int	*pBuffer)
{
	unsigned int i,t;
	lcd_1602_send_cmd(0x40);
	LCD_1602_CONFIG_PORT |= (1 << LCD_1602_RS);
	LCD_1602_CONFIG_PORT &= ~(1 << LCD_1602_RW);
	for(i = num;i != 0;i--)
	{
		t = *pBuffer;
		LCD_1602_CONFIG_PORT |= (1 << LCD_1602_EN);
		LCD_1602_DATA_PORT = t;
		LCD_1602_CONFIG_PORT &= ~(1 << LCD_1602_EN);				
		pBuffer++;
	}
}

/*
//================================================================
// 描述：在任意位置写任意多个字符
// 入口：’row‘要写的字符所在的行，只能为1或2；
//       ‘col’要写的字符所在的列，只能为0---15
//       ‘num’要写字符的个数
//       ‘pbuffer’要写字符的首地址
//==================================================================
*/
void lcd_1602_write_string(unsigned char row,unsigned char col,unsigned int num,unsigned char *pBuffer)
{
	unsigned char i,t;
	if (row == 1)
	{
		row = 0x80 + col;
	}
	else if(row == 2)
	{
		row = 0xC0 + col;
	}
	lcd_1602_send_cmd(row);
	//if (num<=0 | num>9)	num =0x30;
	//else	num = 0x30 + num;

	LCD_1602_CONFIG_PORT |= (1 << LCD_1602_RS);
	delay_reduce(F_CPU_DELAY*500);
	LCD_1602_CONFIG_PORT &= ~(1 << LCD_1602_RW);
	delay_reduce(F_CPU_DELAY*500);
	for(i = num;i != 0;i--)
	{
		t = *pBuffer;
		delay_reduce(F_CPU_DELAY*500);
		LCD_1602_DATA_PORT = t;
		delay_reduce(F_CPU_DELAY*500);
		LCD_1602_CONFIG_PORT |= (1 << LCD_1602_EN);
		delay_reduce(F_CPU_DELAY*500);
		LCD_1602_CONFIG_PORT &= ~(1 << LCD_1602_EN);		
		delay_reduce(F_CPU_DELAY*500);	
		pBuffer++;
	}
}

/*
================================================================
// 描述：在任意位置写任意多个字符
// 入口：’row‘要写的字符所在的行，只能为1或2；
//       ‘col’要写的字符所在的列，只能为0---15
//       ‘pbuffer’要写字符的首地址
================================================================
*/
void lcd_1602_write_string_unsafe(unsigned char row,unsigned char col,char *pBuffer)
{
	unsigned char i,t;
	if (row == 1)
	{
		row = 0x80 + col;
	}
	else if(row == 2)
	{
		row = 0xC0 + col;
	}
	
	lcd_1602_send_cmd(row);
	//if (num<=0 | num>9)	num =0x30;
	//else	num = 0x30 + num;

	LCD_1602_CONFIG_PORT |= (1 << LCD_1602_RS);
	delay_reduce(F_CPU_DELAY*500);
	LCD_1602_CONFIG_PORT &= ~(1 << LCD_1602_RW);
	delay_reduce(F_CPU_DELAY*500);
	for(i = strlen(pBuffer);i != 0;i--)
	{
		t = *pBuffer;
		delay_reduce(F_CPU_DELAY*500);
		LCD_1602_DATA_PORT = t;
		delay_reduce(F_CPU_DELAY*500);
		LCD_1602_CONFIG_PORT |= (1 << LCD_1602_EN);
		delay_reduce(F_CPU_DELAY*500);
		LCD_1602_CONFIG_PORT &= ~(1 << LCD_1602_EN);		
		delay_reduce(F_CPU_DELAY*500);	
		pBuffer++;
	}
}
#endif /* BZH_LIB_AVR8_LCD_1602_H_ */