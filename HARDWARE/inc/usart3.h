/*-------------------------------------------------*/
/*         超纬电子STM32F407ZGT6开发板             */
/*    淘宝地址： http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/

#ifndef __USART3_H
#define __USART3_H	

#include "main.h"	 								  
#include "stdio.h"
#include "stdarg.h"		 
#include "string.h"	

#define USART3_RXBUFF_SIZE  			1024    //定义接收缓冲区大小
#define USART3_TXBUFF_SIZE  			1024    //定义接收缓冲区大小

extern u8  USART3_RX_BUF[USART3_RXBUFF_SIZE];     //接收缓冲
extern u8  USART3_TX_BUF[USART3_TXBUFF_SIZE];     //发送缓冲

extern u16 USART3_RX_STA;   			          //接收状态标记
														 
void Usart3_init(u32 bound);         //初始化串口3
void u3_printf(char* fmt,...);      //串口3printf函数

#endif	   
















