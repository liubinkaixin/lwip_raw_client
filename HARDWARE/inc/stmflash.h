/*-------------------------------------------------*/
/*         超纬电子STM32F407ZGT6开发板             */
/*    淘宝地址： http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/

#ifndef __STMFLASH_H__
#define __STMFLASH_H__

#include "stm32f4xx.h" 

#define STM32_FLASH_BASE 0x08000000 	//STM32 FLASH的起始地址
 
u8 STMFLASH_Write(u32 WriteAddr,u32 *pBuffer,u32 NumToWrite);	//从指定地址开始写入指定长度的数据
u8 STMFLASH_Erase(u8 Sector);                                   //擦除扇区                            
void STMFLASH_Read(u32 ReadAddr,u32 *pBuffer,u32 NumToRead) ;   //从指定地址开始读出指定长度的数据 						   
#endif

















