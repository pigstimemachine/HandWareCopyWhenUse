#ifndef  __SEND_H
#define	 __SEND_H



#include "stm32f10x.h"



/********************************** �û���Ҫ���õĲ���**********************************/
//Ҫ���ӵ��ȵ�����ƣ���WIFI����
#define      macUser_ESP8266_ApSsid           "TP-LINK_D1BA" 

//Ҫ���ӵ��ȵ����Կ
#define      macUser_ESP8266_ApPwd            "70212345" 

//Ҫ���ӵķ������� IP�������Ե�IP
#define      macUser_ESP8266_TcpServer_IP     "192.168.1.101" 

//Ҫ���ӵķ������Ķ˿�
#define      macUser_ESP8266_TcpServer_Port    "8086"         



/********************************** �ⲿȫ�ֱ��� ***************************************/
extern volatile uint8_t ucTcpClosedFlag;



/********************************** ���Ժ������� ***************************************/
void                     ESP8266_StaTcpClient_UnvarnishTest  ( void );



#endif
