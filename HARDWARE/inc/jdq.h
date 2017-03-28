/*-------------------------------------------------*/
/*         超纬电子STM32F407ZGT6开发板             */
/*    淘宝地址： http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/
#ifndef __JDQ_H
#define __JDQ_H

#include "main.h"   

#define  JDQ_GROUP_EN  RCC_AHB1Periph_GPIOG  //控制JDQ引脚的IO组

#define  JDQ_GROUP   GPIOG        //控制G8引脚的IO组
#define  JDQ_PIN     GPIO_Pin_8   //控制G8引脚的编号

#define  JDQ_IO      PGout(8)        //G8控制JDQ
#define  JDQ_IOin    PGin(8)         //G8控制JDQ

void JDQ_Init(void);            //初始化	

#endif
