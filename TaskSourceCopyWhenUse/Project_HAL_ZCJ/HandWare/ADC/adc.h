#ifndef __ADC_H
#define __ADC_H	
#include "sys.h"

#define ADC1_DR_Address    ((u32)0x4001244C)		
#define proportion  (float)(3.3/4096)
#define ADC_CH1  1  //通道1	
#define ADC_CH0  0  //通道0	 
#define ADC_CH7 7

#define ADC_VOLT 9
#define ADC_CURR 8

void Adc_Init(void); 				                   //ADC通道初始化
u16 Get_Adc(u8 ch); 				                   //获得某个通道值 
//float Get_Adc_Average(u16 times);
void DMA_WITH_ADC_Init(void);
void Start_DMA_Adc(u8 ch);
u16 Get_Adc_Average(u8 ch,u8 times);
//u16 filter_1(u16 NEW_DATA,u16 OLD_DATA,u16 k);
#endif 















