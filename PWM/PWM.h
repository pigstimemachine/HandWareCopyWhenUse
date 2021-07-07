#ifndef __PWM_H__
#define __PWM_H__

#include "sys.h"


#define arr_init 7199     //ÆµÂÊ=72000 000/£¨ £¨7199+1£©*£¨0+1£© £©£¬Ã¿Ãë10000´Î
#define psc_init 0
typedef enum
{
	PWM1_CH1,//TIM1_CH1,
	PWM1_CH2,//TIM1_CH2,
	PWM1_CH3,//TIM1_CH3,
	PWM1_CH4,//TIM1_CH4,
	
	PWM2_CH1,//TIM2_CH1,
	PWM2_CH2,//TIM2_CH2,
	PWM2_CH3,//TIM2_CH3,
	PWM2_CH4,//TIM2_CH4,
	
	PWM3_CH1,//TIM3_CH1,
	PWM3_CH2,//TIM3_CH2,
	PWM3_CH3,//TIM3_CH3,
	PWM3_CH4,//TIM3_CH4,
	
	PWM4_CH1,//TIM4_CH1,
	PWM4_CH2,//TIM4_CH2,
	PWM4_CH3,//TIM4_CH3,
	PWM4_CH4,//TIM4_CH4,
	
//	PWM5_CH1,//TIM5_CH1,
//	PWM5_CH2,//TIM5_CH2,
//	PWM5_CH3,//TIM5_CH3,
//	PWM5_CH4,//TIM5_CH4,
}PWM_Ch;

void PWM_Init(PWM_Ch n,u16 PWM_VAL);
void PWM_duty(PWM_Ch n,u16 PWM_DUTY);



#endif
