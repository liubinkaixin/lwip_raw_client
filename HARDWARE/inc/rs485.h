/*-------------------------------------------------*/
/*         超纬电子STM32F407ZGT6开发板             */
/*    淘宝地址： http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/

#ifndef __RS485_H
#define __RS485_H	

#include "main.h"	 								  
#include "stdio.h"
#include "stdarg.h"		 
#include "string.h"	

#define RS485_RXBUFF_SIZE  			1024    //定义接收缓冲区大小
#define RS485_TXBUFF_SIZE  			1024    //定义接收缓冲区大小

extern u8  RS485_RX_BUF[RS485_RXBUFF_SIZE];     //接收缓冲
extern u8  RS485_TX_BUF[RS485_TXBUFF_SIZE];     //发送缓冲

extern u16 RS485_RX_STA;   			            //接收状态标记

//收发模式引脚控制
#define RS485_RX_TX		PFout(11)	            //485控制.0,接收;1,发送.
														 
void RS485_Init(u32 bound);         //485初始化串口3
void RS485_printf(char* fmt,...);   //485printf函数

#endif	   
















