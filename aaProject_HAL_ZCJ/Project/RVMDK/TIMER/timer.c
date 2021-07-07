#include "timer.h" 
//#include "usart.h" 
#include "TaskMgr.h"
//#include "lib.h"
//#include "PWM.h"
//#include "UAPP_Comm.h"
//extern LED0PWM led0pwm;
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

/*
void TIM4_Clear(u8 cnt)
{
  TIM4->CNT=cnt;
}
//����TIM4�Ŀ���
//sta:0���ر�;1,����;
void TIM4_Enable(u8 sta)
{
	if(sta)
	{
		TIM4->CR1|=1<<0;     //ʹ�ܶ�ʱ��4
    TIM4->CNT=0;         //���������
	}else TIM4->CR1&=~(1<<0);//�رն�ʱ��4	   
}

void TIM4_IRQHandler(void)
{ 	
	if(TIM4->SR&0X01)//�Ǹ����ж�
	{	 		   
//		USART1_FrameEND();	//��ǽ������
		TIM4->SR&=~(1<<0);		//����жϱ�־λ		   
		TIM4_Enable(0);			//�ر�TIM4 
	}	    
}

//ͨ�ö�ʱ���жϳ�ʼ��
//����ʼ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��		 
void TIM4_Init(u16 arr,u16 psc)
{
	RCC->APB1ENR|=1<<2;	//TIM4ʱ��ʹ��    
 	TIM4->ARR=arr;  	//�趨�������Զ���װֵ   
	TIM4->PSC=psc;  	//Ԥ��Ƶ��
 	TIM4->DIER|=1<<0;   //��������ж�				
 	TIM4->CR1|=0x00;  	//ʹ�ܶ�ʱ��4	  	   
  MY_NVIC_Init(1,3,TIM4_IRQn,2);//��ռ2�������ȼ�3����2	��2�����ȼ����		TIM4_IRQChannel						 
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













//TIM1_CH1 PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
void TIM1_PWM_Init(u16 arr,u16 psc)
{		 					 
	//�˲������ֶ��޸�IO������
    RCC->APB2ENR|=1<<2;    //ʹ��PORTAʱ��
	RCC->APB2ENR|=1<<11; 	//TIM1ʱ��ʹ��    
	GPIOA->CRH&=0XFFFFFFF0;	//PA8���֮ǰ������
	GPIOA->CRH|=0X0000000B;	//���ù������ 
	
	TIM1->ARR=arr;			//�趨�������Զ���װֵ 
	TIM1->PSC=psc;			//Ԥ��Ƶ������
  
	TIM1->CCMR1|=7<<4;  	//CH1 PWM2ģʽ		 
	TIM1->CCMR1|=1<<3; 		//CH1Ԥװ��ʹ��	 
 	TIM1->CCER|=1<<0;   	//OC1 ���ʹ��	   
	TIM1->BDTR|=1<<15;   	//MOE �����ʹ��	   

	TIM1->CR1=0x0080;   	//ARPEʹ�� 
	TIM1->CR1|=0x01;    	//ʹ�ܶ�ʱ��1 										  
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
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);//TIM_OC3 �����̨�3
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
	
	TIM_Cmd(TIM3, ENABLE);
	
   */ 
