/*-------------------------------------------------*/
/*         ��γ����STM32F407ZGT6������             */
/*    �Ա���ַ�� http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/

#ifndef __IR_H
#define __IR_H 

#include "main.h"   
  
#define IR_in PAin(8)	    //�������������

extern u8  RmtSta;	//״̬ 	  
extern u16 Dval;    //�½���ʱ��������ֵ
extern u32 RmtRec;	//������յ�������	   		    
extern u8  RmtCnt;	//�������µĴ���	

void IR_Init(void);  //����ң�س�ʼ����ʱ��
u8 IR_recive(void);  //����ң���źŲɼ� 

#endif

