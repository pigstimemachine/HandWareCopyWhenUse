/**
  ******************************************************************************
  *主函数-传输数据
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
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main ( void )
{
	/* 初始化 */
    USARTx_Config ();                       //初始化串口1
	SysTick_Init ();                       //配置 SysTick 为 1ms 中断一次 
	ESP8266_Init ();                      //初始化WiFi模块使用的接口和外设
	DHT11_Init ();

	
	printf ( "\r\nFire monitoring and alarm system\r\n" );        //打印测试例程提示信息

	
    ESP8266_StaTcpClient_UnvarnishTest ();
	
	
  while ( 1 );
	
	
}


/*********************************************END OF FILE**********************/
