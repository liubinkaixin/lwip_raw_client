/*-------------------------------------------------*/
/*         ��γ����STM32F407ZGT6������             */
/*    �Ա���ַ�� http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/
#ifndef __LED_H
#define __LED_H

#include "main.h"   

#define  LED_GROUP  RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC   //����LED���ŵ�IO��

#define  LED8_GROUP   GPIOB        //����D8���ŵ�IO��
#define  LED8_PIN     GPIO_Pin_0   //����D8���ŵı��

#define  LED9_GROUP   GPIOB        //����D9���ŵ�IO��
#define  LED9_PIN     GPIO_Pin_1   //����D9���ŵı��

#define  LED10_GROUP  GPIOC        //����D10���ŵ�IO��
#define  LED10_PIN    GPIO_Pin_13  //����D10���ŵı��

#define  LED_D8    PBout(0)	       // PB0����D8
#define  LED_D9    PBout(1)	       // PB1����D9
#define  LED_D10   PCout(13)       // PC13����D10

#define  LED_D8in    PBin(0)	    // PB0����D8
#define  LED_D9in    PBin(1)	    // PB1����D9
#define  LED_D10in   PCin(13)       // PC13����D10

void LED_Init(void);            //��ʼ��	
void LED_AllOn(void);           //��������LED
void LED_AllOff(void);          //Ϩ������LED

#endif
