#include "lib.h"

ArrayBufType CANTxType,CANRxType;

const ECANID_INTERNAL TransmitID0={0,LocalHostAddr,MainboardAddr}; 
const ECANID_INTERNAL TransmitID1={0,LocalHostAddr,BroadcastAddr}; 

const ECANID_INTERNAL ReceiveID0={0,MainboardAddr,LocalHostAddr}; 
const ECANID_INTERNAL ReceiveID1={0,MainboardAddr,BroadcastAddr}; 

static void InitArrayBuf(void);

u8 CAN_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode)
{ 
		u16 i=0,j=0;
		if(tsjw==0||tbs2==0||tbs1==0||brp==0)return 1;
		tsjw-=1;//先减去1.再用于设置
		tbs2-=1;
		tbs1-=1;
		brp-=1;
		RCC->APB2ENR|=1<<2;    //使能PORTA时钟	 
		GPIOA->CRH&=0XFFF00FFF; 
		GPIOA->CRH|=0X000B8000;//PA11 RX,PA12 TX推挽输出   	 
		GPIOA->ODR|=3<<11;
			
		RCC->APB1ENR|=1<<25;//使能CAN时钟 CAN使用的是APB1的时钟(max:36M)	
				
		CAN->MCR=0x0000;	//退出睡眠模式(同时设置所有位为0)
		CAN->MCR|=1<<0;		//请求CAN进入初始化模式
		while((CAN->MSR&1<<0)==0)
		{
			i++;
			if(i>100)return 2;//进入初始化模式失败
		}
		CAN->MCR|=0<<7;		//非时间触发通信模式
		CAN->MCR|=1<<6;		//软件自动离线管理
		CAN->MCR|=0<<5;		//睡眠模式通过软件唤醒(清除CAN->MCR的SLEEP位)
		CAN->MCR|=0<<4;		//允许报文自动传送
		CAN->MCR|=0<<3;		//报文不锁定,新的覆盖旧的
		CAN->MCR|=0<<2;		//优先级由报文标识符决定
		CAN->BTR=0x00000000;//清除原来的设置.
		CAN->BTR|=mode<<30;	//模式设置 0,普通模式;1,回环模式;
		CAN->BTR|=tsjw<<24; //重新同步跳跃宽度(Tsjw)为tsjw+1个时间单位
		CAN->BTR|=tbs2<<20; //Tbs2=tbs2+1个时间单位
		CAN->BTR|=tbs1<<16;	//Tbs1=tbs1+1个时间单位
		CAN->BTR|=brp<<0;  	//分频系数(Fdiv)为brp+1
							//波特率:Fpclk1/((Tbs1+Tbs2+1)*Fdiv)

		//过滤器初始化
		CAN->FMR|=1<<0;			                                                //过滤器组工作在初始化模式
		CAN->FA1R&=~(1<<0);		                                              //过滤器0不激活
		CAN->FA1R&=~(1<<1);                                                 //过滤器1不激活
		CAN->FS1R|=7<<0; 		                                                //过滤器0,1,2,3位宽为32位.
		CAN->FM1R|=0<<0;		                                                //过滤器0工作在标识符屏蔽位模式
		CAN->FM1R|=0<<1;		                                                //过滤器1工作在标识符屏蔽位模式
		CAN->FFA1R|=0<<0;		                                                //过滤器0关联到FIFO0
		CAN->FFA1R|=0<<1;		                                                //过滤器1关联到FIFO0

		CAN->sFilterRegister[0].FR1=*(u16*)&ReceiveID0<<21;                //32位ID
		CAN->sFilterRegister[0].FR2=0XFF000000;    	//32位MASK	
		CAN->sFilterRegister[1].FR1=*(u16*)&ReceiveID1<<21;                //32位ID
		CAN->sFilterRegister[1].FR2=0XF0000000;                             //32位MASK	

		CAN->FA1R|=1<<0;		//激活过滤器0	
		CAN->FA1R|=1<<1;		//激活过滤器1	
		
		CAN->FMR&=~(1<<0);  //过滤器组进入正常模式

		CAN->IER|=1<<1;			//FIFO0消息挂号中断允许.	    
		MY_NVIC_Init(1,1,USB_LP_CAN_RX0_IRQChannel,2);//组2	
		
//		MY_NVIC_Init(1,3,CAN_TX_IRQn,2);
		
		CAN->MCR&=~(1<<0);	//请求CAN退出初始化模式
		while((CAN->MSR&1<<0)==1)
		{
			i++;
			if(i>0XFFF0)return 3;//退出初始化模式失败
		}
		while((CAN->ESR&1<<2)==1<<2)  //确认退出离线模式
		{
			j++;
			if(j>0XFFF0) return 1;
		}		
		
		InitArrayBuf();
		return 0;
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

static u16 ArrayGetBufCtr(ArrayBufType *pbuf)
{
	return(pbuf->ArrayBufCtr);       
}
/***********************************************************************************
*                              数据写入队列缓冲区
***********************************************************************************/

static u8 ArrayPutFrame(ArrayBufType *Arraybuf,CANBufType *Frame)
{   
	 if(Arraybuf->ArrayBufCtr<ArrayBufSize)
	 {
			 Arraybuf->ArrayBufCtr++;
			 memcpy(Arraybuf->ArrayBufInPtr->FrameData,Frame->FrameData,CAN_FRAME_WIDTH);
			 Arraybuf->ArrayBufInPtr->DLC=Frame->DLC;
			 Arraybuf->ArrayBufInPtr->MSGID=Frame->MSGID;
			 Arraybuf->ArrayBufInPtr++;
			 if(Arraybuf->ArrayBufInPtr==&Arraybuf->ArrayBuf[ArrayBufSize])
					 Arraybuf->ArrayBufInPtr=&Arraybuf->ArrayBuf[0]; 
			 return ABUF_NO_ERR;
			 
	 }
	 return ABUF_TXBUFFULL_ERR;
}
            
    
/***********************************************************************************
*                              取出队列缓冲区数据
***********************************************************************************/
static u8 ArrayGetFrame(ArrayBufType *Arraybuf,CANBufType *Frame)
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
	 if(Arraybuf->ArrayBufOutPtr==&Arraybuf->ArrayBuf[ArrayBufSize])
			Arraybuf->ArrayBufOutPtr=&Arraybuf->ArrayBuf[0];   
   return ABUF_NO_ERR;
}

