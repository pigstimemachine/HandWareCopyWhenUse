 
#include "CH451.h"
#include "Define.h"
#include "sys.h"
/*------------------------------------------------------------
函数名称: void ch451_write(unsigned short int command)
函数功能: 向显示芯片CH451传递数据
参    数: command 发送的数据
返 回 值: 无
-------------------------------------------------------------*/
void ch451_write(unsigned int command)
{
  unsigned char i=0;
  LOAD_L ;
  for(i=0;i<12;i++)
  {
    if(!(command&0x01))
    {
       DAT_L;
    }
    else
    {
      DAT_H;
    }
    DCLK_L;
    command>>=1;
    DCLK_H;
  }
    LOAD_H;
}

/*-------------------------------------------------------------
函数名称: void ch451_init(void)
函数功能: CH451初始化程序
参    数: 无
返 回 值: 无
-------------------------------------------------------------*/
  void ch451_init(void)
{
  DAT_L;
  DAT_H;
  LOAD_H;
  DCLK_H;
  ch451_write(0x0201);//芯片内部复位
//  ch451_write(0x0202);//进入睡眠状态
  ch451_write(0x0401);//关看门狗，开显示扫描
  ch451_write(0x0500);//不译码，扫描极限8，辉度8/16
  ch451_write(0x0600);//不闪烁显示
}
/*-------------------------------------------------------------
函数名称: DISP_V(unsigned int dis_num,unsigned char point)
函数功能: 参数调节数码管显示
参    数: dis_num显示的数据值,十位有小数点
返 回 值: 无
-------------------------------------------------------------*/
void DISP(unsigned int dis_num,unsigned char point)
{
  unsigned char code[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x00};//0123456789
  unsigned int ge=0,shi=0,bai=0,qian=0;
  ge=dis_num%10;//取余
  shi=(dis_num/10)%10;
  bai=(dis_num/100)%10;
  qian=dis_num/1000;
  if(qian==0)
  {
    if(bai==0)
    {
      if(shi==0)
      {
        if(!point)
        {
          shi=10;
        }
      }
      bai=10;
    }
    qian=10;
  }

    ch451_write(0x0800|code[qian]);//千位
    ch451_write(0x0900|code[bai]);//百位
    if(point==0)
    {
      ch451_write(0x0a00|code[shi]);//十位
    }
    else if(point==1)
    {
      ch451_write(0x0a00|code[shi]|0x80);//十位带小数点
    }
    else if(point==2)
    {
         ch451_write(0x0a00|code[shi]|0x80);
    }
    ch451_write(0x0b00|code[ge]);  //个位
}


