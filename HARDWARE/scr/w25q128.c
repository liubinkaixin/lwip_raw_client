/*-------------------------------------------------*/
/*         ��γ����STM32F407ZGT6������             */
/*    �Ա���ַ�� http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/
/*-------------------------------------------------*/
/*            W25Q128Դ�ļ�,�и�����               */
/*              4KbytesΪһ��Sector                */
/*               16������Ϊ1��Block                */
/*      ����Ϊ16M�ֽ�,����128��Block,4096��Sector  */
/*-------------------------------------------------*/
/*-------------------------------------------------*/

#include "w25q128.h" 
#include "spi1.h"
#include "delay.h"	    
 
/*--------------------------------*----------------*/
/*����������ʼ��SPI FLASH��IO��                    */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/													 
void W25Q128_Init(void)
{ 
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);//ʹ��GPIOGʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;           //PG15
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;        //���
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;       //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;   //100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;         //����
	GPIO_Init(GPIOG, &GPIO_InitStructure);               //��ʼ��

	W25Q128_CS=1;			                 //SPI FLASH��ѡ��
	SPI1_Init();		   			         //��ʼ��SPI
	SPI1_SetSpeed(SPI_BaudRatePrescaler_2);  //����Ϊ42Mʱ��,����ģʽ 
}

/*-------------------------------------------------*/
/*����������ȡW25Q128��״̬�Ĵ���                  */
/*��  ������                                       */
/*����ֵ��״̬                                     */
/*-------------------------------------------------*/
u8 W25Q128_ReadSR(void)   
{  
	u8 byte=0;                            
	W25Q128_CS=0;                              //ʹ������   
	SPI1_ReadWriteByte(W25X128_ReadStatusReg);    //���Ͷ�ȡ״̬�Ĵ�������    
	byte=SPI1_ReadWriteByte(0Xff);             //��ȡһ���ֽ�  
	W25Q128_CS=1;                              //ȡ��Ƭѡ     
	return byte;                               //����״̬
} 

/*-------------------------------------------------*/
/*��������дW25Q128״̬�Ĵ���                      */
/*��  ����sr��д��ֵ                               */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void W25Q128_Write_SR(u8 sr)   
{   
	W25Q128_CS=0;                              //ʹ������   
	SPI1_ReadWriteByte(W25X128_WriteStatusReg);   //����дȡ״̬�Ĵ�������    
	SPI1_ReadWriteByte(sr);                    //д��һ���ֽ�  
	W25Q128_CS=1;                              //ȡ��Ƭѡ     	      
}   

/*-------------------------------------------------*/
/*��������W25Q128дʹ��                            */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/  
void W25Q128_Write_Enable(void)   
{
	W25Q128_CS=0;                            //ʹ������   
    SPI1_ReadWriteByte(W25X128_WriteEnable);    //����дʹ��  
	W25Q128_CS=1;                            //ȡ��Ƭѡ     	      
} 

/*-------------------------------------------------*/
/*��������W25Q128д��ֹ                            */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/ 
void W25Q128_Write_Disable(void)   
{  
	W25Q128_CS=0;                            //ʹ������   
    SPI1_ReadWriteByte(W25X128_WriteDisable);   //����д��ָֹ��    
	W25Q128_CS=1;                            //ȡ��Ƭѡ     	      
} 	

/*-------------------------------------------------*/
/*����������ȡFLASH                                */
/*��  ����pBuffer:���ݴ洢��                       */
/*��  ����ReadAddr:��ʼ��ȡ�ĵ�ַ                  */
/*��  ����NumByteToRead:Ҫ��ȡ���ֽ���             */
/*����ֵ����                                       */
/*-------------------------------------------------*/ 
void W25Q128_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead)   
{ 
 	u16 i;   										    
	W25Q128_CS=0;                              //ʹ������   
    SPI1_ReadWriteByte(W25X128_ReadData);         //���Ͷ�ȡ����   
    SPI1_ReadWriteByte((u8)((ReadAddr)>>16));  //����24bit��ַ ����3�η���   
    SPI1_ReadWriteByte((u8)((ReadAddr)>>8));   
    SPI1_ReadWriteByte((u8)ReadAddr);           
    
	for(i=0;i<NumByteToRead;i++)               //����forѭ����ȡ����
	{ 
        pBuffer[i]=SPI1_ReadWriteByte(0XFF);   //ѭ����ȡ  
    }
	W25Q128_CS=1;                              //ȡ��Ƭѡ 				    	      
} 

/*-------------------------------------------------*/
/*��������д������256���ֽڵ�����                  */
/*��  ����pBuffer:���ݴ洢��                       */
/*��  ����WriteAddr:��ʼд��ĵ�ַ                  */
/*��  ����NumByteToRead:Ҫд����ֽ���             */
/*����ֵ����                                       */
/*-------------------------------------------------*/ 
void W25Q128_Write_Page(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)
{
 	u16 i;   
    W25Q128_Write_Enable();                    //дʹ��
	W25Q128_CS=0;                              //ʹ������   
    SPI1_ReadWriteByte(W25X128_PageProgram);      //����дҳ����   
    SPI1_ReadWriteByte((u8)((WriteAddr)>>16)); //����24bit��ַ    
    SPI1_ReadWriteByte((u8)((WriteAddr)>>8));   
    SPI1_ReadWriteByte((u8)WriteAddr);   
    for(i=0;i<NumByteToWrite;i++)SPI1_ReadWriteByte(pBuffer[i]);//ѭ��д��  
	W25Q128_CS=1;                                               //ȡ��Ƭѡ 
	W25Q128_Wait_Busy();					                    //�ȴ�д�����
} 

