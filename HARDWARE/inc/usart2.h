/*-------------------------------------------------*/
/*         ��γ����STM32F407ZGT6������             */
/*    �Ա���ַ�� http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/

#ifndef __USART2_H
#define __USART2_H

#include "stdio.h"
#include "stdarg.h"		 
#include "string.h"
#include "main.h" 

#define USART2_RXBUFF_SIZE      1024               //������ջ�������С1024�ֽ�
#define USART2_TXBUFF_SIZE      1024               //������ջ�������С1024�ֽ�

extern  u8  USART2_RX_BUF[USART2_RXBUFF_SIZE];     //���ջ������� 
extern  u16 USART2_RX_STA;         		           //����״̬���	

extern  u8  USART2_TX_BUF[USART2_TXBUFF_SIZE];     //���ͻ������� 

void   Usart2_init(u32 bound);                     //����2��ʼ������
void u2_printf(char* fmt,...) ;                    //����2printf����

#endif


