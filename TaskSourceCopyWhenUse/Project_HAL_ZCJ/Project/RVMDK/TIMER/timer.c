#include "timer.h" 
//#include "usart.h" 
#include "TaskMgr.h"
//#include "lib.h"
//#include "PWM.h"
//#include "UAPP_Comm.h"
//extern LED0PWM led0pwm;
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

/*
void TIM4_Clear(u8 cnt)
{
  TIM4->CNT=cnt;
}
//设置TIM4的开关
//sta:0，关闭;1,开启;
void TIM4_Enable(u8 sta)
{
	if(sta)
	{
		TIM4->CR1|=1<<0;     //使能定时器4
    TIM4->CNT=0;         //计数器清空
	}else TIM4->CR1&=~(1<<0);//关闭定时器4	   
}

void TIM4_IRQHandler(void)
{ 	
	if(TIM4->SR&0X01)//是更新中断
	{	 		   
//		USART1_FrameEND();	//标记接收完成
		TIM4->SR&=~(1<<0);		//清除中断标志位		   
		TIM4_Enable(0);			//关闭TIM4 
	}	    
}

//通用定时器中断初始化
//这里始终选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数		 
void TIM4_Init(u16 arr,u16 psc)
{
	RCC->APB1ENR|=1<<2;	//TIM4时钟使能    
 	TIM4->ARR=arr;  	//设定计数器自动重装值   
	TIM4->PSC=psc;  	//预分频器
 	TIM4->DIER|=1<<0;   //允许更新中断				
 	TIM4->CR1|=0x00;  	//使能定时器4	  	   
  MY_NVIC_Init(1,3,TIM4_IRQn,2);//抢占2，子优先级3，组2	在2中优先级最低		TIM4_IRQChannel						 
}
*/

/*
 void Isr_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure; 	
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
  NVIC_InitStructure.NVIC_IRQChannel =TIM4_IRQn;// TIM4_IRQChannel; 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 
  NVIC_Init (&NVIC_InitStructure); 

  NVIC_InitStructure.NVIC_IRQChannel =TIM3_IRQn;// TIM4_IRQChannel; 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; 
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
  NVIC_Init (&NVIC_InitStructure);
	
}
*/













//TIM1_CH1 PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
void TIM1_PWM_Init(u16 arr,u16 psc)
{		 					 
	//此部分需手动修改IO口设置
    RCC->APB2ENR|=1<<2;    //使能PORTA时钟
	RCC->APB2ENR|=1<<11; 	//TIM1时钟使能    
	GPIOA->CRH&=0XFFFFFFF0;	//PA8清除之前的设置
	GPIOA->CRH|=0X0000000B;	//复用功能输出 
	
	TIM1->ARR=arr;			//设定计数器自动重装值 
	TIM1->PSC=psc;			//预分频器设置
  
	TIM1->CCMR1|=7<<4;  	//CH1 PWM2模式		 
	TIM1->CCMR1|=1<<3; 		//CH1预装载使能	 
 	TIM1->CCER|=1<<0;   	//OC1 输出使能	   
	TIM1->BDTR|=1<<15;   	//MOE 主输出使能	   

	TIM1->CR1=0x0080;   	//ARPE使能 
	TIM1->CR1|=0x01;    	//使能定时器1 										  
}  





/*
void init_hardware_pwm(uint16_t arr, uint16_t psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;//PB0 -> TIM3
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB  | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	//GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
	
	TIM_TimeBaseStructure.TIM_Period = arr;
	TIM_TimeBaseStructure.TIM_Prescaler = psc;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);	
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;	
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);//TIM_OC3 í¨μà3
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
	
	TIM_Cmd(TIM3, ENABLE);
	
   */ 
