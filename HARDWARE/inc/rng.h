/*-------------------------------------------------*/
/*         超纬电子STM32F407ZGT6开发板             */
/*    淘宝地址： http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/

#ifndef __RNG_H
#define __RNG_H	 

#include "main.h" 
	
u8  RNG_Init(void);			               //RNG初始化 
u32 RNG_Get_RandomNum(void);               //得到随机数

#endif

