u8 Can_Tx_Msg(CANBufType buffer)
{	   
	u8 mbox;	  
	if(CAN->TSR&(1<<26))mbox=0;			//邮箱0为空
	else if(CAN->TSR&(1<<27))mbox=1;	//邮箱1为空
	else if(CAN->TSR&(1<<28))mbox=2;	//邮箱2为空
	else return(0);
	CAN->sTxMailBox[mbox].TIR=0;		//清除之前的设置  
	CAN->sTxMailBox[mbox].TIR|=(((unsigned long)buffer.MSGID)<<21);		 
	CAN->sTxMailBox[mbox].TIR|=0<<2;	  
	CAN->sTxMailBox[mbox].TIR|=0<<1;
	CAN->sTxMailBox[mbox].TDTR&=~(0X0000000F);
	CAN->sTxMailBox[mbox].TDTR|=buffer.DLC;		   //设置DLC.
	//待发送数据存入邮箱.
	CAN->sTxMailBox[mbox].TDHR=(((u32)buffer.FrameData[7]<<24)|
								((u32)buffer.FrameData[6]<<16)|
 								((u32)buffer.FrameData[5]<<8)|
								((u32)buffer.FrameData[4]));
	CAN->sTxMailBox[mbox].TDLR=(((u32)buffer.FrameData[3]<<24)|
								((u32)buffer.FrameData[2]<<16)|
 								((u32)buffer.FrameData[1]<<8)|
								((u32)buffer.FrameData[0]));
	CAN->sTxMailBox[mbox].TIR|=1<<0; //请求发送邮箱数据
	return 1;
}

void Can_Rx_Msg(u8 fifox,CANBufType* buffer)
{	    
	u8 i=0;
	buffer->DLC=CAN->sFIFOMailBox[fifox].RDTR&0x0F;//得到DLC
	buffer->MSGID =CAN->sFIFOMailBox[fifox].RIR>>16;  //得到ID
	buffer->FrameData[0]=CAN->sFIFOMailBox[fifox].RDLR&0XFF;
	buffer->FrameData[1]=(CAN->sFIFOMailBox[fifox].RDLR>>8)&0XFF;
	buffer->FrameData[2]=(CAN->sFIFOMailBox[fifox].RDLR>>16)&0XFF;
	buffer->FrameData[3]=(CAN->sFIFOMailBox[fifox].RDLR>>24)&0XFF;    
	buffer->FrameData[4]=CAN->sFIFOMailBox[fifox].RDHR&0XFF;
	buffer->FrameData[5]=(CAN->sFIFOMailBox[fifox].RDHR>>8)&0XFF;
	buffer->FrameData[6]=(CAN->sFIFOMailBox[fifox].RDHR>>16)&0XFF;
	buffer->FrameData[7]=(CAN->sFIFOMailBox[fifox].RDHR>>24)&0XFF;    
  	if(fifox==0)CAN->RF0R|=0X20;//释放FIFO0邮箱
	else if(fifox==1)CAN->RF1R|=0X20;//释放FIFO1邮箱 

}

