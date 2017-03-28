/*-------------------------------------------------*/
/*         超纬电子STM32F407ZGT6开发板             */
/*    淘宝地址： http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/

#ifndef _IWDG_H
#define _IWDG_H

#include "main.h"

void IWDG_Init(u8 prer,u16 rlr);//看门狗初始化
void IWDG_Feed(void);           //喂狗函数
#endif
