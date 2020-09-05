#ifndef _CODEMODULE_H
#define _CODEMODULE_H

#include "sys.h"

//主机命令
#define CMD_WakeUp           0x00

#define CMD_Beep             0xE6
#define CMD_HostAck          0xD0
#define CMD_HostNack         0xD1
#define CMD_LedOn            0xE7
#define CMD_LedOff           0xE8
#define CMD_StartDecode      0xE4
#define CMD_StopDecode       0xE5
#define CMD_Sleep            0xEB
#define CMD_ScanDisable      0xEA
#define CMD_ScanEnable       0xE9
#define CMD_ParaSendhost     0xD6
#define CMD_ParaRequest      0xD7
#define CMD_ParaDefault      0xD9
#define CMD_ParaWrite        0xEC
#define CMD_CustomDefault    0xD8
#define CMD_ReqVision        0xA3
#define CMD_ParaScan         0xE0
#define CMD_NoRead           0xE1
#define CMD_CapImage         0xE2

//引擎命令
#define CMD_EngineAck        0xD0
#define CMD_EngineNack       0xD1
#define CMD_DecodeData       0xF3
#define CMD_ParaSendEngine   0xD6
#define CMD_ReplyVision      0xA4


//参数代码
#define CODE_ScanMode        0x0191
#define CODE_HoldTime        0x0192 
#define CODE_MultiVerify     0x0194 
#define CODE_1Dmaxlen        0x0195 
#define CODE_1Dminlen        0x0196
#define CODE_OutputDelay     0x019B 
#define CODE_DecodeSync      0x0137
#define CODE_AckNack         0x012D
#define CODE_SleepMode       0x01A0 
#define CODE_SleepDealy      0x019C 
#define CODE_Poweron         0x01F5
#define CODE_Ledon           0x01F6
#define CODE_Beepon          0x01F7
#define CODE_Beeptime        0x01F8
#define CODE_Beepvol         0x01F9
#define CODE_LightMode       0x2329
#define CODE_SightMode       0x232A
#define CODE_Brightness      0x232B
#define CODE_ScreenRead      0x03EF

#define STAT_Idle            0
#define STAT_Wakeup          1
#define STAT_Beep            2
#define STAT_Ledon           3
#define STAT_LedOff          4
#define STAT_StartDecode     5
#define STAT_StopDecode      6
#define STAT_ReqVision       7
#define STAT_ScanMode        8
#define STAT_EncodeSync      9
#define STAT_AckNack         10
#define STAT_HoldTime        11
#define STAT_HostAck         12

#define STAT_WaitData        0xfc
#define STAT_WaitAck         0xfd

#define STAT_Delay           0xfe
#define STAT_END             0xff


#define RecOk                0
#define RecErr               1
#define RecTout              2
#define RecWait              3

#define Tready               1
#define Twait                0

#define Readok               0
#define Readerr              1

#define NRead                0x55



#define MaxDatalen           150
#define MaxErrNum            10

//#define TrigKey              PBin(9)//PB9//before PDin(2)
//#define TrigLayout           PEout(6)


typedef struct
{
	u8 StageManage[30];
	u8 StageCnt;
	u8 StageRec;
	u32 TimerCnt;
	u32 Toutms;
	u8 ErrManage;
	u8 ErrCnt;
}PROC_CTRL;

typedef struct
{
	char* FrameData;
  u8    Stage;
}STAT_CTRL;

typedef struct {
	u8   CodeData[MaxDatalen];
	u16  DLC;
	u8   Readflag;
}DataBufType;

void _FrameProcCtrl(void);
u8 _ProcStat_Check(void);
/* ProcQueue:对话流程队列, ProcNum:队列长度, Tout:等待数据超时时间 */
u8 _Start_CommProc(u8* ProcQueue,u8 ProcNum,u8 Tout);
void TrigKey_Task(void);
void GPIO_Init(void);
void CodeModule_INIT(void);
u8 _ReadDecodeData(DataBufType** Codedata);
#endif

