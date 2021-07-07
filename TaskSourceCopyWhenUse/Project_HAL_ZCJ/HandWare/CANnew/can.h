#ifndef _CAN_H
#define _CAN_H
#include "lib.h"

#define CAN_FRAME_WIDTH      8//32字节的用户数据宽度

typedef struct {
	u8	             	 FrameData[CAN_FRAME_WIDTH];
	u8                 DLC;
	u16                MSGID;
}CANBufType;


#define CanArrayBufSize      40
typedef struct {
	CANBufType	           ArrayBuf[CanArrayBufSize+1];
	CANBufType             *ArrayBufInPtr;
	CANBufType             *ArrayBufOutPtr;
	short			             ArrayBufCtr;
}CANArrayBufType;



#define RX_MAX_NUM            255
#define TX_MAX_NUM            255



#define ABUF_NO_ERR               0

#define ABUF_FRAMEFORMATS_ERR     1

#define ABUF_TXBUFFULL_ERR        2

#define ABUF_CHARSTRING_ERR       3

#define ABUF_ArrayLength_ERR      4

#define ABUF_RXBUFLESS_ERR        5

#define RX_NO_ERR               0
#define TX_NO_ERR               0
#define TX_BUFFULL              1
#define RX_BUFLESS              1

u8 CAN_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode);
u8 Can_Tx_Msg(CANBufType buffer);
void Can_Rx_Msg(u8 fifox,CANBufType* buffer);
void Can_sendMsg(u16 CID,u8*BYTE,u8 len);

u8 Can_RxMsg(CANBufType *RxFrame);
u8 Can_TxMsg(CANBufType *TxFrame);
void TASK_CAN_AutoSend(void);
void SetRxMbox(u16 STDMSGID,u32 EXTMSGID,u16 STDMSGIDLAM,u32 EXTMSGIDLAM,u8 mailbox);
#endif





