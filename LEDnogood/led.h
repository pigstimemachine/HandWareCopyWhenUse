#ifndef __LED_H
#define __LED_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK Mini STM32������
//LED��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2014/3/05
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	   

#define ScanLedSpeeed 50
#define ScanLedTime   1500//��˸ʱ��
typedef struct
{
	long	Cnt;//����
	long	Time;//����ʱ��
	u8      Sts;//������־λ
}LED;
typedef struct
{
	u32 ScanCNT;//����
}SCAN;

#define HEARTBEAT 1
#define STAON     2
#define ScanLED    3

//LED�˿ڶ���
#define LED_PB1_ON      ( GPIOB->ODR|=0x00000002)
#define LED_PB1_OFF     ( GPIOB->ODR&=0xfffffffd)

#define LED_PB0_ON      ( GPIOB->ODR|=0x00000001)
#define LED_PB0_OFF     ( GPIOB->ODR&=0xfffffffe)

#define LED_PA7_ON      ( GPIOA->ODR|=0x00000080)
#define LED_PA7_OFF     ( GPIOA->ODR&=0xffffff7f)
void LED_Init(void);	//��ʼ��		
long TwinkleSpeed(u8 LedSelect,long speed);
void HeartbeatLed(void);
void STA_ON_Led(void);
void Twinkle3(void);
void ScanLed(void);//ɨ��ɹ�����˸;
void _LED_TASK1MS(void);
#endif


