/*
https://blog.csdn.net/zhuhao19990902/article/details/100808869
配置如下
TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStrue;
        
        GPIO_InitTypeDef  GPIO_InitStructure;
        
        TIM_OCInitTypeDef TIM_OCInitStruct;

        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);        
        GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);
        
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;                                 //pb53&#245;ê&#188;&#187;ˉ
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                  //&#184;′ó&#195;í&#198;íìê&#228;3&#246;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                 //IO&#191;ú&#203;ù&#182;è&#206;a50MHz
  GPIO_Init(GPIOB, &GPIO_InitStructure);
        
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;                                 //pb43&#245;ê&#188;&#187;ˉ
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                  //&#184;′ó&#195;í&#198;íìê&#228;3&#246;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                 //IO&#191;ú&#203;ù&#182;è&#206;a50MHz
  GPIO_Init(GPIOB, &GPIO_InitStructure);
        
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;                                 
  GPIO_Init(GPIOB, &GPIO_InitStructure);
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;                                 
  GPIO_Init(GPIOB, &GPIO_InitStructure);
        
        GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3,ENABLE);//&#198;&#244;&#182;ˉ2&#191;·&#214;&#214;&#216;ó3é&#228;TIM3CH1-PB4   TIM3CH2-PB5
        ////////////////////////////////////////////////////
        TIM_TimeBaseInitStrue.TIM_ClockDivision = TIM_CKD_DIV1;
        TIM_TimeBaseInitStrue.TIM_CounterMode = TIM_CounterMode_Up; //&#188;&#198;êy&#198;÷&#207;òé&#207;&#188;&#198;êy
        TIM_TimeBaseInitStrue.TIM_Period = arr;
        TIM_TimeBaseInitStrue.TIM_Prescaler = psc;
        TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStrue);
        
        ///////////////////////////////////////////////////////////////
        TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM2;
        TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_Low;
        TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
        TIM_OC2Init(TIM3,&TIM_OCInitStruct);//&#188;&#198;ê±&#198;÷3í¨μà23&#245;ê&#188;&#187;ˉ
        
        TIM_OC1Init(TIM3,&TIM_OCInitStruct);//&#188;&#198;ê±&#198;÷3í¨μà13&#245;ê&#188;&#187;ˉ
        TIM_OC3Init(TIM3,&TIM_OCInitStruct);
        TIM_OC4Init(TIM3,&TIM_OCInitStruct);
        

//
        TIM_OC2PreloadConfig(TIM3,TIM_OCPreload_Enable);
        TIM_OC1PreloadConfig(TIM3,TIM_OCPreload_Enable);
        TIM_OC3PreloadConfig(TIM3,TIM_OCPreload_Enable);
        TIM_OC4PreloadConfig(TIM3,TIM_OCPreload_Enable);
        
        TIM_Cmd(TIM3,ENABLE);
   
}
*/ 
/*
void func_pwm_enable(void)
{
	TIM_Cmd(TIM3, ENABLE);
}
 
void func_pwm_disable(void)
{
	TIM_Cmd(TIM3, DISABLE);
}
 
void func_set_pwm_compare(uint8_t rate)//, uint16_t psc
{
	long tmp = 60000;
	if(rate == 0)
	{
		func_pwm_disable();
		return;
	}
	if(rate > 100)
	{
		return;
	}
	func_pwm_enable();
	tmp = tmp * rate / 100;
	TIM_SetCompare3(TIM3, tmp);
}
*/
//————————————————
//版权声明：本文为CSDN博主「Mr_Johhny」的原创文章，遵循 CC 4.0 BY-SA 版权协议，转载请附上原文出处链接及本声明。
//原文链接：https://blog.csdn.net/lnniyunlong99/java/article/details/103214188
//照上述文件，即可以现实PB0端口的PWM输出，通过 func_set_pwm_compare(70); 来设置占空比。

//需要注意的是 使用的端口，是对应哪个定时器的那个通道的。
//例如?TIM_OC3Init(TIM3, &TIM_OCInitStructure);//TIM_OC3的设置，设置了PB0使用定时器3通道3。

//void TIM4_PWM_Init(u16 arr,u16 psc)
//{  
//	GPIO_InitTypeDef GPIO_InitStructure;
//	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//	TIM_OCInitTypeDef  TIM_OCInitStructure;
//	
//	
//	/* PWM信号电平跳变值 */
// 	u16 CCR1_Val = 4999;        
// 	u16 CCR2_Val = 4999;
// 	u16 CCR3_Val = 4999;
// 	u16 CCR4_Val = 4999;
///* ----------------------------------------------------------------------- 
//    TIM1 Channel1 duty cycle = (TIM1_CCR1/ TIM3_ARR+1)* 100% = 50%
//    TIM1 Channel2 duty cycle = (TIM1_CCR2/ TIM3_ARR+1)* 100% = 37.5%
//    TIM1 Channel3 duty cycle = (TIM1_CCR3/ TIM3_ARR+1)* 100% = 25%
//    TIM1 Channel4 duty cycle = (TIM1_CCR4/ TIM3_ARR+1)* 100% = 12.5%
//  ----------------------------------------------------------------------- */
//  
////	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART1, DISABLE);
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);// 
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);  //使能GPIO外设时钟使能
//	
//																
//																
//	//设置该引脚为复用输出功能,输出TIM1 CH1的PWM脉冲波形		   //PB6,7,8,9输出PWM
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9; //TIM_CH1~TIM_CH4
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);


//	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 80K
//	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  不分频
//	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//0; //设置时钟分割:TDTS = Tck_tim
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
//	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

//	/* PWM1 Mode configuration: Channel1 */
//	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
//	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
//	TIM_OCInitStructure.TIM_Pulse = CCR1_Val; //设置待装入捕获比较寄存器的脉冲值
//	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
//	TIM_OC1Init(TIM4, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx

//	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);  //CH1预装载使能


//	/* PWM1 Mode configuration: Channel2 */
//	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
//	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
//	TIM_OCInitStructure.TIM_Pulse = CCR2_Val; //设置待装入捕获比较寄存器的脉冲值
//	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
//	TIM_OC2Init(TIM4, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx

//	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);  //CH1预装载使能
//	
//	/* PWM1 Mode configuration: Channel3 */
//	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
//	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
//	TIM_OCInitStructure.TIM_Pulse = CCR3_Val; //设置待装入捕获比较寄存器的脉冲值
//	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
//	TIM_OC3Init(TIM4, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx

//	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);  //CH3预装载使能	
//	
//		/* PWM1 Mode configuration: Channel4 */
//	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
//	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
//	TIM_OCInitStructure.TIM_Pulse = CCR4_Val; //设置待装入捕获比较寄存器的脉冲值
//	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
//	TIM_OC4Init(TIM4, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
//	
//	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);  //CH4预装载使能	 
//	


////	TIM_CtrlPWMOutputs(TIM4,ENABLE);	//MOE 主输出使能	,适用于高级定时器
//	TIM_ARRPreloadConfig(TIM4, ENABLE); //使能TIMx在ARR上的预装载寄存器
//	TIM_Cmd(TIM4, ENABLE);  //使能TIM1
//}

