#ifndef _CAN_H
#define _CAN_H
#include "lib.h"

#define CAN_FRAME_WIDTH      8

typedef struct {
	Uint8	             	  FrameData[CAN_FRAME_WIDTH];
	Uint8                 DLC;
	Uint16                MSGID;
}CANBufType;

#if !ArrayType  
#define ArrayType         1
#define ArrayBufSize      40
typedef struct {
	FrameBufType	           ArrayBuf[ArrayBufSize+1];
	FrameBufType             *ArrayBufInPtr;
	FrameBufType             *ArrayBufOutPtr;
	int16			             ArrayBufCtr;
}ArrayBufType;

#endif

typedef struct
{
   u8 count;
   u8 status;
   u8 workmode;
   u8 error;
   u8 stage;
   u8 speedfeedback;	
}HEARTBEAT_DATA;

typedef struct
{
   u16 WeldingCurrent;
   u16 WeldingVoltage;
   u16 PAR1;	
}FEEDBACK_DATA;

typedef __packed struct
{
   u8  ParaCmd;
   u16 ParaData;	
}WELDINGPARA_DATA;


typedef struct
{
   u8 CANTxReady;
   u8 CANRxReady;
   u8 CANWaitAck;
   u8 CANonlineFlag;
   u8 MemSyncSign;
}CAN_STATUS;

typedef struct tagECANID_INTERNAL
{
	unsigned short reserved:3;	//0b000
	unsigned short source:4;	//Source address
	unsigned short target:4;	//Target address
}ECANID_INTERNAL;


#define CMD_GetReady           0x01
#define CMD_WeldingSW          0x02
#define CMD_WFCtrl             0x02
#define CMD_GFCtrl             0x03
#define CMD_CycStatusCheack    0x04
#define CMD_WeldingStatusCheck 0x05
#define CMD_WalkStatusCheck    0x06
#define CMD_HorStatusCheck     0x07
#define CMD_VtcStatusCheck     0x08
#define CMD_OscStatusCheck     0x09
#define CMD_WeldParameter      0x12
#define CMD_TrailCheck         0x83


#define CMD_WeldingPara       0x0f
#define CMD_Heartbeat         0x80
#define CMD_SendAck           0x81
#define CMD_PowerOn           0x83
#define CMD_PwmStart          0x90
#define CMD_ModeSwitch        0x91
#define CMD_MemStore          0x92
#define CMD_MemRecall         0x93
#define CMD_PwmStop           0x94
#define CMD_FeedBack          0x97
#define CMD_SettingPara       0x98
#define CMD_SendParaAck       0x99
#define CMD_ErrorAlert        0xa0
#define CMD_AutoTrack         0x9a
#define CMD_SwingCtrl         0x9b
#define CMD_EdgeSign          0x9d
#define CMD_Recall            0x1f

#define PARA_WorkMode         0x50
#define PARA_SlopeRate        0x51
#define PARA_WireFeedRate     0x53
#define PARA_P1ArcLenth       0x55
#define PARA_FormerGasT       0x56
#define PARA_LaterGasT        0x57
#define PARA_StartTime        0x58
#define PARA_StopTime         0x59
#define PARA_SlowFeedRate     0x5c
#define PARA_P1ArcLenAdj      0x5d
#define PARA_DPwfspeed        0x5e
#define PARA_P1Deflection     0x5f
#define PARA_Material         0x60
#define PARA_P1Burnback       0x61
#define PARA_StepMode         0x63
#define PARA_MmaCurrent       0x64
#define PARA_StartCurrent     0x65
#define PARA_BoostCurrent     0x66
#define PARA_ArcStatus        0x67
#define PARA_ArcStartTime     0x68
#define PARA_TurnVoltage      0x69
#define PARA_Inductance       0x6a
#define PARA_StartFeedRate    0x6b
#define PARA_StopFeedRate     0x6d
#define PARA_TackTime         0x6e
#define PARA_P1ArcLenComp     0x6f
#define PARA_P1PackCurrent    0x70
#define PARA_P1BaseCurrent    0x71
#define PARA_P1FreqAdj        0x72
#define PARA_P1DPluseFreq     0x73
#define PARA_ArcStrikAdj      0x74
#define PARA_MigMidCurrent    0x75
#define PARA_P1Dutycycle      0x76
#define PARA_P2TransCtrl      0x77
#define PARA_P2BaseCurrent    0x78
#define PARA_P2Voltage        0x7a
#define PARA_P2ArcCtrl        0x7b
#define PARA_P2ArcLenth       0x7c
#define PARA_P2PackCurrent    0x7d
#define PARA_WirefeederSelect 0x7e
#define PARA_DualConfig       0x7f

#define PARA_CCCurrent        0x52
#define PARA_CVVoltage        0x54
#define PARA_CCstartCurrent   0x65
#define PARA_CCstopCurrent    0x66
#define PARA_CVinductance     0x6A
#define	PARA_P1MidCurrent	    0x75

#define WireSpeed             0x53
#define TrvSpeed              0xc2

#define RX_MAX_NUM            255
#define TX_MAX_NUM            255

#define WFswitchOn            0x5a
#define WFswitchOff           0xa5
#define WFstatusOn            0x55
#define WFstatusOff           0x4a
#define WFswitch(x)           (x==0?WFswitchOff:WFswitchOn)
#define WFstatus(x)           (x==0?WFstatusOff:WFstatusOn)

#define BroadcastAddr         0x0f
#define MainboardAddr         0x01
#define MainpanelAddr         0x02
#define WirefeederAddr        0x03
#define WirepanelAddr         0x05
#define RobotcommAddr         0x08
#define FCAWcommAddr          0x06   

#define LocalHostAddr         FCAWcommAddr

extern FEEDBACK_DATA          FeedBackData;
extern const ECANID_INTERNAL  TransmitID0;
extern const ECANID_INTERNAL  TransmitID1;

u8 CAN_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode);
u8 Can_Tx_Msg(CANBufType buffer);
void Can_Rx_Msg(u8 fifox,CANBufType* buffer);
void Can_sendMsg(u16 CID,u8*BYTE,u8 len);

u8 Can_RxMsg(u16* CID,u8 *rxbuf,u8* DLC);
u8 Can_TxMsg(u16 CID,u8 *txbuf,u8 DLC);
void TASK_CAN_AutoSend(void);
#endif
