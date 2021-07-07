#include "send.h"
#include "esp8266.h"
#include "SysTick.h"
#include <stdio.h>  
#include <string.h>  
#include <stdbool.h>
#include "dht11.h"
#include "usart1.h"
#include "ds18b20.h"
#include "mq.h"

/************************************************
*����ͨ��wifi���䵽������
************************************************/


volatile uint8_t ucTcpClosedFlag = 0;

/**
  * @brief  ESP8266 ��Sta Tcp Client��͸��
  * @param  ��
  * @retval ��
  */
void ESP8266_StaTcpClient_UnvarnishTest ( void )
{
	uint8_t ucStatus;
	
	char cStr [ 100 ] = { 0 };

//	DHT11_Data_TypeDef DHT11_Data;
	
	
  printf ( "\r\nConfiguring ESP8266 ......\r\n" );

	macESP8266_CH_ENABLE();
	
	ESP8266_AT_Test ();
	
	ESP8266_Net_Mode_Choose ( STA );

  while ( ! ESP8266_JoinAP ( macUser_ESP8266_ApSsid, macUser_ESP8266_ApPwd ) );	
	
	ESP8266_Enable_MultipleId ( DISABLE );
	
	while ( !	ESP8266_Link_Server ( enumTCP, macUser_ESP8266_TcpServer_IP, macUser_ESP8266_TcpServer_Port, Single_ID_0 ) );
	
	while ( ! ESP8266_UnvarnishSend () );
	
	printf ( "\r\nConfigure ESP8266 finished\r\n" );
	
	
	while ( 1 )
	{		
		//�¶ȴ�����������һ����̼Ũ�ȴ���0
		if(DS18B20_Check()&&Get_Adc_Convert()>=0)
		//	sprintf (cStr, "\r\n Success\r\n Temp:%04.1f�� \r\n CO:%05.1fm3 \r\n",DS18B20_Get_Temp(),Get_Adc_Convert());
			/*ƴ����������������*/
		//	sprintf (cStr, "%s%s",Temp_convert(),CO_convert());
			strcat(strcpy(cStr,Temp_convert()),CO_convert());
			 //strcat(cStr,Temp_convert());
			 //strcat(cStr,CO_convert());
		else
			printf(cStr,"Error\r\n");
		printf ( "%s", cStr );


	
		ESP8266_SendString ( ENABLE, cStr, 0, Single_ID_0 );               //������Ϣ��������
		
		Delay_ms ( 1000 );
		Delay_ms ( 1000 );
		Delay_ms ( 1000 );
		
		if ( ucTcpClosedFlag )                                             //����Ƿ�ʧȥ����
		{
			ESP8266_ExitUnvarnishSend ();                                    //�˳�͸��ģʽ
			
			do ucStatus = ESP8266_Get_LinkStatus ();                         //��ȡ����״̬
			while ( ! ucStatus );
			
			if ( ucStatus == 4 )                                             //ȷ��ʧȥ���Ӻ�����
			{
				printf ( "\r\nReconnection hotspots and servers ......\r\n" );
				
				while ( ! ESP8266_JoinAP ( macUser_ESP8266_ApSsid, macUser_ESP8266_ApPwd ) );
				
				while ( !	ESP8266_Link_Server ( enumTCP, macUser_ESP8266_TcpServer_IP, macUser_ESP8266_TcpServer_Port, Single_ID_0 ) );
				
				printf ( "\r\nReconnect hotspot and server success\r\n" );

			}
			
			while ( ! ESP8266_UnvarnishSend () );		
			
		}

	}
	
		
}


