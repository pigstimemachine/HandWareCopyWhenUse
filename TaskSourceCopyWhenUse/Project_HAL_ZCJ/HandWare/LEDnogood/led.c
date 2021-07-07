#include "sys.h"   
#include "lib.h"
//////////////////////////////////////////////////////////////////////////////////	 

LED HeartBeatLed={0,0,0},STA_ON_LED={0,0,0},Twinkle3Time={0,0,0};
//LED IO初始化
SCAN Scan;
void LED_Init(void)
{
	  RCC->APB2ENR|=1<<2;    //使能PORTA时钟	   	 
	  RCC->APB2ENR|=1<<3;    //使能PORTD时钟	   
    
    GPIOB->CRL&=0XFFFFFF00;//系统呼吸灯PB0 PB1 PA7
    GPIOB->CRL|=0X00000033;
    GPIOA->CRL&=0X0FFFFFFF;//系统呼吸灯PB0 PB1 PA7
    GPIOA->CRL|=0X30000000;    
    
    TwinkleSpeed(1,0);	
    TwinkleSpeed(2,0);
    TwinkleSpeed(3,0);    
}

long TwinkleSpeed(u8 LedSelect,long SPEED)
{
    switch(LedSelect)
    {
        case HEARTBEAT:
            HeartBeatLed.Time=SPEED;
            return  HeartBeatLed.Time;
          //break;  
        case STAON:
            STA_ON_LED.Time=SPEED;
            return STA_ON_LED.Time;
          //break;  
        case 3:
            Twinkle3Time.Time=SPEED;
            return Twinkle3Time.Time;            
          //break;          
    }
    return 0;
}


void HeartbeatLed(void)
{
    HeartBeatLed.Cnt++;
    HeartBeatLed.Sts=1;
    if(HeartBeatLed.Cnt>=HeartBeatLed.Time)
    {
        HeartBeatLed.Sts=0; 
        HeartBeatLed.Cnt=0;
        HeartBeatLed.Time=0;
    }
    if(HeartBeatLed.Sts)
    {
        LED_PB0_ON;
    }
    else
    {
        LED_PB0_OFF;
    }
    if(HeartBeatLed.Time==0)
    {
        LED_PB0_OFF;
    }
}
void STA_ON_Led(void)
{
    STA_ON_LED.Cnt++;
    if(STA_ON_LED.Cnt>=STA_ON_LED.Time)
    {
        STA_ON_LED.Sts^=1; 
        STA_ON_LED.Cnt=0;
    }
    if(STA_ON_LED.Sts)
    {
        LED_PB1_ON;
    }
    else
    {
        LED_PB1_OFF;
    } 
    if(STA_ON_LED.Time==0)
    {
        LED_PB1_OFF;
    }
}
void Twinkle3(void)
{
    Twinkle3Time.Cnt++;
    if(Twinkle3Time.Cnt>=Twinkle3Time.Time)
    {
        Twinkle3Time.Sts^=1; 
        Twinkle3Time.Cnt=0;
    }
    if(Twinkle3Time.Sts)
    {
        LED_PA7_ON;
    }
    else
    {
        LED_PA7_OFF;
    } 
    if(Twinkle3Time.Time==0)    
    {
        LED_PA7_OFF;
    }
}

void ScanLed(void)//扫描成功则闪烁
{
    Scan.ScanCNT++;
    //TwinkleSpeed(ScanLED,0);
    if(!PBin(7))
    {
        TwinkleSpeed(ScanLED,ScanLedSpeeed); 
    }
    if(Scan.ScanCNT>ScanLedTime)
    {
        TwinkleSpeed(ScanLED,0); 
        Scan.ScanCNT=0;
    }   
}

void STA_LED(void)//上线灯
{    
    if(_READ_OnlineSta()==STA_ON)
    {
       TwinkleSpeed(STAON,1000); 
    }
    else
    {
       TwinkleSpeed(STAON,100);
    }
}

void _LED_TASK1MS(void)
{
    HeartbeatLed();
    STA_ON_Led();
    Twinkle3();
    ScanLed();
    STA_LED();
}	



