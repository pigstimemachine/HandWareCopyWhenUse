#include "CodeModule.h"
#include "usart.h"
#include "lib.h"
#define ByteH(x) (x>>8)&0xff
#define ByteL(x) x&0xff
#define Rol(x,y) ((long)x<<y)
#define RoR(x,y) ((long)x>>y)

PROC_CTRL ProcCtrl={0,0,0,0,0,0,0,0};
DataBufType DecodeData;
const char BeepFrame[]={0x05,CMD_Beep,0x04,0x00,0x0d,0x00,0x00};
const char HostAckFrame[]={0x04,CMD_HostAck,0x04,0x00,0x00,0x00};
const char LedonFrame[]={0x05,CMD_LedOn,0x04,0x00,0x01,0x00,0x00};
const char LedoffFrame[]={0x05,CMD_LedOff,0x04,0x00,0x01,0x00,0x00};
const char StartDecodeFrame[]={0x04,CMD_StartDecode,0x04,0x00,0x00,0x00};
const char StopDecodeFrame[]={0x04,CMD_StopDecode,0x04,0x00,0x00,0x00};
const char ReqVisionFrame[]={0x04,CMD_ReqVision,0x04,0x00,0x00,0x00};
const char ScanModeFrame[]={0x09,0xd6,0x04,0x08,0xff,ByteH(CODE_ScanMode),ByteL(CODE_ScanMode),0x01,0x05,0x00,0x00};
const char DecodeSyncFrame[]={0x09,0xd6,0x04,0x08,0xff,ByteH(CODE_DecodeSync),ByteL(CODE_DecodeSync),0x01,0x01,0x00,0x00};
const char AckNackFrame[]={0x09,0xd6,0x04,0x08,0xff,ByteH(CODE_AckNack),ByteL(CODE_AckNack),0x01,0x06,0x00,0x00};
const char HoldTimeFrame[]={0x09,0xd6,0x04,0x08,0xff,ByteH(CODE_HoldTime),ByteL(CODE_HoldTime),0x01,0x05,0x00,0x00};

const char WakeupFrame[]={0x00,0x00};

const char EngineAckFrame[]={0x00,0x05,0xd0,0x00,0x00,0xff,0xff,0xff,0x2b};


STAT_CTRL StatCtrlbuf[]={{(char*)BeepFrame,STAT_Beep},
                      {(char*)WakeupFrame,STAT_Wakeup},
                      {(char*)LedonFrame,STAT_Ledon},
                      {(char*)LedoffFrame,STAT_LedOff},
                      {(char*)StartDecodeFrame,STAT_StartDecode},
                      {(char*)StopDecodeFrame,STAT_StopDecode},
											{(char*)ReqVisionFrame,STAT_ReqVision},
											{(char*)ScanModeFrame,STAT_ScanMode},
											{(char*)DecodeSyncFrame,STAT_EncodeSync},
											{(char*)AckNackFrame,STAT_AckNack},
											{(char*)HoldTimeFrame,STAT_HoldTime},
											{(char*)HostAckFrame,STAT_HostAck},
                     };
#define Statbuflen sizeof(StatCtrlbuf)/sizeof(STAT_CTRL)+1
void GPIO_Init(void)
{		
	RCC->APB2ENR|=1<<6;//先使能外设IO PORTE时钟 							 
	GPIOE->CRL&=0XF0FFFFFF;//PE6推挽输出
	GPIOE->CRL|=0X03000000;	   
	GPIOE->ODR|=1<<6;     //PE6 输出高
	
	RCC->APB2ENR|=1<<5;//先使能外设IO PORTD时钟 							 
	GPIOD->CRL&=0XFFFFF0FF;//PD2输入
	GPIOD->CRL|=0X00000800;	   
	GPIOD->ODR|=1<<2;     //PD2
	
	RCC->APB2ENR|=1<<2;//先使能外设IO PORTA时钟 							 
	GPIOA->CRH&=0XFFF00FFF;//PD2输入
	GPIOA->CRH|=0X0008B000;	   
	GPIOA->ODR|=3<<11;     //PD2	
}  


