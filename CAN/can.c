#include "lib.h"

CANArrayBufType CANTxType,CANRxType;


static void InitArrayBuf(void);


u8 CAN1_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode)
{
  	GPIO_InitTypeDef GPIO_InitStructure; 
	  CAN_InitTypeDef        CAN_InitStructure;
   	NVIC_InitTypeDef  NVIC_InitStructure;
    //使能相关时钟
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能PORTA时钟	                   											 

  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);//使能CAN1时钟	
	
    //初始化GPIO
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11| GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化PA11,PA12
	
	  //引脚复用映射配置
	  GPIO_PinAFConfig(GPIOA,GPIO_PinSource11,GPIO_AF_CAN1); //GPIOA11复用为CAN1
	  GPIO_PinAFConfig(GPIOA,GPIO_PinSource12,GPIO_AF_CAN1); //GPIOA12复用为CAN1
	  
  	//CAN单元设置
   	CAN_InitStructure.CAN_TTCM=DISABLE;	//非时间触发通信模式   
  	CAN_InitStructure.CAN_ABOM=DISABLE;	//软件自动离线管理	  
  	CAN_InitStructure.CAN_AWUM=DISABLE;//睡眠模式通过软件唤醒(清除CAN->MCR的SLEEP位)
  	CAN_InitStructure.CAN_NART=ENABLE;	//禁止报文自动传送 
  	CAN_InitStructure.CAN_RFLM=DISABLE;	//报文不锁定,新的覆盖旧的  
  	CAN_InitStructure.CAN_TXFP=DISABLE;	//优先级由报文标识符决定 
  	CAN_InitStructure.CAN_Mode= mode;	 //模式设置 
  	CAN_InitStructure.CAN_SJW=tsjw;	//重新同步跳跃宽度(Tsjw)为tsjw+1个时间单位 CAN_SJW_1tq~CAN_SJW_4tq
  	CAN_InitStructure.CAN_BS1=tbs1; //Tbs1范围CAN_BS1_1tq ~CAN_BS1_16tq
  	CAN_InitStructure.CAN_BS2=tbs2;//Tbs2范围CAN_BS2_1tq ~	CAN_BS2_8tq
  	CAN_InitStructure.CAN_Prescaler=brp;  //分频系数(Fdiv)为brp+1	
  	CAN_Init(CAN1, &CAN_InitStructure);   // 初始化CAN1 
    
	
	  CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);//FIFO0消息挂号中断允许.		    
  
  	NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;     // 主优先级为1
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;            // 次优先级为0
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);
	return 0;
}   


void SetRxMbox(u16 STDMSGID,u32 EXTMSGID,u16 STDMSGIDLAM,u32 EXTMSGIDLAM,u8 mailbox)
{
	  CAN_FilterInitTypeDef  CAN_FilterInitStructure;
    		//配置过滤器
 	  CAN_FilterInitStructure.CAN_FilterNumber=mailbox;	  //过滤器0
  	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask; 
  	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit; //32位 
  	CAN_FilterInitStructure.CAN_FilterIdHigh=EXTMSGID;////32位ID
  	CAN_FilterInitStructure.CAN_FilterIdLow=STDMSGID;
  	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=EXTMSGIDLAM;//32位MASK
  	CAN_FilterInitStructure.CAN_FilterMaskIdLow=STDMSGIDLAM;
   	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;//过滤器0关联到FIFO0
  	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE; //激活过滤器0
  	CAN_FilterInit(&CAN_FilterInitStructure);//滤波器初始化
}

static void InitArrayBuf(void)
{
	CANTxType.ArrayBufInPtr=&CANTxType.ArrayBuf[0];
	CANTxType.ArrayBufOutPtr=&CANTxType.ArrayBuf[0];
	CANTxType.ArrayBufCtr=0;

	CANRxType.ArrayBufInPtr=&CANRxType.ArrayBuf[0];
	CANRxType.ArrayBufOutPtr=&CANRxType.ArrayBuf[0];
	CANRxType.ArrayBufCtr=0;
}

static u16 ArrayGetBufCtr(CANArrayBufType *pbuf)
{
	return(pbuf->ArrayBufCtr);       
}
/***********************************************************************************
*                              数据写入队列缓冲区
***********************************************************************************/

