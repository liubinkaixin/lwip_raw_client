/*-------------------------------------------------*/
/*         超纬电子STM32F407ZGT6开发板             */
/*    淘宝地址： http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/

#ifndef __W25Q128_H
#define __W25Q128_H

#include "main.h"  
   
#define	W25Q128_CS 		PGout(15)  		//W25Q128的片选信号

//指令表
#define W25X128_WriteEnable		0x06 
#define W25X128_WriteDisable		0x04 
#define W25X128_ReadStatusReg		0x05 
#define W25X128_WriteStatusReg		0x01 
#define W25X128_ReadData			0x03 
#define W25X128_FastReadData		0x0B 
#define W25X128_FastReadDual		0x3B 
#define W25X128_PageProgram		0x02 
#define W25X128_BlockErase			0xD8 
#define W25X128_SectorErase		0x20 
#define W25X128_ChipErase			0xC7 
#define W25X128_PowerDown			0xB9 
#define W25X128_ReleasePowerDown	0xAB 
#define W25X128_DeviceID			0xAB 
#define W25X128_ManufactDeviceID	0x90 
#define W25X128_JedecDeviceID		0x9F 

void W25Q128_Init(void);                //初始化
u8	 W25Q128_ReadSR(void);        		//读取状态寄存器 
void W25Q128_Write_SR(u8 sr);  			//写状态寄存器
void W25Q128_Write_Enable(void);  		//写使能 
void W25Q128_Write_Disable(void);		//写保护
void W25Q128_Write_NoCheck(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);
void W25Q128_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead);   //读取flash
void W25Q128_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);//写入flash
void W25Q128_Erase_Chip(void);    	  	    //整片擦除
void W25Q128_Erase_Sector(u32 Dst_Addr);	//扇区擦除
void W25Q128_Wait_Busy(void);           	//等待空闲
#endif
















