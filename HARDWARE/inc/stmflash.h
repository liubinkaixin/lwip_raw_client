/*-------------------------------------------------*/
/*         ��γ����STM32F407ZGT6������             */
/*    �Ա���ַ�� http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/

#ifndef __STMFLASH_H__
#define __STMFLASH_H__

#include "stm32f4xx.h" 

#define STM32_FLASH_BASE 0x08000000 	//STM32 FLASH����ʼ��ַ
 
u8 STMFLASH_Write(u32 WriteAddr,u32 *pBuffer,u32 NumToWrite);	//��ָ����ַ��ʼд��ָ�����ȵ�����
u8 STMFLASH_Erase(u8 Sector);                                   //��������                            
void STMFLASH_Read(u32 ReadAddr,u32 *pBuffer,u32 NumToRead) ;   //��ָ����ַ��ʼ����ָ�����ȵ����� 						   
#endif

















