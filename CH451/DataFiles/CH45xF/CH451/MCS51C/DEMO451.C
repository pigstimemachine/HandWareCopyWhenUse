//CH451
//AT89C51 C语言程序
//Web: http//winchiphead.com
//Author: yhw  2003.7

//功能  开机后在最低位置1，其余清零；按下键后，左移两位，在低两位显示
//十进制键值，如果按住键不放，最低位小数点点亮，释放后小数点熄灭。

#include <reg51.h>

//如果使用键盘中断请定义
#define  USE_KEY  1

//须主程序定义的参数
 sbit   ch451_dclk=P1^7;                          //串行数据时钟上升延激活
 sbit   ch451_din=P1^6;		                // 串行数据输出，接CH451的数据输入
 sbit   ch451_load=P1^5;                        //串行命令加载，上升延激活
 sbit   ch451_dout=P3^3;                        //INT1，键盘中断和键值数据输入，接CH451的数据输出
 uchar  ch451_key;		                // 存放键盘中断中读取的键值

#include <CH451C54.C>

//*************************************************
 //延时程序
 void delay(){					//在12P下延时十毫秒
   uchar i,j,k;
   for(i=0;i<10;i++)
     for(j=0;j<40;j++)
       for(k=0;k<120;k++);
  }

//************************************************
void main(){
   uchar a,b,c,d;
   d=0;
   ch451_init();                                  //调用初始化
   ch451_write(0x403);
   ch451_write(0x580);                           //测试送入命令
   ch451_write(CH451_DIG0|0X01);                 //最低位显示1
   EA=1;
   ch451_key=0x0ff;
   while(1){
    while(ch451_key==0xff);	                //等待中断
    a=ch451_key;
    d=a;
    ch451_key=0x0ff;
    ch451_write(0x300);		              //左移两位
    ch451_write(0x300);
    a&=0x3f;			             //显示十进制数据
    b=a;
    a/=10;
    ch451_write(CH451_DIG1|a);
    b%=10;
    ch451_write(CH451_DIG0|b);
     delay();
     c=ch451_read();
     while(c==d)		           //测试按键是否释放
     {
      ch451_write(CH451_DIG0|b|0x80);
      delay();
      c=ch451_read();
     }
    ch451_write(CH451_DIG0|b);
 }
}
