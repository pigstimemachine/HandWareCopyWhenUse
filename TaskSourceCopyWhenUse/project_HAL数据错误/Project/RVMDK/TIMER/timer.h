#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"


 //通过改变TIM1->CCR1的值来改变占空比，从而控制LED0的亮度
#define LED0_PWM_VAL TIM1->CCR1     

void TIM3_Int_Init(u16 arr,u16 psc);
void TIM2_Int_Init(u16 arr,u16 psc);
void TIM4_Int_Init(u16 arr,u16 psc);
void TIM6_Int_Init(u16 arr,u16 psc);
void TIM7_Int_Init(u16 arr,u16 psc);
void TIM4_Clear(u8 cnt);
void TIM4_Enable(u8 sta);
void TIM4_Init(u16 arr,u16 psc);
unsigned char TimerIsOverflowEvent(void);
void TimeCount(void);
void Isr_Init(void);
 

void TIM1_PWM_Init(u16 arr,u16 psc);



 
void init_hardware_pwm(u16 arr, u16 psc);
void func_pwm_enable(void);
void func_pwm_disable(void);

void TIM4_PWM_Init(u16 arr,u16 psc);




#endif























