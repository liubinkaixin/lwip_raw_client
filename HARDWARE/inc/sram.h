/*-------------------------------------------------*/
/*         超纬电子STM32F407ZGT6开发板             */
/*    淘宝地址： http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/

#ifndef __SRAM_H
#define __SRAM_H	

#include "main.h" 

#define Bank1_SRAM3_ADDR    ((u32)(0x68000000))	
											  
void SRAM_Init(void);        //初始化控制SRAM的FSMC

#endif

