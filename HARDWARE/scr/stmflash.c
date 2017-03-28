/*-------------------------------------------------*/
/*         ��γ����STM32F407ZGT6������             */
/*    �Ա���ַ�� http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/
/*-------------------------------------------------*/
/*        stm32�ڲ�flash�ļ�,�и�����             */
/*-------------------------------------------------*/

/*-------------------------------------------------*/

#include "stmflash.h"

/*-------------------------------------------------*/
/*����������ָ����ַ��ʼд��ָ�����ȵ�����         */
/*��  ����WriteAddr����ʼд��ĵ�ַ                */
/*��  ����pBuffer��д�������                      */
/*��  ����NumToWrite��д������ݸ���               */
/*����ֵ��1������   0����ȷ                        */
/*-------------------------------------------------*/
u8 STMFLASH_Write(u32 WriteAddr,u32 *pBuffer,u32 NumToWrite)	
{ 
	u32 endaddr=0;	
    if(WriteAddr<STM32_FLASH_BASE||WriteAddr%4)return 1;	//�Ƿ���ַ���������0x08000000��ַ�������ֽڶ���
	
	FLASH_Unlock();							//����FLASHдȨ�� 
    FLASH_DataCacheCmd(DISABLE);            //FLASH�����ڼ�,�����ֹ���ݻ���		
	
	endaddr=WriteAddr+NumToWrite;	        //����д��Ľ�����ַ	
	
	while(WriteAddr<endaddr)                //��ʼд����
	{
		if(FLASH_ProgramWord(WriteAddr,*pBuffer)!=FLASH_COMPLETE)    //д������
		{ 
			return 1;	              //д���쳣����1
		}
		WriteAddr+=4;
		pBuffer++;
	} 
	
    FLASH_DataCacheCmd(ENABLE);	      //����,�������ݻ���
	FLASH_Lock();                     //����
	
	return 0;                         //����0.��ʾ��ȷ
}

/*-------------------------------------------------*/
/*���������������� ��д֮ǰҪ����                  */
/*��  ����Sector��Ҫ�������������                 */
/*����ֵ��1������   0����ȷ                        */
/*-------------------------------------------------*/
/*     STM32F407ZGT6-1M��flash������12������       */
/*-------------------------------------------------*/
/*     ����0�� 16KB    FLASH_Sector_0              */
/*     ����1�� 16KB    FLASH_Sector_1              */
/*     ����2�� 16KB    FLASH_Sector_2              */
/*     ����3�� 16KB    FLASH_Sector_3              */
/*     ����4�� 64KB    FLASH_Sector_4              */
/*     ����5�� 128KB   FLASH_Sector_5              */
/*     ����6�� 128KB   FLASH_Sector_6              */
/*     ����7�� 128KB   FLASH_Sector_7              */
/*     ����8�� 128KB   FLASH_Sector_8              */
/*     ����9�� 128KB   FLASH_Sector_9              */
/*     ����10��128KB   FLASH_Sector_10             */
/*     ����11��128KB   FLASH_Sector_11             */
/*-------------------------------------------------*/
u8 STMFLASH_Erase(u8 Sector)   	
{
	FLASH_Status status = FLASH_COMPLETE;

	FLASH_Unlock();				//���� 
    FLASH_DataCacheCmd(DISABLE);//FLASH�����ڼ�,�����ֹ���ݻ���

	status=FLASH_EraseSector(Sector,VoltageRange_3);//VCC=2.7~3.6V֮��!!
	if(status!=FLASH_COMPLETE)
	{	
		return 1;                    //����1
	}
	
	FLASH_DataCacheCmd(ENABLE);	//FLASH��������,�������ݻ���
	FLASH_Lock();               //����
	
	return 0;                   //����0��ȷ
}

/*-------------------------------------------------*/
/*����������ָ����ַ��ʼ����ָ�����ȵ�����         */
/*��  ����ReadAddr����ʼ�����ĵ�ַ                 */
/*��  ����pBuffer������������ָ��                  */
/*��  ����NumToRead�����������ݸ���                */
/*����ֵ��1������   0����ȷ                        */
/*-------------------------------------------------*/
void STMFLASH_Read(u32 ReadAddr,u32 *pBuffer,u32 NumToRead)   	
{
	u32 i;
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=(*(vu32*)ReadAddr);         //��ȡ4���ֽ�.
		ReadAddr+=4;                           //ƫ��4���ֽ�.	
	}
}











