/*-------------------------------------------------*/
/*          ��γ����STM32F407ZGT6������            */
/*    �Ա���ַ�� http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/
#ifndef __EXTI_H
#define __EXIT_H	

#include "main.h"  	

#define  KEY1_EXTI_GPOUP   EXTI_PortSourceGPIOA    //KEY1������Ӧ���ж�����
#define  KEY1_EXTI_SOURCE  EXTI_PinSource0         //KEY1������Ӧ���ж�Դ
#define  KEY1_EXTI_LINE    EXTI_Line0              //KEY1������Ӧ���ж���
#define  KEY1_EXTI_IRQ     EXTI0_IRQn              //KEY1�����ж�
 
#define  KEY2_EXTI_GPOUP   EXTI_PortSourceGPIOE    //KEY2������Ӧ���ж�����
#define  KEY2_EXTI_SOURCE  EXTI_PinSource2         //KEY2������Ӧ���ж�Դ
#define  KEY2_EXTI_LINE    EXTI_Line2              //KEY2������Ӧ���ж���
#define  KEY2_EXTI_IRQ     EXTI2_IRQn              //KEY2�����ж�

#define  KEY3_EXTI_GPOUP   EXTI_PortSourceGPIOE    //KEY3������Ӧ���ж�����
#define  KEY3_EXTI_SOURCE  EXTI_PinSource3         //KEY3������Ӧ���ж�Դ
#define  KEY3_EXTI_LINE    EXTI_Line3              //KEY3������Ӧ���ж���
#define  KEY3_EXTI_IRQ     EXTI3_IRQn              //KEY3�����ж�

#define  KEY4_EXTI_GPOUP   EXTI_PortSourceGPIOE    //KEY4������Ӧ���ж�����
#define  KEY4_EXTI_SOURCE  EXTI_PinSource4         //KEY4������Ӧ���ж�Դ
#define  KEY4_EXTI_LINE    EXTI_Line4              //KEY4������Ӧ���ж���
#define  KEY4_EXTI_IRQ     EXTI4_IRQn              //KEY4�����ж�
 
void KEY_EXTI_Init(void);	//�ⲿ�жϳ�ʼ��		 					    
#endif

























