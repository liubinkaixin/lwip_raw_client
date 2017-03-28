/*-------------------------------------------------*/
/*          超纬电子STM32F407ZGT6开发板            */
/*    淘宝地址： http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/
#ifndef __EXTI_H
#define __EXIT_H	

#include "main.h"  	

#define  KEY1_EXTI_GPOUP   EXTI_PortSourceGPIOA    //KEY1按键对应的中断线组
#define  KEY1_EXTI_SOURCE  EXTI_PinSource0         //KEY1按键对应的中断源
#define  KEY1_EXTI_LINE    EXTI_Line0              //KEY1按键对应的中断线
#define  KEY1_EXTI_IRQ     EXTI0_IRQn              //KEY1所在中断
 
#define  KEY2_EXTI_GPOUP   EXTI_PortSourceGPIOE    //KEY2按键对应的中断线组
#define  KEY2_EXTI_SOURCE  EXTI_PinSource2         //KEY2按键对应的中断源
#define  KEY2_EXTI_LINE    EXTI_Line2              //KEY2按键对应的中断线
#define  KEY2_EXTI_IRQ     EXTI2_IRQn              //KEY2所在中断

#define  KEY3_EXTI_GPOUP   EXTI_PortSourceGPIOE    //KEY3按键对应的中断线组
#define  KEY3_EXTI_SOURCE  EXTI_PinSource3         //KEY3按键对应的中断源
#define  KEY3_EXTI_LINE    EXTI_Line3              //KEY3按键对应的中断线
#define  KEY3_EXTI_IRQ     EXTI3_IRQn              //KEY3所在中断

#define  KEY4_EXTI_GPOUP   EXTI_PortSourceGPIOE    //KEY4按键对应的中断线组
#define  KEY4_EXTI_SOURCE  EXTI_PinSource4         //KEY4按键对应的中断源
#define  KEY4_EXTI_LINE    EXTI_Line4              //KEY4按键对应的中断线
#define  KEY4_EXTI_IRQ     EXTI4_IRQn              //KEY4所在中断
 
void KEY_EXTI_Init(void);	//外部中断初始化		 					    
#endif

























