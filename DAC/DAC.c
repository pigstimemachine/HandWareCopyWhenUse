#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "stdlib.h"
#include "Define.h"
#include "DAC.h"

/*-----------------------------------------------------------------
��������: void Disp_DAC(unsigned char chan,unsigned int dat) 
��������: DAC104S085 D/Aת������
��    ��: unsigned char chan,unsigned int dat
�� �� ֵ: ��
-----------------------------------------------------------------*/ 
//void Disp_DAC(unsigned int dat) 
//{ 
//  unsigned int ads,i; 
//	if(dat>1023)
//  dat=1023;
//  CS_L; 
//  CS_H;
//	delay_us(1);
//  CS_L;
//  ads=(chan<<14)|0x1000|(dat<<2);
//  for(i=16;i>=1;i--)   
//  { 
//    CLK_H;
//	  if(ads&0x8000) 
//	  {
//	    DIN_H;
//	  }
//    else  
//	  {
//      DIN_L;
//    }
//	  CLK_L;
//	  ads=ads<<1;          //����һλ
//  }    
//	DIN_L;//
//} 

/*-----------------------------------------------------------------
��������: void DAC_Init(void)
��������: D/A��ʼ��
��    ��: 
�� �� ֵ: ��
-----------------------------------------------------------------*/
void DAC_Init(void)
{
    RCC->APB2ENR|=1<<2;//ʹ��portAʱ��    
	RCC->APB1ENR|=1<<29;   //ʹ��DACʱ��
    GPIOA->CRL&=0XFFF0FFFF;//pa4ģ������
    GPIOA->CRL|=0X00000000;
	//DAC->CR|=0x00030003;	   
    DAC->CR|=1<<0;//ʹ��dac1
    DAC->CR|=1<<1;//ʹ��dac1������治ʹ��boff1=1
    DAC->CR|=0<<2;
    DAC->CR|=0<<3;
    DAC->CR|=0<<6;
    DAC->CR|=0<<8;
    DAC->CR|=0<<12;
    
	DAC->DHR12R1=0;
}

void Dac1_Set_Vol(unsigned  int vol)
{
    unsigned int temp=0;
    temp=3650-vol*180/10;
//    temp=3401-vol*17;
//    temp=250;//���
   // temp=temp/4095*25/10;
    DAC->DHR12R1=temp;
}
/*-----------------------------------------------------------------
��������: void DAC_Set_cur(unsigned long num)												 
��������: D/A
��    ��: num
�� �� ֵ: ��
//����ͨ��1�����ѹ

-----------------------------------------------------------------*/
//void DAC_Set_cur(unsigned int num)
//{ 
//	  if(num>max_speed)
//		{
//			num=max_speed;	
//		}
//		else if(num<min_speed)
//		{
//			num=min_speed;
//		}
//        num*=4096/3.3;
//		num*=5;
//		DAC->DHR12R1=num;
//        
//}
