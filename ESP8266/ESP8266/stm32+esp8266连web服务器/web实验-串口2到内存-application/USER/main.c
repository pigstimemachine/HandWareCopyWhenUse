#include <string.h>
#include <stdlib.h>
#include "stm32f10x.h"
#include "bsp_gpio.h"
#include "bsp_usart1.h"
#include "bsp_usart2.h"
#include "wifi_config.h"
#include "wifi_function.h"
#include "bsp_SysTick.h"
//#include <cJSON.h> 
 

//void parseJson_array(char * array,char ar[][2])
//{
//	cJSON * pJson = NULL;
//	cJSON *pSub   = NULL;
//	cJSON *pSubSub= NULL;
//	int i,SIZE;
//	if (NULL == array)
//	{
//		return;
//	}
//	pJson = cJSON_Parse(array);
//	if (NULL == pJson)
//	{
//		// parse faild, return
//		return;
//	}
//	SIZE = cJSON_GetArraySize(pJson);
//	for (i = 0; i < SIZE; i++)
//	{
//		pSub = cJSON_GetArrayItem(pJson, i);//item为元素在数组中序列
//		if (NULL == pJson)
//		{
//			// parse faild, return
//			return;
//		}

//		pSubSub = cJSON_GetObjectItem(pSub, "seatId");
//		if (NULL == pSubSub)
//		{
//			//get number from json faild
//		}
//		ar[i][0] = pSubSub->valueint;
//		pSubSub = cJSON_GetObjectItem(pSub, "seatState");
//		if (NULL == pSubSub)
//		{
//			// get bool from json faild
//		}
//		ar[i][1] = pSubSub->valueint;
//////		printf("seatState : %d\n", pSubSub->valueint);
////		pSubSub = cJSON_GetObjectItem(pSub, "usrId");
////		if (NULL == pSubSub)
////		{
////			// get bool from json faild
////		}
////		ar[i][2] = pSubSub->valueint;
//////		printf("usrId : %d\n", pSubSub->valueint);
//	}
//	cJSON_Delete(pSub);	
//	return;
//}

int main(void)
{
	u8 i;
//	u16 num=0;
//	char seat[30][2]={0};
//	char *symbol="[{";
//	char *address=NULL;
	/* 初始化 */
	WiFi_RST_INIT();
    WiFi_USART1_INIT(); 
	WiFi_USART2_INIT(); 
	WiFi_NVIC_INIT();                                                                 
	SysTick_Init();//配置 SysTick 为 1ms 中断一次 
	                                                               
	PC_Usart ("\r\n 测试开始\r\n");                            //打印测试例程提示信息
	ESP8266_init();
	while(1)
	{
			Delay_ms(2000);
			Delay_ms(1000);
//			num++;
//			PC_Usart ("\r\n%d\r\n",num);
			Web_To_ESP();
			Delay_ms(500);
//			address=strstr(strEsp8266_Fram_Record .Data_RX_BUF,symbol);
//			parseJson_array(address,seat);
//			for(i=0;i<30;i++)	
//					PC_Usart ("%d",seat[i][1]);//逐个发送数据到串口1			
	}

}
	


