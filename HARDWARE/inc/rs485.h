/*-------------------------------------------------*/
/*         ��γ����STM32F407ZGT6������             */
/*    �Ա���ַ�� http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/

#ifndef __RS485_H
#define __RS485_H	

#include "main.h"	 								  
#include "stdio.h"
#include "stdarg.h"		 
#include "string.h"	

#define RS485_RXBUFF_SIZE  			1024    //������ջ�������С
#define RS485_TXBUFF_SIZE  			1024    //������ջ�������С

extern u8  RS485_RX_BUF[RS485_RXBUFF_SIZE];     //���ջ���
extern u8  RS485_TX_BUF[RS485_TXBUFF_SIZE];     //���ͻ���

extern u16 RS485_RX_STA;   			            //����״̬���

//�շ�ģʽ���ſ���
#define RS485_RX_TX		PFout(11)	            //485����.0,����;1,����.
														 
void RS485_Init(u32 bound);         //485��ʼ������3
void RS485_printf(char* fmt,...);   //485printf����

#endif	   
















