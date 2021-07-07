#include "timer.h" 
//#include "usart.h" 
#include "TaskMgr.h"
void TIM2_Int_Init(u16 arr,u16 psc)
{	
	RCC->APB1ENR|=1<<0;	//TIM2时钟使能    
 	TIM2->ARR=arr;  	//设定计数器自动重装值//刚好1ms    
	TIM2->PSC=psc;  	//预分频器7200,得到10Khz的计数时钟		  
	TIM2->DIER|=1<<0;   //允许更新中断	  
	TIM2->CR1|=0x01;    //使能定时器3
  MY_NVIC_Init(3,3,TIM2_IRQn,2);//抢占1，子优先级3，组2TIM2_IRQChannel		  
}

void TIM2_IRQHandler(void)
{ 	
	if(TIM2->SR&0X01)//是更新中断
	{	 		   
		Timer0_IsrHandler();

        //Twinkle3Time(50);
		TIM2->SR&=~(1<<0);		//清除中断标志位		   
	}	    
}
