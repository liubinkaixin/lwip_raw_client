/*-------------------------------------------------*/
/*         ��γ����STM32F407ZGT6������             */
/*    �Ա���ַ�� http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/
#ifndef __SPI1_H
#define __SPI1_H

#include "main.h"

void SPI1_Init(void);			   //��ʼ��SPI1��
void SPI1_SetSpeed(u8 SpeedSet);   //����SPI1�ٶ�   
u8   SPI1_ReadWriteByte(u8 TxData);//SPI1���߶�дһ���ֽ�
		 
#endif

