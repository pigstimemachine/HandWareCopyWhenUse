#include "lib.h"
#include "adc.h"
void  Adc_Init(void) 
{     
   //�ȳ�ʼ��IO�� 
   RCC->APB2ENR|=1<<3;    //ʹ��PORTB��ʱ�� 
   GPIOB->CRL&=0XFFFFFF00;//PB0,1 anolog���� 
   //ͨ��10/11����          
   RCC->APB2ENR|=1<<9;    //ADC1ʱ��ʹ��      
   RCC->APB2RSTR|=1<<9;   //ADC1��λ 
   RCC->APB2RSTR&=~(1<<9);//��λ����        
   RCC->CFGR&=~(3<<14);   //��Ƶ��������    
   //SYSCLK/DIV2=12M ADCʱ������Ϊ12M,ADC���ʱ�Ӳ��ܳ���14M! 
   //���򽫵���ADC׼ȷ���½�! 
   RCC->CFGR|=2<<14;          
   ADC1->CR1&=0XF0FFFF;   //����ģʽ���� 
   ADC1->CR1|=0<<16;      //��������ģʽ   
   ADC1->CR1&=~(1<<8);    //��ɨ��ģʽ      
   ADC1->CR2&=~(1<<1);    //����ת��ģʽ 
   ADC1->CR2&=~(7<<17);      
   ADC1->CR2|=7<<17;      //��������ת��   
   ADC1->CR2|=1<<20;      //ʹ�����ⲿ����(SWSTART)!!!   ����ʹ��һ���¼������� 
   ADC1->CR2&=~(1<<11);   //�Ҷ���    
   ADC1->SQR1&=(0XF<<20); 
   ADC1->SQR1|=0<<20;     //1��ת���ڹ���������            
   //����ͨ��1�Ĳ���ʱ�� 
   ADC1->SMPR2&=~(7<<24);  //ͨ��8����ʱ�����      
   ADC1->SMPR2|=7<<24;     //ͨ��8  239.5����,��߲���ʱ�������߾�ȷ��  
   ADC1->SMPR2&=~(7<<27);  //ͨ��9����ʱ�����      
   ADC1->SMPR2|=7<<27;     //ͨ��9  239.5����,��߲���ʱ�������߾�ȷ��    	 
   ADC1->CR2|=1<<0;      //����ADת����    
   ADC1->CR2|=1<<3;       //ʹ�ܸ�λУ׼   
   while(ADC1->CR2&1<<3); //�ȴ�У׼����           
   //��λ���������ò���Ӳ���������У׼�Ĵ�������ʼ�����λ���������        
   ADC1->CR2|=1<<2;        //����ADУ׼      
   while(ADC1->CR2&1<<2);  //�ȴ�У׼���� 
   //��λ�����������Կ�ʼУ׼������У׼����ʱ��Ӳ�����   
}               
//���ADCֵ 
//ch:ͨ��ֵ 0~16 
//����ֵ:ת����� 
u16 Get_Adc(u8 ch)   
{ 
   //����ת������ 
   u16 ADCX;         
   ADC1->SQR3&=0XFFFFFFE0;//��������1 ͨ��ch 
   ADC1->SQR3|=ch;                      
   ADC1->CR2|=1<<22;       //��������ת��ͨ�� 
   while(!(ADC1->SR&1<<1));//�ȴ�ת������          
   ADCX=ADC1->DR;          
   return ADCX;        //����adcֵ 
} 
//��ȡͨ��ch��ת��ֵ��ȡtimes��,Ȼ��ƽ�� 
//ch:ͨ����� 
//times:��ȡ���� 
//����ֵ:ͨ��ch��times��ת�����ƽ��ֵ 
u16 Get_Adc_Average(u8 ch,u8 times) 
{ 
   u32 temp_val=0; 
   u8 t; 
   for(t=0;t<times;t++) 
   { 
      temp_val+=Get_Adc(ch);
   } 
   return (temp_val/times);
} 

//u16 filter_1(u16 NEW_DATA,u16 OLD_DATA,u16 k)   //k        ????(0~255)(???????????)
//{
//    long result;
//    if(NEW_DATA<OLD_DATA)
//    {
//        result=OLD_DATA-NEW_DATA;
//        result=result*k;
//        result=result+128;//+128???????
//        result=result>>8;
//        result=OLD_DATA-result;
//    }
//    else if(NEW_DATA>OLD_DATA)
//    {
//        result=NEW_DATA-OLD_DATA;
//        result=result*k;
//        result=result+128;//+128???????
//        result=result>>8;
//        result=OLD_DATA+result;
//    }
//    else result=OLD_DATA;
//    return((u16)result);
//}