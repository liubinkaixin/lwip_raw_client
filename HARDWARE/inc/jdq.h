/*-------------------------------------------------*/
/*         ��γ����STM32F407ZGT6������             */
/*    �Ա���ַ�� http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/
#ifndef __JDQ_H
#define __JDQ_H

#include "main.h"   

#define  JDQ_GROUP_EN  RCC_AHB1Periph_GPIOG  //����JDQ���ŵ�IO��

#define  JDQ_GROUP   GPIOG        //����G8���ŵ�IO��
#define  JDQ_PIN     GPIO_Pin_8   //����G8���ŵı��

#define  JDQ_IO      PGout(8)        //G8����JDQ
#define  JDQ_IOin    PGin(8)         //G8����JDQ

void JDQ_Init(void);            //��ʼ��	

#endif