static u8 ArrayPutFrame(CANArrayBufType *Arraybuf,CANBufType *Frame)
{   
	 if(Arraybuf->ArrayBufCtr<CanArrayBufSize)
	 {
			 Arraybuf->ArrayBufCtr++;
			 memcpy(Arraybuf->ArrayBufInPtr->FrameData,Frame->FrameData,CAN_FRAME_WIDTH);
			 Arraybuf->ArrayBufInPtr->DLC=Frame->DLC;
			 Arraybuf->ArrayBufInPtr->MSGID=Frame->MSGID;
			 Arraybuf->ArrayBufInPtr++;
			 if(Arraybuf->ArrayBufInPtr==&Arraybuf->ArrayBuf[CanArrayBufSize])
					 Arraybuf->ArrayBufInPtr=&Arraybuf->ArrayBuf[0]; 
			 return ABUF_NO_ERR;
			 
	 }
	 return ABUF_TXBUFFULL_ERR;
}
            
    
/***********************************************************************************
*                              取出队列缓冲区数据
***********************************************************************************/
static u8 ArrayGetFrame(CANArrayBufType *Arraybuf,CANBufType *Frame)
{             
   if(ArrayGetBufCtr(Arraybuf)<1)   
   {
       return ABUF_RXBUFLESS_ERR;                 
   }
	 Arraybuf->ArrayBufCtr--;
	 memcpy(Frame->FrameData,Arraybuf->ArrayBufOutPtr->FrameData,CAN_FRAME_WIDTH);
	 Frame->DLC=Arraybuf->ArrayBufOutPtr->DLC;
	 Frame->MSGID=Arraybuf->ArrayBufOutPtr->MSGID;
	 Arraybuf->ArrayBufOutPtr++;
	 if(Arraybuf->ArrayBufOutPtr==&Arraybuf->ArrayBuf[CanArrayBufSize])
			Arraybuf->ArrayBufOutPtr=&Arraybuf->ArrayBuf[0];   
   return ABUF_NO_ERR;
}

u8 Can_Tx_Msg(CANBufType buffer)
{	   
	u8 mbox;	  
	if(CAN1->TSR&(1<<26))mbox=0;			//邮箱0为空
	else if(CAN1->TSR&(1<<27))mbox=1;	//邮箱1为空
	else if(CAN1->TSR&(1<<28))mbox=2;	//邮箱2为空
	else return(0);
	CAN1->sTxMailBox[mbox].TIR=0;		//清除之前的设置  
	CAN1->sTxMailBox[mbox].TIR|=(((unsigned long)buffer.MSGID)<<21);		 
	CAN1->sTxMailBox[mbox].TIR|=0<<2;	  
	CAN1->sTxMailBox[mbox].TIR|=0<<1;
	CAN1->sTxMailBox[mbox].TDTR&=~(0X0000000F);
	CAN1->sTxMailBox[mbox].TDTR|=buffer.DLC;		   //设置DLC.
	//待发送数据存入邮箱.
	CAN1->sTxMailBox[mbox].TDHR=(((u32)buffer.FrameData[7]<<24)|
								((u32)buffer.FrameData[6]<<16)|
 								((u32)buffer.FrameData[5]<<8)|
								((u32)buffer.FrameData[4]));
	CAN1->sTxMailBox[mbox].TDLR=(((u32)buffer.FrameData[3]<<24)|
								((u32)buffer.FrameData[2]<<16)|
 								((u32)buffer.FrameData[1]<<8)|
								((u32)buffer.FrameData[0]));
	CAN1->sTxMailBox[mbox].TIR|=1<<0; //请求发送邮箱数据
	return 1;
}

void Can_Rx_Msg(u8 fifox,CANBufType* buffer)
{	    
	buffer->DLC=CAN1->sFIFOMailBox[fifox].RDTR&0x0F;//得到DLC
	buffer->MSGID =CAN1->sFIFOMailBox[fifox].RIR>>21;  //得到ID
	buffer->FrameData[0]=CAN1->sFIFOMailBox[fifox].RDLR&0XFF;
	buffer->FrameData[1]=(CAN1->sFIFOMailBox[fifox].RDLR>>8)&0XFF;
	buffer->FrameData[2]=(CAN1->sFIFOMailBox[fifox].RDLR>>16)&0XFF;
	buffer->FrameData[3]=(CAN1->sFIFOMailBox[fifox].RDLR>>24)&0XFF;    
	buffer->FrameData[4]=CAN1->sFIFOMailBox[fifox].RDHR&0XFF;
	buffer->FrameData[5]=(CAN1->sFIFOMailBox[fifox].RDHR>>8)&0XFF;
	buffer->FrameData[6]=(CAN1->sFIFOMailBox[fifox].RDHR>>16)&0XFF;
	buffer->FrameData[7]=(CAN1->sFIFOMailBox[fifox].RDHR>>24)&0XFF;    
  	if(fifox==0)CAN1->RF0R|=0X20;//释放FIFO0邮箱
	else if(fifox==1)CAN1->RF1R|=0X20;//释放FIFO1邮箱 

}

void USB_LP_CAN1_RX0_IRQHandler(void)
{	 
   CANBufType TempRxBuf;
	 Can_Rx_Msg(0,&TempRxBuf);
	 ArrayPutFrame(&CANRxType,&TempRxBuf);
}	

u8 Can_TxMsg(CANBufType *TxFrame)
{
	 if(ArrayPutFrame(&CANTxType,TxFrame)==ABUF_NO_ERR)
	 	  return TX_NO_ERR;
	 else
		  return TX_BUFFULL;	 
}

u8 Can_RxMsg(CANBufType *RxFrame)
{
	 if(ArrayGetFrame(&CANRxType,RxFrame)==ABUF_NO_ERR)
	 {	 	 
		 return RX_NO_ERR;
	 }	 
	 else 
		 return RX_BUFLESS; 
}

void TASK_CAN_AutoSend(void)
{
	 CANBufType Tmpbuf;
   if(ArrayGetFrame(&CANTxType,&Tmpbuf)==ABUF_NO_ERR)
	 {
		  Can_Tx_Msg(Tmpbuf);
	 }	 	 
}	
