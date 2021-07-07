// CH451 
// AT89C51 C语言程序
//Web:http//winchiphead.com
//Author: yhw  2003.7

#define CH451_RESET     0x0201                           //复位
#define CH451_LEFTMOV   0x0300		            //设置移动方式-作移
#define CH451_LEFTCYC   0x0301		            //设置移动方式-左循
#define CH451_RIGHTMOV  0x0302		            //设置移动方式-右移
#define CH451_RIGHTCYC  0x0303		            //设置移动方式-右循	
#define CH451_SYSOFF    0x0400                  //关显示、键盘、看门狗
#define CH451_SYSON1    0x0401                  //开显示
#define CH451_SYSON2    0x0403                  //开显示、键盘
#define CH451_SYSON3    0x0407                  //开显示、键盘、看门狗功能
#define CH451_DSP       0x0500                  //设置默认显示方式
#define CH451_BCD       0x0580                  //设置BCD译码方式
#define CH451_TWINKLE   0x0600		            //设置闪烁控制
#define CH451_DIG0      0x0800                  //数码管位0显示
#define CH451_DIG1      0x0900		            //数码管位1显示 
#define CH451_DIG2      0x0a00		            //数码管位2显示
#define CH451_DIG3      0x0b00		            //数码管位3显示
#define CH451_DIG4      0x0c00		            //数码管位4显示
#define CH451_DIG5      0x0d00	                //数码管位5显示	
#define CH451_DIG6      0x0e00	                //数码管位6显示	
#define CH451_DIG7      0x0f00		            //数码管位7显示

//须主程序定义的参数
// sbit ch451_dclk=P1^7;                            //串行数据时钟上升延激活
// sbit ch451_din=P1^6;		                    // 串行数据输出，接CH451的数据输入
// sbit ch451_load=P1^5;                            //串行命令加载，上升延激活
// sbit ch451_dout=P3^3;        	                     //INT1，键盘中断和键值数据输入，接CH451的数据输出
// uchar  ch451_key;		                    // 存放键盘中断中读取的键值
//********************************************

//初始化子程序

void ch451_init()
{
  ch451_din=0;                                     //先低后高，选择4线输入
  ch451_din=1;
#ifdef USE_KEY
  IT1=0;			                  //设置下降沿触发
  IE1=0;			                  //清中断标志
  PX1=0;			                  //设置低优先级
  EX1=1;			                  //开中断
#endif

}
//*****************************************************

 //输出命令子程序
 //定义一无符号整型变量存储12字节的命令字。
 void ch451_write(unsigned int command)
{
  unsigned char i;
#ifdef USE_KEY
  EX1=0;			                 //禁止键盘中断
#endif	
  ch451_load=0;                                  //命令开始       
  for(i=0;i<12;i++)
  {                             //送入12位数据，低位在前
    ch451_din=command&1;				
    ch451_dclk=0;
    command>>=1;
    ch451_dclk=1;                               //上升沿有效
  }
  ch451_load=1;                                 //加载数据
#ifdef USE_KEY
  EX1=1;
#endif
}

#ifdef USE_KEY

//*************************************************
//输入命令子程序，MCU从451读一字节
 unsigned char ch451_read()
{
  unsigned char i;
  unsigned char command,keycode;		//定义命令字，和数据存储器
  EX1=0;			               //关中段	
  command=0x07;			               //输入读451命令字	
  ch451_load=0;
  for(i=0;i<4;i++){
 
    ch451_din=command&1;		      //送入最低位
    ch451_dclk=0;			 
    command>>=1;			      //往右移一位
    ch451_dclk=1;			      //产生时钟上升沿锁通知CH451输入位数据
 }
  ch451_load=1;				      //产生加载上升沿通知CH451处理命令数据
  keycode=0;				      //清除keycode
  for(i=0;i<7;i++){
    keycode<<=1;			      //数据移入keycode,高位在前,低位在后
    keycode|=ch451_dout;		      //从高到低读入451的数据
    ch451_dclk=0;			      //产生时钟下升沿通知CH451输出下一位
    ch451_dclk=1;
 }
  IE1=0;				      //清中断标志
  EX1=1;
  return(keycode);			     //反回键值
}
//*************************************************
//中断子程序  使用中断2，寄存器组1

void ch451_inter() interrupt 2 using 1
{
  unsigned char i;			      //定义循环变量
  unsigned char command,keycode;	      //定义控制字寄存器，和中间变量定时器
  command=0x07;				      //读取键值命令的高4位0111B
  ch451_load=0;				      //命令开始
  for(i=0;i<4;i++){
    ch451_din=command&1;		     //低位在前,高位在后
    ch451_dclk=0;
    command>>=1;			     //右移一位
    ch451_dclk=1;			     //产生时钟上升沿锁通知CH451输入位数据
 }
  ch451_load=1;				     //产生加载上升沿通知CH451处理命令数据
  keycode=0;				     //清除keycode
  for(i=0;i<7;i++){
    keycode<<=1;			     //数据作移一位,高位在前,低位在后
    keycode|=ch451_dout;		     //从高到低读入451的数据
    ch451_dclk=0;			     //产生时钟下升沿通知CH451输出下一位
    ch451_dclk=1;
 }
  ch451_key=keycode;			    //保存上次的键值
  IE1=0;				    //清中断标志
}

//***********************************************

#endif 