/*
https://blog.csdn.net/zhuhao19990902/article/details/100808869
��������
TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStrue;
        
        GPIO_InitTypeDef  GPIO_InitStructure;
        
        TIM_OCInitTypeDef TIM_OCInitStruct;

        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);        
        GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);
        
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;                                 //pb53&#245;��&#188;&#187;��
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                  //&#184;�䨮&#195;��&#198;������&#228;3&#246;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                 //IO&#191;��&#203;��&#182;��&#206;a50MHz
  GPIO_Init(GPIOB, &GPIO_InitStructure);
        
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;                                 //pb43&#245;��&#188;&#187;��
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                  //&#184;�䨮&#195;��&#198;������&#228;3&#246;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                 //IO&#191;��&#203;��&#182;��&#206;a50MHz
  GPIO_Init(GPIOB, &GPIO_InitStructure);
        
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;                                 
  GPIO_Init(GPIOB, &GPIO_InitStructure);
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;                                 
  GPIO_Init(GPIOB, &GPIO_InitStructure);
        
        GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3,ENABLE);//&#198;&#244;&#182;��2&#191;��&#214;&#214;&#216;��3��&#228;TIM3CH1-PB4   TIM3CH2-PB5
        ////////////////////////////////////////////////////
        TIM_TimeBaseInitStrue.TIM_ClockDivision = TIM_CKD_DIV1;
        TIM_TimeBaseInitStrue.TIM_CounterMode = TIM_CounterMode_Up; //&#188;&#198;��y&#198;��&#207;����&#207;&#188;&#198;��y
        TIM_TimeBaseInitStrue.TIM_Period = arr;
        TIM_TimeBaseInitStrue.TIM_Prescaler = psc;
        TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStrue);
        
        ///////////////////////////////////////////////////////////////
        TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM2;
        TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_Low;
        TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
        TIM_OC2Init(TIM3,&TIM_OCInitStruct);//&#188;&#198;����&#198;��3�����̨�23&#245;��&#188;&#187;��
        
        TIM_OC1Init(TIM3,&TIM_OCInitStruct);//&#188;&#198;����&#198;��3�����̨�13&#245;��&#188;&#187;��
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
//��������������������������������
//��Ȩ����������ΪCSDN������Mr_Johhny����ԭ�����£���ѭ CC 4.0 BY-SA ��ȨЭ�飬ת���븽��ԭ�ĳ������Ӽ���������
//ԭ�����ӣ�https://blog.csdn.net/lnniyunlong99/java/article/details/103214188
//�������ļ�����������ʵPB0�˿ڵ�PWM�����ͨ�� func_set_pwm_compare(70); ������ռ�ձȡ�

//��Ҫע����� ʹ�õĶ˿ڣ��Ƕ�Ӧ�ĸ���ʱ�����Ǹ�ͨ���ġ�
//����?TIM_OC3Init(TIM3, &TIM_OCInitStructure);//TIM_OC3�����ã�������PB0ʹ�ö�ʱ��3ͨ��3��

//void TIM4_PWM_Init(u16 arr,u16 psc)
//{  
//	GPIO_InitTypeDef GPIO_InitStructure;
//	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//	TIM_OCInitTypeDef  TIM_OCInitStructure;
//	
//	
//	/* PWM�źŵ�ƽ����ֵ */
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
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);  //ʹ��GPIO����ʱ��ʹ��
//	
//																
//																
//	//���ø�����Ϊ�����������,���TIM1 CH1��PWM���岨��		   //PB6,7,8,9���PWM
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9; //TIM_CH1~TIM_CH4
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);


//	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 80K
//	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  ����Ƶ
//	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//0; //����ʱ�ӷָ�:TDTS = Tck_tim
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
//	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

//	/* PWM1 Mode configuration: Channel1 */
//	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
//	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
//	TIM_OCInitStructure.TIM_Pulse = CCR1_Val; //���ô�װ�벶��ȽϼĴ���������ֵ
//	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
//	TIM_OC1Init(TIM4, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx

//	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);  //CH1Ԥװ��ʹ��


//	/* PWM1 Mode configuration: Channel2 */
//	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
//	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
//	TIM_OCInitStructure.TIM_Pulse = CCR2_Val; //���ô�װ�벶��ȽϼĴ���������ֵ
//	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
//	TIM_OC2Init(TIM4, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx

//	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);  //CH1Ԥװ��ʹ��
//	
//	/* PWM1 Mode configuration: Channel3 */
//	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
//	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
//	TIM_OCInitStructure.TIM_Pulse = CCR3_Val; //���ô�װ�벶��ȽϼĴ���������ֵ
//	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
//	TIM_OC3Init(TIM4, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx

//	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);  //CH3Ԥװ��ʹ��	
//	
//		/* PWM1 Mode configuration: Channel4 */
//	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
//	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
//	TIM_OCInitStructure.TIM_Pulse = CCR4_Val; //���ô�װ�벶��ȽϼĴ���������ֵ
//	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
//	TIM_OC4Init(TIM4, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
//	
//	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);  //CH4Ԥװ��ʹ��	 
//	


////	TIM_CtrlPWMOutputs(TIM4,ENABLE);	//MOE �����ʹ��	,�����ڸ߼���ʱ��
//	TIM_ARRPreloadConfig(TIM4, ENABLE); //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���
//	TIM_Cmd(TIM4, ENABLE);  //ʹ��TIM1
//}

