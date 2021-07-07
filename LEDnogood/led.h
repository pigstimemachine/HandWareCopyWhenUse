#ifndef __LED_H
#define __LED_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK Mini STM32开发板
//LED驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2014/3/05
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	   

#define ScanLedSpeeed 50
#define ScanLedTime   1500//闪烁时间
typedef struct
{
	long	Cnt;//计数
	long	Time;//运行时长
	u8      Sts;//点亮标志位
}LED;
typedef struct
{
	u32 ScanCNT;//计数
}SCAN;

#define HEARTBEAT 1
#define STAON     2
#define ScanLED    3

//LED端口定义
#define LED_PB1_ON      ( GPIOB->ODR|=0x00000002)
#define LED_PB1_OFF     ( GPIOB->ODR&=0xfffffffd)

#define LED_PB0_ON      ( GPIOB->ODR|=0x00000001)
#define LED_PB0_OFF     ( GPIOB->ODR&=0xfffffffe)

#define LED_PA7_ON      ( GPIOA->ODR|=0x00000080)
#define LED_PA7_OFF     ( GPIOA->ODR&=0xffffff7f)
void LED_Init(void);	//初始化		
long TwinkleSpeed(u8 LedSelect,long speed);
void HeartbeatLed(void);
void STA_ON_Led(void);
void Twinkle3(void);
void ScanLed(void);//扫描成功则闪烁;
void _LED_TASK1MS(void);
#endif


















