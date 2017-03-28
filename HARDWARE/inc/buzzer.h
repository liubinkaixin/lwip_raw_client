/*-------------------------------------------------*/
/*          ��γ����STM32F407ZGT6������            */
/*    �Ա���ַ�� http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/
#ifndef __BUZZER_H
#define __BUZZER_H

#include "main.h"   

#define  BUZZER_GROUP_EN   RCC_AHB1Periph_GPIOG   //���Ʒ��������ŵ�IO��

#define  BUZZER_GROUP      GPIOG        //���Ʒ��������ŵ�IO��
#define  BUZZER_PIN        GPIO_Pin_9   //���Ʒ��������ŵı��

#define  BUZZE         PGout(9)	        // PG9���Ʒ�����
#define  BUZZEin       PGin(9)	        // PG9���Ʒ�����

void Buzzer_Init(void);

#endif
