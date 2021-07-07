#ifndef __IIC_H
#define __IIC_H
#include "sys.h"
 /******SCL PA0 / SDA PA1*****************/ 	   		   
//IO方向设置
#define SDA_IN()  {GPIOA->CRL&=0XFFFFFF0F;GPIOA->CRL|=0x00000080;}   //IIC_SDA DIR
#define SDA_OUT() {GPIOA->CRL&=0XFFFFFF0F;GPIOA->CRL|=0x00000030;}   //IIC_SDA DIR

//IO操作函数	 
#define IIC_SCL    PAout(0) //SCL
#define IIC_SDA    PAout(1) //SDA	 
#define READ_SDA   PAin(1)  //输入SDA  

//IIC所有操作函数
void IIC_Init(void);                //初始化IIC的IO口				 
u8 IIC_Start(void);				//发送IIC开始信号
void IIC_Stop(void);	  			//发送IIC停止信号
void IIC_Send_Byte(u8 txd);			//IIC发送一个字节
u8 IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
u8 IIC_Wait_Ack(void); 				//IIC等待ACK信号
void IIC_Ack(void);					//IIC发送ACK信号
void IIC_NAck(void);				//IIC不发送ACK信号

void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte(u8 daddr,u8 addr);	

void I2C_WriteOneByte(u8 id, u16 write_address,u8 byte);
u8 I2C_ReadOneByte(u8 id, u16 read_address);
  
#endif
















