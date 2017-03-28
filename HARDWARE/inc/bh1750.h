/*-------------------------------------------------*/
/*         超纬电子STM32F407ZGT6开发板             */
/*    淘宝地址： http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/

#ifndef __BH1750_H
#define __BH1750_H

#include "main.h"
#include "stdio.h"

#define	  SlaveAddress   0x46

extern u8 BUF[8]; 

void Write_BH1750(u8 REG_Address);   //写BH1750
void Read_BH1750(void);              //读BH1750
void Start_BH1750(void);             //开始采集数据

#endif
