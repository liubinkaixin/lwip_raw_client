/*-------------------------------------------------*/
/*         超纬电子STM32F407ZGT6开发板             */
/*    淘宝地址： http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/
/*-------------------------------------------------*/
/*        stm32内部flash文件,有个函数             */
/*-------------------------------------------------*/

/*-------------------------------------------------*/

#include "stmflash.h"

/*-------------------------------------------------*/
/*函数名：从指定地址开始写入指定长度的数据         */
/*参  数：WriteAddr：开始写入的地址                */
/*参  数：pBuffer：写入的数据                      */
/*参  数：NumToWrite：写入的数据个数               */
/*返回值：1：错误   0：正确                        */
/*-------------------------------------------------*/
u8 STMFLASH_Write(u32 WriteAddr,u32 *pBuffer,u32 NumToWrite)	
{ 
	u32 endaddr=0;	
    if(WriteAddr<STM32_FLASH_BASE||WriteAddr%4)return 1;	//非法地址，必须大于0x08000000地址，且四字节对齐
	
	FLASH_Unlock();							//解锁FLASH写权限 
    FLASH_DataCacheCmd(DISABLE);            //FLASH擦除期间,必须禁止数据缓存		
	
	endaddr=WriteAddr+NumToWrite;	        //计算写入的结束地址	
	
	while(WriteAddr<endaddr)                //开始写数据
	{
		if(FLASH_ProgramWord(WriteAddr,*pBuffer)!=FLASH_COMPLETE)    //写入数据
		{ 
			return 1;	              //写入异常返回1
		}
		WriteAddr+=4;
		pBuffer++;
	} 
	
    FLASH_DataCacheCmd(ENABLE);	      //结束,开启数据缓存
	FLASH_Lock();                     //上锁
	
	return 0;                         //返回0.表示正确
}

/*-------------------------------------------------*/
/*函数名：擦除扇区 ，写之前要擦除                  */
/*参  数：Sector：要擦除的扇区编号                 */
/*返回值：1：错误   0：正确                        */
/*-------------------------------------------------*/
/*     STM32F407ZGT6-1M的flash，共计12个扇区       */
/*-------------------------------------------------*/
/*     扇区0： 16KB    FLASH_Sector_0              */
/*     扇区1： 16KB    FLASH_Sector_1              */
/*     扇区2： 16KB    FLASH_Sector_2              */
/*     扇区3： 16KB    FLASH_Sector_3              */
/*     扇区4： 64KB    FLASH_Sector_4              */
/*     扇区5： 128KB   FLASH_Sector_5              */
/*     扇区6： 128KB   FLASH_Sector_6              */
/*     扇区7： 128KB   FLASH_Sector_7              */
/*     扇区8： 128KB   FLASH_Sector_8              */
/*     扇区9： 128KB   FLASH_Sector_9              */
/*     扇区10：128KB   FLASH_Sector_10             */
/*     扇区11：128KB   FLASH_Sector_11             */
/*-------------------------------------------------*/
u8 STMFLASH_Erase(u8 Sector)   	
{
	FLASH_Status status = FLASH_COMPLETE;

	FLASH_Unlock();				//解锁 
    FLASH_DataCacheCmd(DISABLE);//FLASH擦除期间,必须禁止数据缓存

	status=FLASH_EraseSector(Sector,VoltageRange_3);//VCC=2.7~3.6V之间!!
	if(status!=FLASH_COMPLETE)
	{	
		return 1;                    //返回1
	}
	
	FLASH_DataCacheCmd(ENABLE);	//FLASH擦除结束,开启数据缓存
	FLASH_Lock();               //上锁
	
	return 0;                   //返回0正确
}

/*-------------------------------------------------*/
/*函数名：从指定地址开始读出指定长度的数据         */
/*参  数：ReadAddr：开始读出的地址                 */
/*参  数：pBuffer：读出的数据指针                  */
/*参  数：NumToRead：读出的数据个数                */
/*返回值：1：错误   0：正确                        */
/*-------------------------------------------------*/
void STMFLASH_Read(u32 ReadAddr,u32 *pBuffer,u32 NumToRead)   	
{
	u32 i;
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=(*(vu32*)ReadAddr);         //读取4个字节.
		ReadAddr+=4;                           //偏移4个字节.	
	}
}











