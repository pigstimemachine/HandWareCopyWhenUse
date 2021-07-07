#include "lib.h"


int main(void)
{			  	
    Stm32_Clock_Init(9);	//系统时钟设置
	delay_init(72);
    uart_init(72,9600);	
    TIM2_Int_Init(9,7199);
	SysTask_Create();
	while(1)
	{
	  Timer1_IsrHandler();
	}	
}




