/*-------------------------------------------------*/
/*         ��γ����STM32F407ZGT6������             */
/*    �Ա���ַ�� http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/

#ifndef __IRTX_H
#define __IRTX_H 

#include "main.h"   
  
#define IR_SEND   PCout(7)	          //���ⷢ�͵����ݵ�����
    
void  IR_SentInit(u32 arr,u32 psc);   //���ⷢ�ͳ�ʼ�� 
void  Sent_0(void);                   //����һ��������λ0
void  Sent_1(void);                   //����һ��������λ1
void  IR_Send(u8 data);               //���ͺ����ź� 

#endif