void USB_LP_CAN1_RX0_IRQHandler(void)
{	 
   CANBufType TempRxBuf;
	 Can_Rx_Msg(0,&TempRxBuf);
	 ArrayPutFrame(&CANRxType,&TempRxBuf);
}	

u8 Can_TxMsg(u16 CID,u8 *txbuf,u8 DLC)
{
	 CANBufType TempTxBuf;
	 TempTxBuf.MSGID=CID;
	 TempTxBuf.DLC=DLC;
	 memcpy(TempTxBuf.FrameData,txbuf,DLC);
	 if(ArrayPutFrame(&CANTxType,&TempTxBuf)==ABUF_NO_ERR)
	 	  return TX_NO_ERR;
	 else
		  return TX_BUFFULL;	 
}

u8 Can_RxMsg(u16* CID,u8 *rxbuf,u8* DLC)
{
	 CANBufType TempRxBuf;
	 if(ArrayGetFrame(&CANRxType,&TempRxBuf)==ABUF_NO_ERR)
	 {	 
		 *DLC=TempRxBuf.DLC;
		 *CID=TempRxBuf.MSGID;
		 memcpy(rxbuf,TempRxBuf.FrameData,TempRxBuf.DLC);		 
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


/*发送数据示例
void _SEND_TempFeedback(void)
{
	short Onlinenum=_Read_CanOnlineSta();
	if(Onlinenum>0)
	{	
			CANBufType TxFrame;
			SENSER_VAL SenserData;
			_READ_SenserData(&SenserData);
			TxFrame.MSGID=*(Uint16*)&TransmitID3;
			TxFrame.FrameData[0]=CMD_Tempdata;//0x95
			TxFrame.FrameData[1]=SenserData.Temp1&0xff;
			TxFrame.FrameData[2]=SenserData.Temp1>>8;
			TxFrame.FrameData[3]=SenserData.TempA&0xff;
			TxFrame.FrameData[4]=SenserData.TempA>>8;
			TxFrame.DLC=5;
			Can_TxMsg(&TxFrame);
	}	
}
*/

/*接收数据示例
u8 CanReceiveProcess(void)
{
   u8 Ackflag=0,Tmpid; ECANID_INTERNAL CID;
	 CANBufType  CANRxData,CANTxData;
	 memset(&CANRxData,0,sizeof(CANRxData));	 
	 if(Can_RxMsg(&CANRxData)!=RX_NO_ERR) 
         return 0; 
	 memcpy(&CID,&CANRxData.MSGID,2);
	 switch(CANRxData.FrameData[0])
	 {
		 case CMD_SendAck://0x81
			 switch(CANRxData.FrameData[1])
			 {
                  case RFCMD_ReqConn://0x01 接收到主机发送的应答，则从机上线
                         ConnCtrl.OnLineSta=STA_ON;
                      //  _LED_SetTime(1,60000,0,0);       
                       break;                 
				  case CMD_CodeApply://0x30应答发送扫码数据申请
						CAN_CodeTrans.CTSflag=CANRxData.FrameData[2];
					  break;
					case CMD_CodeData://应答发送扫码数据
						CAN_CodeTrans.CTSflag=CANRxData.FrameData[2];
					  break;
			 }	 
		   break;
		 case CMD_GetReady://0x01

       break; 
     case CMD_WeldingStatusCheck://0x05

       break;
     case CMD_Heartbeat://0x80//
/* 
         0x07C0 2810ID     0000 01111 1000 000? 
         0x07D0 4GboardID  0000 01111 1010 000?
         0X0788 1068-4     0000 01111 0001 000?
         0x00d0            0000 00001 1010 000
         0X0450            0000 01000 1010 000
/*     
       LedProcess(CANRxData.MSGID);    
       _CommWatchDog_Feed();			 
       memcpy(&HeartBeatData,&CANRxData.FrameData[1],CANRxData.DLC-1);
       break;
     case CMD_FeedBack://0x97
       memcpy(&FeedBackData,&CANRxData.FrameData[1],CANRxData.DLC-1);
       break;
     case CMD_ErrorAlert://0xa0		 
       break;
     
     
     case CMD_BinApply://0x40
        if(CANRxData.MSGID==0x0450) 
       Ackflag=_Rx_BinApply(CANRxData.FrameData,CID.target);         
		   break;	//返回0xaa，id=0x0450；
     case CMD_BinData://0x41
         if(CANRxData.MSGID==0x0450)
        Ackflag=_Rx_BinData(CANRxData.FrameData,CANRxData.DLC,CID.target);
       break;
     
     
     case CMD_RESET://0x42
       Ackflag=_Rx_ResetDevid(CANRxData.FrameData);
       break;		 
	 }	
   if(Ackflag!=0)
   {
	    _SEND_ACK(CANRxData.FrameData[0],Ackflag,&CID);
	 }		 
   return 1;	 
}
*/