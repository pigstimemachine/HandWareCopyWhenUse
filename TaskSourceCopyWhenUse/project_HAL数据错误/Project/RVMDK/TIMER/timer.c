#include "timer.h" 
//#include "usart.h" 
#include "TaskMgr.h"
void TIM2_Int_Init(u16 arr,u16 psc)
{	
	RCC->APB1ENR|=1<<0;	//TIM2ʱ��ʹ��    
 	TIM2->ARR=arr;  	//�趨�������Զ���װֵ//�պ�1ms    
	TIM2->PSC=psc;  	//Ԥ��Ƶ��7200,�õ�10Khz�ļ���ʱ��		  
	TIM2->DIER|=1<<0;   //��������ж�	  
	TIM2->CR1|=0x01;    //ʹ�ܶ�ʱ��3
  MY_NVIC_Init(3,3,TIM2_IRQn,2);//��ռ1�������ȼ�3����2TIM2_IRQChannel		  
}

void TIM2_IRQHandler(void)
{ 	
	if(TIM2->SR&0X01)//�Ǹ����ж�
	{	 		   
		Timer0_IsrHandler();

        //Twinkle3Time(50);
		TIM2->SR&=~(1<<0);		//����жϱ�־λ		   
	}	    
}
