/*-------------------------------------------------*/
/*         超纬电子STM32F407ZGT6开发板             */
/*    淘宝地址： http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/

#ifndef __IRTX_H
#define __IRTX_H 

#include "main.h"   
  
#define IR_SEND   PCout(7)	          //红外发送的数据的引脚
    
void  IR_SentInit(u32 arr,u32 psc);   //红外发送初始化 
void  Sent_0(void);                   //发送一个二进制位0
void  Sent_1(void);                   //发送一个二进制位1
void  IR_Send(u8 data);               //发送红外信号 

#endif