/*-------------------------------------------------*/
/*���������޼���дSPI FLASH                        */
/*��  ����pBuffer:���ݴ洢��                       */
/*��  ����WriteAddr:��ʼд��ĵ�ַ                 */
/*��  ����NumByteToRead:Ҫд����ֽ���             */
/*����ֵ����                                       */
/*-------------------------------------------------*/ 
void W25Q128_Write_NoCheck(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
{ 			 		 
	u16 pageremain;	   
	pageremain=256-WriteAddr%256;                           //��ҳʣ����ֽ���		 	    
	if(NumByteToWrite<=pageremain)pageremain=NumByteToWrite;//������256���ֽ�
	while(1)
	{	   
		W25Q128_Write_Page(pBuffer,WriteAddr,pageremain);
		if(NumByteToWrite==pageremain)break;               //д�������
	 	else                                               //NumByteToWrite>pageremain
		{
			pBuffer+=pageremain;
			WriteAddr+=pageremain;	

			NumByteToWrite-=pageremain;			  //��ȥ�Ѿ�д���˵��ֽ���
			if(NumByteToWrite>256)pageremain=256; //һ�ο���д��256���ֽ�
			else pageremain=NumByteToWrite; 	  //����256���ֽ���
		}
	};	    
} 

/*-------------------------------------------------*/
/*��������дSPI FLASH                              */
/*��  ����pBuffer:���ݴ洢��                       */
/*��  ����WriteAddr:��ʼд��ĵ�ַ                 */
/*��  ����NumByteToRead:Ҫд����ֽ���             */
/*����ֵ����                                       */
/*-------------------------------------------------*/  
u8 W25Q128_BUFFER[4096];		 
void W25Q128_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
{ 
	u32 secpos;
	u16 secoff;
	u16 secremain;	   
 	u16 i;    
	u8 * W25Q128_BUF;	  
   	W25Q128_BUF=W25Q128_BUFFER;	     
 	secpos=WriteAddr/4096;//������ַ  
	secoff=WriteAddr%4096;//�������ڵ�ƫ��
	secremain=4096-secoff;//����ʣ��ռ��С   
 	if(NumByteToWrite<=secremain)secremain=NumByteToWrite;//������4096���ֽ�
	while(1) 
	{	
		W25Q128_Read(W25Q128_BUF,secpos*4096,4096);//������������������
		for(i=0;i<secremain;i++)//У������
		{
			if(W25Q128_BUF[secoff+i]!=0XFF)break;//��Ҫ����  	  
		}
		if(i<secremain)//��Ҫ����
		{
			W25Q128_Erase_Sector(secpos);//�����������
			for(i=0;i<secremain;i++)	   //����
			{
				W25Q128_BUF[i+secoff]=pBuffer[i];	  
			}
			W25Q128_Write_NoCheck(W25Q128_BUF,secpos*4096,4096);//д����������  

		}else W25Q128_Write_NoCheck(pBuffer,WriteAddr,secremain);//д�Ѿ������˵�,ֱ��д������ʣ������. 				   
		if(NumByteToWrite==secremain)break;//д�������
		else//д��δ����
		{
			secpos++;//������ַ��1
			secoff=0;//ƫ��λ��Ϊ0 	 

		   	pBuffer+=secremain;  //ָ��ƫ��
			WriteAddr+=secremain;//д��ַƫ��	   
		   	NumByteToWrite-=secremain;				//�ֽ����ݼ�
			if(NumByteToWrite>4096)secremain=4096;	//��һ����������д����
			else secremain=NumByteToWrite;			//��һ����������д����
		}	 
	};	 
}

/*-------------------------------------------------*/
/*����������������оƬ                             */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void W25Q128_Erase_Chip(void)   
{                                   
    W25Q128_Write_Enable();                  //SET WEL 
    W25Q128_Wait_Busy();                     //�ȴ�����
  	W25Q128_CS=0;                            //ʹ������   
    SPI1_ReadWriteByte(W25X128_ChipErase);      //����Ƭ��������  
	W25Q128_CS=1;                            //ȡ��Ƭѡ     	      
	W25Q128_Wait_Busy();   				     //�ȴ�оƬ��������
}  

/*-------------------------------------------------*/
/*������������һ������                             */
/*��  ����Dst_Addr:������ַ                        */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void W25Q128_Erase_Sector(u32 Dst_Addr)   
{  	  
 	Dst_Addr*=4096;
    W25Q128_Write_Enable();                    //дʹ��	 
    W25Q128_Wait_Busy();                       //�ȴ�����  
  	W25Q128_CS=0;                              //ʹ������   
    SPI1_ReadWriteByte(W25X128_SectorErase);      //������������ָ�� 
    SPI1_ReadWriteByte((u8)((Dst_Addr)>>16));  //����24bit��ַ    
    SPI1_ReadWriteByte((u8)((Dst_Addr)>>8));   
    SPI1_ReadWriteByte((u8)Dst_Addr);  
	W25Q128_CS=1;                              //ȡ��Ƭѡ     	      
    W25Q128_Wait_Busy();   				       //�ȴ��������
}  

/*-------------------------------------------------*/
/*���������ȴ�����                                 */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void W25Q128_Wait_Busy(void)   
{   
	while((W25Q128_ReadSR()&0x01)==0x01);   // �ȴ�BUSYλ���
}   
