/*-------------------------------------------------*/
/*         超纬电子STM32F407ZGT6开发板             */
/*    淘宝地址： http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/

#ifndef _RGBLED_H
#define _RGBLED_H

#include "main.h"

#define  LED_Bout    PBout(6)	       
#define  LED_Rout    PBout(5)	       
#define  LED_Gout    PBout(4)   

#define  LED_Bin     PBin(6)	       
#define  LED_Rin     PBin(5)	       
#define  LED_Gin     PBin(4)  

void TIM4_PWM_Init(u32 arr,u32 psc);  
void TIM3_PWM_Init(u32 arr,u32 psc);
void BLUE_LED(u8 num);
void RED_LED(u8 num);
void GREEN_LED(u8 num);
void RGB_LED_Init(void);

#endif
