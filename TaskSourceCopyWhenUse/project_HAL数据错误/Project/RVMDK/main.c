#include "delay.h"
#include "sys.h"

#include "string.h"
#include "stdio.h"

#include <string.h>
#include <stdlib.h>
#include "stm32f10x.h"


//#include "timer.c"

int main(void)
{
    Stm32_Clock_Init(9);	//系统时钟设置
    delay_init();
    TIM2_Int_Init(9,7199);
	SysTask_Create();	
	while(1) 
    {		
        Timer1_IsrHandler();	
    }	
}



 		



