#include "lib.h"

WELDINGPARA_DATA    ParaData;
HEARTBEAT_DATA      HeartBeatData;
FEEDBACK_DATA       FeedBackData;
CAN_STATUS          CanStatus;
ECANID_INTERNAL ReceiveID;


u8 CanReceiveProcess(void)
{
   u8 NeedAck=NULL; ECANID_INTERNAL CID;
	 CANBufType  CANRxData,CANTxData;
	 memset(&CANRxData,0,sizeof(CANRxData));	 
	 Can_RxMsg(&CANRxData.MSGID,CANRxData.FrameData,&CANRxData.DLC);  
	 switch(CANRxData.FrameData[0])
	 {
		 case CMD_GetReady:

       break; 
     case CMD_WeldingStatusCheck:

       break;
     case CMD_Heartbeat: 
       memcpy(&HeartBeatData,&CANRxData.FrameData[1],CANRxData.DLC-1);
       break;
     case CMD_FeedBack:
       memcpy(&FeedBackData,&CANRxData.FrameData[1],CANRxData.DLC-1);
       break;
     case CMD_ErrorAlert:
			 
       break;
     case CMD_SendParaAck:
       
		   break;	
     case CMD_SettingPara:
       
       break;		 
	 }		
   return 1;	 
}	


void CANTaskOperation(void)  //1000HZ
{
   CanReceiveProcess();
	 TASK_CAN_AutoSend();
}
