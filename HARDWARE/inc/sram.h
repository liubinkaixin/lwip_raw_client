/*-------------------------------------------------*/
/*         ��γ����STM32F407ZGT6������             */
/*    �Ա���ַ�� http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/

#ifndef __SRAM_H
#define __SRAM_H	

#include "main.h" 

#define Bank1_SRAM3_ADDR    ((u32)(0x68000000))	
											  
void SRAM_Init(void);        //��ʼ������SRAM��FSMC

#endif

