#ifndef UAPP_COMM_H
#define UAPP_COMM_H
#include "sys.h"

#define CpuTimerms        CpuTimer0.InterruptCount      

#define MAX_DataWidth  FRAME_WIDTH   
#define STA_ON         1
#define STA_OFF        0


#define ACK_SUCC       0xaa
#define ACK_ERR        0xee

#define SEND_OK        0x01
#define SEND_ERR       0x02



#define Hostaddr       0x00

#define MaxOfflinecnt     5


#define SYS_DEVID      0x01


#define SYS_INIT       0x02

#define PARA_INIT      "default"

#define MAX_BOOT_NUM        5120
#define MAX_BIN_NUM         25600
#define MAX_APP_NUM         MAX_BIN_NUM
#define MAX_PARA_NUM        64

#define STM32_FLASH_BASE    0x08000000 	//STM32 FLASH的起始地址
#define ADDR_APP_OFFSET     MAX_BOOT_NUM
#define ADDR_BIN_OFFSET     MAX_BOOT_NUM+MAX_APP_NUM
#define ADDR_PARA_OFFSET    MAX_BOOT_NUM+MAX_APP_NUM+MAX_BIN_NUM


#define ADDR_INIT           0x00
#define ADDR_DEVID          0x02
#define ADDR_ADDR           0x06
#define ADDR_FREQ           0x07
#define ADDR_ValidDev       0x08
#define ADDR_BinLen         0x09
#define ADDR_BinSum         0x0d
#define ADDR_Updata         0x11


#define INIT_OK             0xaabb
#define UPDATA_OK           0x7788

#define Mode_CodeTrans      0x01
#define Mode_BinTrans       0x02
#define Mode_CodeWait       0x03
#define Mode_BinWait        0x04

#define MaxRetryCnt         10
#define MaxTout             18000

#define TransDelayms        300

#define RFCMD_Heartbeat        0x80
#define RFCMD_SendAck          0x81
#define RFCMD_SendBin          0x40
#define RFCMD_SendBinData      0x41
#define RFCMD_SetSourAddr      0x42


#define	CMD_BothStop				0X00//0	0	0	0 	停止 				0x00
#define CMD_LStop_RForward			0x02//0	0	1	0	左停右正转			0x02
#define	CMD_LStop_RBackward			0x01//0	0	0	1	左停右反转			0x01
#define	CMD_LStop_RBrake			0x03//0	0	1	1	左停右刹车			0x03

#define	CMD_LBackward_RStop			0x04//0	1	0	0	左反转右停止			0x04
#define	CMD_LBackward_RForward		0x06//0	1	1	0	左反转右正转			0x06
#define	CMD_LBackward_RBackward		0x05//0	1	0	1	左反转右反转			0x05
#define	CMD_LBackward_RBrake		0x07//0	1	1	1	左反转右刹车			0x07

#define CMD_LForward_RStop			0x08//1	0	0	0	左正转右停止			0x08
#define CMD_LForward_RForward		0x0A//1	0	1	0	左正转右正转			0x0A
#define CMD_LForward_RBackward		0x09//1	0	0	1	左正转右反转			0x09
#define CMD_LForward_RBrake			0x0B//1	0	1	1	左正转右刹车			0x0B

#define	CMD_LBrake_RStop			0x0C//1	1	0	0	左刹车右停止			0x0C
#define	CMD_LBrake_RForward			0x0E//1	1	1	0	左刹车右正转			0x0E
#define	CMD_LBrake_RBackward		0x0D//1	1	0	1	左刹车右反转			0x0D
#define	CMD_LBrake_RBrake			0x0F//1	1	1	1	左刹车右刹车			0x0F

#define CMD_MOTORCTL 				0X66

#define LED0_PWM_SPEED	10
#define LED0_PWM_MAX	2500
//10002为100%占空比  
#define MOTOR_PWM_SPEED	100
#define MOTOR_PWM_MAX	10000

typedef struct 
{
    int sts;
    u16 led0pwmval;
}LED0PWM;


typedef struct 
{
    int CMD;
	long count;
}MOTOCTL;



#define RFCMD_SendLeftForward       0x84
#define RFCMD_SendLeftBackward      0x48
#define RFCMD_SendLeftStop			0x44
#define RFCMD_SendLeftBrake			0x88

#define RFCMD_SendRightForward      0x63
#define RFCMD_SendRightBackward     0x36
#define RFCMD_SendRightStop			0x33
#define RFCMD_SendRightBrake		0x66



#define RFCMD_ReqConn          0x01
#define RFCMD_SetDevID         0x32
#define RFCMD_SetCommFreq      0x33
#define RFCMD_SendCode         0x34
#define RFCMD_SendCodeData     0x35

#define RFCMD_SensorVal        0x83

#define MaxDatabyte            12
typedef union
{
	int CMD_LeftForward;
	int CMD_LeftBackward;
	int CMD_LeftStop;
	int CMD_LeftBrake;	
}RemoteLeftIO;
typedef union
{
	int CMD_RightForward;
	int CMD_RightBackward;
	int CMD_RightStop;
	int CMD_RightBrake;

}RemoteRightIO;

typedef struct
{
	RemoteLeftIO LeftIO;
	RemoteRightIO RightIO;
}RemoteIOsts;

typedef struct
{
	u8  OnLineSta;
	u8  OfflineCnt;
	u8  CommINIT;
	u32 DevID;
	u8  Addr;
	u8  Freq;	
	u32 TimerS;
	u32 TimerS1;
	u32 TimerS2;
	u32 TimerS3;
	u8  DataTransMode;
}CONN_CTRL;

typedef struct
{
	u8   CTSflag;
	long Datasum;
	u32  Framecnt;
	u32  Datalen;
	u32  Datacnt;
	long Tmpsum;
}TRANSMIT_CTRL;




void _ReqConn_TASK(void);
void _HartBeat_TASK(void);
u8 _READ_OnlineSta(void);

void _Comm_TASK1000ms(void);  //1HZ
void _Comm_TASK1ms(void);     //1000HZ
void _Comm_TASK10ms(void);    //100HZ
void _Comm_TASK100ms(void);   //10HZ
u8 _UAPP_Comm_INIT(void);
void _Comm_TASK250ms(void) ;//4HZ；
void _Comm_TASK5ms(void);
void _COMM_STA_INIT(void);
void SendIOmsg(short LeftForward ,short LeftBackward ,short LeftStop ,short LeftBrake,
			   short RightForward,short RightBackward,short RightStop,short RightBrake,u8 Taraddr);

#endif
