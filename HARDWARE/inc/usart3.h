/*-------------------------------------------------*/
/*         ��γ����STM32F407ZGT6������             */
/*    �Ա���ַ�� http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/

#ifndef __USART3_H
#define __USART3_H	

#include "main.h"	 								  
#include "stdio.h"
#include "stdarg.h"		 
#include "string.h"	

#define USART3_RXBUFF_SIZE  			1024    //������ջ�������С
#define USART3_TXBUFF_SIZE  			1024    //������ջ�������С

extern u8  USART3_RX_BUF[USART3_RXBUFF_SIZE];     //���ջ���
extern u8  USART3_TX_BUF[USART3_TXBUFF_SIZE];     //���ͻ���

extern u16 USART3_RX_STA;   			          //����״̬���
														 
void Usart3_init(u32 bound);         //��ʼ������3
void u3_printf(char* fmt,...);      //����3printf����

#endif	   
