/* ProcQueue:对话流程队列, ProcNum:队列长度, Tout:等待数据超时时间 */
u8 _Start_CommProc(u8* ProcQueue,u8 ProcNum,u8 Tout)
{
   if(ProcCtrl.StageRec==STAT_Idle)
	 {
		   memset(&ProcCtrl,0,sizeof(ProcCtrl));
            memcpy(ProcCtrl.StageManage,ProcQueue,ProcNum);
		   ProcCtrl.Toutms=Tout;
		   return 0;
	 }	
   return 1;	 
}
/* 对话流程状态查询 */
u8 _ProcStat_Check(void)
{
	 return ProcCtrl.StageRec;	 
}	

void _ProcessCNT(char cnt)
{
   ProcCtrl.StageCnt+=cnt;
	 ProcCtrl.StageRec=ProcCtrl.StageManage[ProcCtrl.StageCnt];
	 ProcCtrl.TimerCnt=0;
	 delay_ms(120);
}

void _SEND_HostFrame(char* Frame)
{
	 UARTBufType Framebuf;  u32 Checksum=0x10000; u8 cnt,Tmplen;
	 Tmplen=*Frame;
	 for(cnt=0;cnt<Tmplen;cnt++)
	 {
	     Checksum-=*(Frame+cnt);
	 }
	 memcpy(Framebuf.FrameData,Frame,Tmplen);
	 Framebuf.FrameData[Tmplen++]=RoR(Checksum,8)&0xff;
   Framebuf.FrameData[Tmplen++]=Checksum&0xff;	
	 Framebuf.DLC=Tmplen;
   USART1_WriteFrame(&Framebuf);	 
}	

u8 _DECODE_Data(u8* Frame,DataBufType* Codebuf)
{
	 long long Checksum1=0x100000000,Checksum2=0; u16 cnt,Tmplen=0,Datalen=0;
	 char OPAcode=0xF3;
   Tmplen=Rol(*Frame,8)&0xff00;
	 Tmplen|=*(Frame+1)&0xff;
	 Datalen=Tmplen-6;
	 for(cnt=0;cnt<Tmplen;cnt++)
	 {
	     Checksum1-=Frame[cnt];
	 }
	 Checksum2=Rol(Frame[Tmplen++],24)&0xff000000;
	 Checksum2|=Rol(Frame[Tmplen++],16)&0x00ff0000;
	 Checksum2|=Rol(Frame[Tmplen++],8)&0x0000ff00;
	 Checksum2|=Frame[Tmplen++]&0x000000ff; 
	 if(Checksum2==Checksum1)
	 {
	     if(Frame[2]==OPAcode)
			 {
			     memcpy(Codebuf->CodeData,Frame+6,Datalen);
				   Codebuf->DLC=Datalen;
				   Codebuf->Readflag=NRead;
				   return RecOk;
			 }	 
	 }	 
	 return RecErr;
}

u8 _WAIT_EngineData(u32 Tout)
{
   UARTBufType TempFrame; u8 err=0;
	 memset(&TempFrame,0,sizeof(TempFrame));
	 ProcCtrl.TimerCnt++;
	 if(USART1_ReadFrame(&TempFrame)==ReadOK)
	 {
		  ProcCtrl.TimerCnt=0;
		  err=_DECODE_Data(TempFrame.FrameData,&DecodeData);
		  return err;
	 }	
	 if((ProcCtrl.TimerCnt>Tout)&&(Tout!=0))
	 {
		  ProcCtrl.TimerCnt=0;
	    return RecTout;
	 }	
   return RecWait;	 
}

u8 _Delay_Task(u32 Tout)
{
   ProcCtrl.TimerCnt++;
	 if((ProcCtrl.TimerCnt>Tout)&&(Tout!=0))
	 {
		  ProcCtrl.TimerCnt=0;
	    return Tready;
	 }
	 return Twait;
}

