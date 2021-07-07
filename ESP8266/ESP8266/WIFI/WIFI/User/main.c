/**
  ******************************************************************************
  *������-��������
  ******************************************************************************
  */ 
 
#include "stm32f10x.h"
#include "usart1.h"
#include "SysTick.h"
#include "esp8266.h"
#include "Send.h"
#include "dht11.h"
#include "mq.h"
 
 

/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main ( void )
{
	/* ��ʼ�� */
    USARTx_Config ();                       //��ʼ������1
	SysTick_Init ();                       //���� SysTick Ϊ 1ms �ж�һ�� 
	ESP8266_Init ();                      //��ʼ��WiFiģ��ʹ�õĽӿں�����
	DHT11_Init ();

	
	printf ( "\r\nFire monitoring and alarm system\r\n" );        //��ӡ����������ʾ��Ϣ

	
    ESP8266_StaTcpClient_UnvarnishTest ();
	
	
  while ( 1 );
	
	
}


/*********************************************END OF FILE**********************/
