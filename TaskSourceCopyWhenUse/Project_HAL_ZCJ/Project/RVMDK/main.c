#include "delay.h"
#include "sys.h"
#include "string.h"
#include "stdio.h"
#include <stdlib.h>
#include "stm32f10x.h"
#include "lib.h"

int main(void)
{		
    Stm32_Clock_Init(9);	//ϵͳʱ������
    delay_init();
    TIM2_Int_Init(9,7199);//(9,7199)
	delay_init();	     //��ʱ��ʼ��
	SysTask_Create();
	while(1) 
    {		
        Timer1_IsrHandler();	
    }	
}



 		