u8 _WAIT_EngineAck(u32 Tout)
{
   UARTBufType TempFrame; u8 err=0;
	 memset(&TempFrame,0,sizeof(TempFrame));
	 ProcCtrl.TimerCnt++;
	 if(USART1_ReadFrame(&TempFrame)==ReadOK)
	 {
		  ProcCtrl.TimerCnt=0;
		  if(TempFrame.DLC==sizeof(EngineAckFrame))
			{	
			   err=strcmp((char*)(&TempFrame.FrameData),EngineAckFrame);
				 if(err==0)
				 {
				    return RecOk;
				 }
         else return RecErr;					 
      }	
      else return RecErr;			
	 }	
	 if((ProcCtrl.TimerCnt>Tout)&&(Tout!=0))
	 {
		  ProcCtrl.TimerCnt=0;
	    return RecTout;
	 }	 
   return RecWait;	 
}	

void _FrameProcCtrl(void)
{
	 u8 ACK=0,cnt=0;
   switch(ProcCtrl.StageRec)
	 {
		 case STAT_Idle:
			 ProcCtrl.StageRec=ProcCtrl.StageManage[0];
 
       break;
		 case STAT_Delay:
			 if(_Delay_Task(1000)==Tready)
				 _ProcessCNT(1);
		   break;
		 case STAT_WaitAck:
			 ACK=_WAIT_EngineAck(2000);
		   switch(ACK)
			 {
				  case RecOk:	
					  _ProcessCNT(1); 
//            delay_ms(45);					
				    break;								
					case RecErr:
					case RecTout:
						ProcCtrl.ErrCnt++;
					  if(ProcCtrl.ErrCnt>MaxErrNum) 							
						   memset(&ProcCtrl,0,sizeof(ProcCtrl));
						else
							 _ProcessCNT(-1); 
					  ProcCtrl.ErrManage=RecErr;
					  break;
					case RecWait:
						
					  break;
			 }	       			 
       break;
		 case STAT_WaitData:      
			 ACK=_WAIT_EngineData(ProcCtrl.Toutms);
		   switch(ACK)
			 {
				  case RecOk:
					  _ProcessCNT(1);
				    break;								
					case RecErr:
					case RecTout:
						ProcCtrl.ErrCnt++;
					  if(ProcCtrl.ErrCnt>MaxErrNum) 
						   memset(&ProcCtrl,0,sizeof(ProcCtrl));
						else 
							 _ProcessCNT(-1);
					  ProcCtrl.ErrManage=RecErr;
					  break;
					case RecWait:
						
					  break;
			 }	  
			 break;
		 case STAT_END:
		   memset(&ProcCtrl,0,sizeof(ProcCtrl));
       break;	
     default:
       for(cnt=0;cnt<Statbuflen;cnt++)
		   {
			    if(ProcCtrl.StageRec==StatCtrlbuf[cnt].Stage)
					{
					    _SEND_HostFrame(StatCtrlbuf[cnt].FrameData);
						  _ProcessCNT(1);
						  break;
					}	
			 } 
			 if(cnt==Statbuflen) memset(&ProcCtrl,0,sizeof(ProcCtrl));
       break;		 
	 }	 
}	

void CodeModule_INIT(void)
{
   u8 TmpProcQue[]={STAT_Delay,STAT_Wakeup,STAT_Wakeup,STAT_Wakeup,STAT_AckNack,STAT_WaitAck,STAT_EncodeSync,STAT_WaitAck,STAT_HoldTime,STAT_WaitAck,STAT_END}; 
	 _Start_CommProc(TmpProcQue,sizeof(TmpProcQue),0);
}	

void TrigKey_Task(void)
{
	 u8 TmpProcQue[]={STAT_Wakeup,STAT_WaitData,STAT_HostAck,STAT_END};
//	 if(ProcCtrl.ErrManage==RecOk)
	    _Start_CommProc(TmpProcQue,sizeof(TmpProcQue),0); 
}	

u8 _ReadDecodeData(DataBufType** Codedata)
{
   if(DecodeData.Readflag==NRead)
	 {	 
	 	 *Codedata=&DecodeData;
		 DecodeData.Readflag=0;
		 return Readok;
	 } 
	 return Readerr;
}
