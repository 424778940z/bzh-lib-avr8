/*
 * bzh_lib_avr8_nrf24l01p_define.h
 *
 * Created: 2013/3/2 15:28:37
 *  Author: Adam
 */ 


#ifndef BZH_LIB_AVR8_NRF24L01P_DEFINE_H_
#define BZH_LIB_AVR8_NRF24L01P_DEFINE_H_

//基本命令
#define R_REGSITER	  0X00	//读寄存器	直接与地址按位或
#define W_REGSITER	  0X20	//写寄存器	直接与地址按位或
#define R_RX_PAYLOAD  0X61	//读数据 1-32 字节  从0字节开始
#define W_TX_PAYLOAD  0XA0	//写数据 1-32 字节  从0字节开始
#define FLUSH_TX	  0xE1	//清除TX FIFO 寄存器
#define FLUSH_RX      0XE2	//清除RX FIFO 寄存器  传输应答信号时，不应使用此命令
#define RESUSE_TX_PL  0XE3	//重新使用上一包有效数据，当CE为高时，数据包不断重新发射
#define NOP			  0XFF	//空指令 用于取回数据

//寄存器地址
#define REG_CONFIG			0x00	//'Config' 寄存器地址
#define REG_EN_AA			0x01	//'Enable Auto Acknowledgment' 寄存器地址
#define REG_EN_RXADDR		0x02	//'Enabled RX addresses' 寄存器地址
#define REG_SETUP_AW		0x03	//'Setup address width' 寄存器地址
#define REG_SETUP_RETR		0x04	//'Setup Auto. Retrans' 寄存器地址
#define REG_RF_CH			0x05	//'RF channel' 寄存器地址
#define REG_RF_SETUP		0x06	//'RF setup' 寄存器地址
#define REG_STATUS			0x07	//'Status' 寄存器地址
#define REG_OBSERVE_TX		0x08	//'Observe TX' 寄存器地址
#define REG_CD				0x09	//'Carrier Detect' 寄存器地址
#define REG_RX_ADDR_P0		0x0A	//'RX address pipe0' 寄存器地址
#define REG_RX_ADDR_P1		0x0B	//'RX address pipe1' 寄存器地址
#define REG_RX_ADDR_P2		0x0C	//'RX address pipe2' 寄存器地址
#define REG_RX_ADDR_P3		0x0D	//'RX address pipe3' 寄存器地址
#define REG_RX_ADDR_P4		0x0E	//'RX address pipe4' 寄存器地址
#define REG_RX_ADDR_P5		0x0F	//'RX address pipe5' 寄存器地址
#define REG_TX_ADDR			0x10	//'TX address' 寄存器地址
#define REG_RX_PW_P0		0x11	//'RX payload width, pipe0' 寄存器地址
#define REG_RX_PW_P1		0x12	//'RX payload width, pipe1' 寄存器地址
#define REG_RX_PW_P2		0x13	//'RX payload width, pipe2' 寄存器地址
#define REG_RX_PW_P3		0x14	//'RX payload width, pipe3' 寄存器地址
#define REG_RX_PW_P4		0x15	//'RX payload width, pipe4' 寄存器地址
#define REG_RX_PW_P5		0x16	//'RX payload width, pipe5' 寄存器地址
#define REG_FIFO_STATUS		0x17	//'FIFO Status Register' 寄存器地址

//Config' 寄存器
#define MASK_RX_DR	6	//可屏蔽中断RX_RD[1: IRQ引脚不显示RX_RD中断,0: RX_RD中断产生时IRQ引脚电平为低]
#define MASK_TX_DS	5	//可屏蔽中断TX_DS[1: IRQ引脚不显示TX_DS中断,0: TX_DS中断产生时IRQ引脚电平为低]
#define MASK_MAX_RT	4	//可屏蔽中断MAX_RT[1: IRQ引脚不显示MAX_RT中断,0: MAX_RT中断产生时IRQ引脚电平为低]
#define EN_CRC		3	//CRC使能,如果EN_AA中任意一位为高则EN_CRC强迫为高
#define CRCO		2	//CRC模式 [0: 8位CRC校验,1: 16位CRC校验]
#define PWR_UP		1	//电源模式 [1: 上电,0: 掉电]
#define PRIM_RX		0	//工作模式 [1: 接收,0: 发送]

//'Enable Auto Acknowledgment' 寄存器
#define ENAA_P5	5	//数据通道5自动应答开关[默认值:1]
#define ENAA_P4	4	//数据通道4自动应答开关[默认值:1]
#define ENAA_P3	3	//数据通道3自动应答开关[默认值:1]
#define ENAA_P2	2	//数据通道2自动应答开关[默认值:1]
#define ENAA_P1	1	//数据通道1自动应答开关[默认值:1]
#define ENAA_P0	0	//数据通道0自动应答开关[默认值:1]

