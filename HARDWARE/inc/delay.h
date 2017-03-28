/*-------------------------------------------------*/
/*         超纬电子STM32F407ZGT6开发板             */
/*    淘宝地址： http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/

#ifndef __DELAY_H
#define __DELAY_H 

#include "main.h"	  
 
void delay_init(void);         //延时初始化函数
void delay_ms(u32 ms);         //延时毫秒函数
void delay_us(u32 us);         //延时微秒函数
void delay_ns(u32 ns);         //延时纳秒函数

#endif





























