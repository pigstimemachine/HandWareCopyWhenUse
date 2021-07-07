#include "stm32f10x_it.h"
#include <stdio.h>
#include "bsp_usart1.h"
#include "bsp_usart2.h"
#include "wifi_config.h"
#include <string.h>
#include <stdlib.h>
#include "stm32f10x.h"
#include "bsp_usart1.h"
#include "wifi_config.h"
#include "bsp_SysTick.h"
 

//extern  char buffer[64];
//extern void TimingDelay_Decrement(void);
//extern void USART2_printf(USART_TypeDef* USARTx, char *Data,...);


/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
//	  break;
	  
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
	TimingDelay_Decrement();	
}


/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/
/**
  * @brief  This function handles USART1 Handler.
  * @param  None
  * @retval None
  */
//void USART1_IRQHandler( void )
//{	
//	char ch;
//	u8 length;
//	
//	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)//读数据寄存器非空
//	{
//		USART_ClearITPendingBit(USART1,USART_IT_RXNE); //清除中断标志.
//		ch  = USART_ReceiveData( USART1 );
//		
//		if( length < 63 )                       //预留1个字节写结束符
//		{
//				buffer [ length ++ ]  = ch;
//		}
//	}
//	 	 
//	if ( USART_GetITStatus( USART1, USART_IT_IDLE ) == SET )                                         //数据帧接收完毕
//	{
//		USART_ClearFlag(USART1,USART_FLAG_ORE);  //读SR       清除IDLE
//		USART_ReceiveData(USART1); //读DR 
//        strEsp8266_Fram_Record .InfBit .FramFinishFlag = 1;
//		
//		ch = USART_ReceiveData( USART1 );                                                              //由软件序列清除中断标志位(先读USART_SR，然后读USART_DR)
//		if( length < 63 )                       //预留1个字节写结束符
//		{
//				buffer [ length ++ ]  = ch;
//		}
//		buffer[ length ] = '\0';
//		length=0;
//    }


//}


/**
  * @brief  This function handles USART2 Handler.
  * @param  None
  * @retval None
  */
void USART2_IRQHandler( void )
{	
	char ch;
//	u16 i;
	
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)//读数据寄存器非空
	{
//		USART_ClearITPendingBit(USART2,USART_IT_RXNE); //清除中断标志.
		ch  = USART_ReceiveData( USART2 );
		
		if( strEsp8266_Fram_Record .InfBit .FramLength < ( RX_BUF_MAX_LEN - 1 ) )                       //预留1个字节写结束符
		{
				strEsp8266_Fram_Record .Data_RX_BUF [ strEsp8266_Fram_Record .InfBit .FramLength ++ ]  = ch;
		}
	}
	 	 
	if ( USART_GetITStatus( USART2, USART_IT_IDLE ) == SET )                                         //数据帧接收完毕
	{
		USART_ClearFlag(USART2,USART_FLAG_ORE);  //读SR 
		USART_ReceiveData(USART2); //读DR 
        strEsp8266_Fram_Record .InfBit .FramFinishFlag = 1;
		
		ch = USART_ReceiveData( USART2 );                                                              //由软件序列清除中断标志位(先读USART_SR，然后读USART_DR)
//		//读SR后读DR清除Idle
//		i = USART2->SR;
//		i = USART2->DR;

  }


}


/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
