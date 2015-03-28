#ifndef __BSP_USART_H
#define __BSP_USART_H

#include <stdio.h>
#include "stm32f4xx.h"


/* bmp Define ----------------------------------------------------------------*/
/* <1=> Enable, <0=> Disable */
#define _USE_DEBUG	      1 
#if _USE_DEBUG
	#define DbgPrintf(FORMAT, ...)      printf(FORMAT, ##__VA_ARGS__)
#else
	#define DbgPrintf(FORMAT, ...)     ;
#endif
/* bmp Define ----------------------------------------------------------------*/

/** @defgroup Exported_Types
  * @{
  */
#define COMn                             3	/*定义三个串口*/
typedef enum 
{
  COM1 = 0,
  COM2 = 1,
  COM3 = 2
} COM_TypeDef; 

/** @defgroup Exported_Functions
  * @{
  */
void DBG_InitUart(COM_TypeDef COM,u32 UsartBaudRate);
void DBG_USART_SendByte(USART_TypeDef* USARTx,uint8_t byte);
void DBG_USART_SendStr(USART_TypeDef* USARTx,const char * data);
void DBG_USART_SendArray(USART_TypeDef* USARTx, u8 *data,u16 len);
uint8_t DBG_USART_ReceiveByte(USART_TypeDef* USARTx);



#endif


