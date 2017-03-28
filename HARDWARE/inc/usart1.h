/*-------------------------------------------------*/
/*         超纬电子STM32F407ZGT6开发板             */
/*    淘宝地址： http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/

#ifndef __USART_H
#define __USART_H

#include "stdio.h"	
#include "main.h" 

#define USART1_RXBUFF_SIZE      1024               //定义接收缓冲区大小1024字节
	  	
extern  u8  USART1_RX_BUF[USART1_RXBUFF_SIZE];     //接收缓冲数组 
extern  u16 USART1_RX_STA;         		           //接收状态标记	

void   Usart1_init(u32 bound);                     //串口1初始化函数
#endif


