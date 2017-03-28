/*-------------------------------------------------*/
/*         超纬电子STM32F407ZGT6开发板             */
/*    淘宝地址： http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/

#ifndef __USART2_H
#define __USART2_H

#include "stdio.h"
#include "stdarg.h"		 
#include "string.h"
#include "main.h" 

#define USART2_RXBUFF_SIZE      1024               //定义接收缓冲区大小1024字节
#define USART2_TXBUFF_SIZE      1024               //定义接收缓冲区大小1024字节

extern  u8  USART2_RX_BUF[USART2_RXBUFF_SIZE];     //接收缓冲数组 
extern  u16 USART2_RX_STA;         		           //接收状态标记	

extern  u8  USART2_TX_BUF[USART2_TXBUFF_SIZE];     //发送缓冲数组 

void   Usart2_init(u32 bound);                     //串口2初始化函数
void u2_printf(char* fmt,...) ;                    //串口2printf函数

#endif


