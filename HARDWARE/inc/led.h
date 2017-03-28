/*-------------------------------------------------*/
/*         超纬电子STM32F407ZGT6开发板             */
/*    淘宝地址： http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/
#ifndef __LED_H
#define __LED_H

#include "main.h"   

#define  LED_GROUP  RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC   //控制LED引脚的IO组

#define  LED8_GROUP   GPIOB        //控制D8引脚的IO组
#define  LED8_PIN     GPIO_Pin_0   //控制D8引脚的编号

#define  LED9_GROUP   GPIOB        //控制D9引脚的IO组
#define  LED9_PIN     GPIO_Pin_1   //控制D9引脚的编号

#define  LED10_GROUP  GPIOC        //控制D10引脚的IO组
#define  LED10_PIN    GPIO_Pin_13  //控制D10引脚的编号

#define  LED_D8    PBout(0)	       // PB0控制D8
#define  LED_D9    PBout(1)	       // PB1控制D9
#define  LED_D10   PCout(13)       // PC13控制D10

#define  LED_D8in    PBin(0)	    // PB0控制D8
#define  LED_D9in    PBin(1)	    // PB1控制D9
#define  LED_D10in   PCin(13)       // PC13控制D10

void LED_Init(void);            //初始化	
void LED_AllOn(void);           //点亮所有LED
void LED_AllOff(void);          //熄灭所有LED

#endif
