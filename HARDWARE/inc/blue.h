/*-------------------------------------------------*/
/*         ��γ����STM32F407ZGT6������             */
/*    �Ա���ַ�� http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/
#ifndef __BLUE_H
#define __BLUE_H

#include "main.h"   

#define   BLUE_KEY    PBout(15)   //��������ģ���KEY�ţ��˳����ǽ���AT����ģʽ

void Blue_init(void);  //��ʼ������ 
void Blue_InAT(void);  //��������ATģʽ 
u8 Blue_QuitAT(void);  //�����˳�ATģʽ
u8 Blue_SendCmd_OK(u8 *cmd, u16 timeout);//����ATָ��жϻظ�OK
u8 Blue_Setup(void);   //��������ģ��

#endif
