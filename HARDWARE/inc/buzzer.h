/*-------------------------------------------------*/
/*          超纬电子STM32F407ZGT6开发板            */
/*    淘宝地址： http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/
#ifndef __BUZZER_H
#define __BUZZER_H

#include "main.h"   

#define  BUZZER_GROUP_EN   RCC_AHB1Periph_GPIOG   //控制蜂鸣器引脚的IO组

#define  BUZZER_GROUP      GPIOG        //控制蜂鸣器引脚的IO组
#define  BUZZER_PIN        GPIO_Pin_9   //控制蜂鸣器引脚的编号

#define  BUZZE         PGout(9)	        // PG9控制蜂鸣器
#define  BUZZEin       PGin(9)	        // PG9控制蜂鸣器

void Buzzer_Init(void);

#endif
