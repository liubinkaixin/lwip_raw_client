/*-------------------------------------------------*/
/*         ��γ����STM32F407ZGT6������             */
/*    �Ա���ַ�� http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/

#ifndef _IWDG_H
#define _IWDG_H

#include "main.h"

void IWDG_Init(u8 prer,u16 rlr);//���Ź���ʼ��
void IWDG_Feed(void);           //ι������
#endif