//'Enabled RX addresses' 寄存器
#define ERX_P5	5	//接收数据通道5开关[默认值:0]
#define ERX_P4	4	//接收数据通道4开关[默认值:0]
#define ERX_P3	3	//接收数据通道3开关[默认值:0]
#define ERX_P2	2	//接收数据通道2开关[默认值:0]
#define ERX_P1	1	//接收数据通道1开关[默认值:1]
#define ERX_P0	0	//接收数据通道0开关[默认值:1]

//'Setup address width' 寄存器
#define AW1 1	//接收/发射地址宽度[00: 无效,01: 3字节宽度,10: 4字节宽度,11: 5字节宽度]
#define AW0 0	//[默认值:11]

//'Setup Auto. Retrans' 寄存器
#define ARD3	7	//ARD为整体[默认值:0000]
#define ARD2	6	//自动重发延时
#define ARD1	5	//'0000' 等待250+86us,'0001' 等待500+86us,'0010' 等待750+86us,'1111' 等待4000+86us
#define ARD0	4	//延时时间是指一包数据发送完成到下一包数据开始发射之间的时间间隔
#define ARC3	3	//ARC为整体[默认值:11]
#define ARC2	2	//自动重发计数
#define ARC1	1	//'0000' 禁止,'0001' 重发一次...'1111' 重发15次
#define ARC0	0	//

//'RF channel' 寄存器
#define RF_CH6	6	//射频通道
#define RF_CH5	5	//设置nRF24L01 工作通道频率
#define RF_CH4	4	//[默认值:0000010]
#define RF_CH3	3	//
#define RF_CH2	2	//
#define RF_CH1	1	//
#define RF_CH0	0	//

//'RF setup' 寄存器
#define PLL_LOCK	4	//PLL_LOCK 允许，仅应用于测试模式[默认值:0]
#define RF_DR		3	//数据传输率 [0: 1Mbps,1: 2Mbps][默认值:1]
#define RF_PWR1		2	//发射功率[默认值:11]
#define RF_PWR0		1	//[00: -18dBm,01: -12dBm,10: -6dBm,11: 0dBm]
#define LNA_HCURR	0	//低噪声放大器增益开关[默认值:1]

//'Status' 寄存器
#define RX_DR				6	//接收数据完成中断,当接收到有效数据后产生中断
#define	TX_DS				5	//数据发送完成中断,当数据发送完成后产生中断.如果工作在自动应答模式下,只有当接收到应答信号后才产生中断
#define MAX_RT				4	//达到最大次重发中断,如果该中断产生则必须清除后系统才能进行通讯
#define RX_P_NO2			3	//接收数据通道号[默认值:111][只读]
#define RX_P_NO1			2	//[000-101: 数据通道号,110: 未使用,111: RX FIFO寄存器为空]
#define RX_P_NO0			1	//
#define STATUS_TX_FULL		0	//TX FIFO寄存器满标志 [1: 已满,0: 未满][默认值:0][只读]

//'Observe TX' 寄存器
#define PLOS_CNT3	7	//数据包丢失计数器
#define PLOS_CNT2	6	//当丢失15个数据包后此寄存器重启
#define PLOS_CNT1	5	//当写RF_CH寄存器时此寄存器复位
#define PLOS_CNT0	4	//[默认值:0][只读]
#define ARC_CNT3	3	//重发计数器
#define ARC_CNT2	2	//发送新数据包时此寄存器复位
#define ARC_CNT1	1	//
#define ARC_CNT0	0	//[默认值:0][只读]

//'Carrier Detect' 寄存器地址
#define CD	0	//载波检测 [默认值:0][只读]

//'FIFO Status Register' 寄存器
#define TX_REUSE	6	//重新使用最后一个数据包,置位后则当CE为高电平状态时不断发送上一数据包,通过指令REUSE_TX_PL置位,通过W_TX_PALOAD或FLUSH_TX复位
#define TX_FULL		5	//TX FIFO寄存器满标志 [1: 已满,0: 未满][默认值:0][只读]
#define TX_EMPTY	4	//TX FIFO寄存器空标志 [1: 已空,0: 未空][默认值:0][只读]
#define RX_FULL		1	//RX FIFO寄存器满标志 [1: 已满,0: 未满][默认值:0][只读]
#define RX_EMPTY	0	//RX FIFO寄存器空标志 [1: 已空,0: 未空][默认值:0][只读]


#define ADR_SIZE	5	//地址大小
#define PLOAD_SIZE	32	//数据缓冲区大小

#endif /* BZH_LIB_AVR8_NRF24L01P_DEFINE_H_ */