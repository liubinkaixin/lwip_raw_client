/*-------------------------------------------------*/
/*         超纬电子STM32F407ZGT6开发板             */
/*    淘宝地址： http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/
/*-------------------------------------------------*/
/*            W25Q128源文件,有个函数               */
/*              4Kbytes为一个Sector                */
/*               16个扇区为1个Block                */
/*      容量为16M字节,共有128个Block,4096个Sector  */
/*-------------------------------------------------*/
/*-------------------------------------------------*/

#include "w25q128.h" 
#include "spi1.h"
#include "delay.h"	    
 
/*--------------------------------*----------------*/
/*函数名：初始化SPI FLASH的IO口                    */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/													 
void W25Q128_Init(void)
{ 
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);//使能GPIOG时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;           //PG15
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;        //输出
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;       //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;   //100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;         //上拉
	GPIO_Init(GPIOG, &GPIO_InitStructure);               //初始化

	W25Q128_CS=1;			                 //SPI FLASH不选中
	SPI1_Init();		   			         //初始化SPI
	SPI1_SetSpeed(SPI_BaudRatePrescaler_2);  //设置为42M时钟,高速模式 
}

/*-------------------------------------------------*/
/*函数名：读取W25Q128的状态寄存器                  */
/*参  数：无                                       */
/*返回值：状态                                     */
/*-------------------------------------------------*/
u8 W25Q128_ReadSR(void)   
{  
	u8 byte=0;                            
	W25Q128_CS=0;                              //使能器件   
	SPI1_ReadWriteByte(W25X128_ReadStatusReg);    //发送读取状态寄存器命令    
	byte=SPI1_ReadWriteByte(0Xff);             //读取一个字节  
	W25Q128_CS=1;                              //取消片选     
	return byte;                               //返回状态
} 

/*-------------------------------------------------*/
/*函数名：写W25Q128状态寄存器                      */
/*参  数：sr：写入值                               */
/*返回值：无                                       */
/*-------------------------------------------------*/
void W25Q128_Write_SR(u8 sr)   
{   
	W25Q128_CS=0;                              //使能器件   
	SPI1_ReadWriteByte(W25X128_WriteStatusReg);   //发送写取状态寄存器命令    
	SPI1_ReadWriteByte(sr);                    //写入一个字节  
	W25Q128_CS=1;                              //取消片选     	      
}   

/*-------------------------------------------------*/
/*函数名：W25Q128写使能                            */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/  
void W25Q128_Write_Enable(void)   
{
	W25Q128_CS=0;                            //使能器件   
    SPI1_ReadWriteByte(W25X128_WriteEnable);    //发送写使能  
	W25Q128_CS=1;                            //取消片选     	      
} 

/*-------------------------------------------------*/
/*函数名：W25Q128写禁止                            */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/ 
void W25Q128_Write_Disable(void)   
{  
	W25Q128_CS=0;                            //使能器件   
    SPI1_ReadWriteByte(W25X128_WriteDisable);   //发送写禁止指令    
	W25Q128_CS=1;                            //取消片选     	      
} 	

/*-------------------------------------------------*/
/*函数名：读取FLASH                                */
/*参  数：pBuffer:数据存储区                       */
/*参  数：ReadAddr:开始读取的地址                  */
/*参  数：NumByteToRead:要读取的字节数             */
/*返回值：无                                       */
/*-------------------------------------------------*/ 
void W25Q128_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead)   
{ 
 	u16 i;   										    
	W25Q128_CS=0;                              //使能器件   
    SPI1_ReadWriteByte(W25X128_ReadData);         //发送读取命令   
    SPI1_ReadWriteByte((u8)((ReadAddr)>>16));  //发送24bit地址 ，分3次发送   
    SPI1_ReadWriteByte((u8)((ReadAddr)>>8));   
    SPI1_ReadWriteByte((u8)ReadAddr);           
    
	for(i=0;i<NumByteToRead;i++)               //利用for循环读取数据
	{ 
        pBuffer[i]=SPI1_ReadWriteByte(0XFF);   //循环读取  
    }
	W25Q128_CS=1;                              //取消片选 				    	      
} 

/*-------------------------------------------------*/
/*函数名：写入少于256个字节的数据                  */
/*参  数：pBuffer:数据存储区                       */
/*参  数：WriteAddr:开始写入的地址                  */
/*参  数：NumByteToRead:要写入的字节数             */
/*返回值：无                                       */
/*-------------------------------------------------*/ 
void W25Q128_Write_Page(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)
{
 	u16 i;   
    W25Q128_Write_Enable();                    //写使能
	W25Q128_CS=0;                              //使能器件   
    SPI1_ReadWriteByte(W25X128_PageProgram);      //发送写页命令   
    SPI1_ReadWriteByte((u8)((WriteAddr)>>16)); //发送24bit地址    
    SPI1_ReadWriteByte((u8)((WriteAddr)>>8));   
    SPI1_ReadWriteByte((u8)WriteAddr);   
    for(i=0;i<NumByteToWrite;i++)SPI1_ReadWriteByte(pBuffer[i]);//循环写数  
	W25Q128_CS=1;                                               //取消片选 
	W25Q128_Wait_Busy();					                    //等待写入结束
} 

/*-------------------------------------------------*/
/*函数名：无检验写SPI FLASH                        */
/*参  数：pBuffer:数据存储区                       */
/*参  数：WriteAddr:开始写入的地址                 */
/*参  数：NumByteToRead:要写入的字节数             */
/*返回值：无                                       */
/*-------------------------------------------------*/ 
void W25Q128_Write_NoCheck(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
{ 			 		 
	u16 pageremain;	   
	pageremain=256-WriteAddr%256;                           //单页剩余的字节数		 	    
	if(NumByteToWrite<=pageremain)pageremain=NumByteToWrite;//不大于256个字节
	while(1)
	{	   
		W25Q128_Write_Page(pBuffer,WriteAddr,pageremain);
		if(NumByteToWrite==pageremain)break;               //写入结束了
	 	else                                               //NumByteToWrite>pageremain
		{
			pBuffer+=pageremain;
			WriteAddr+=pageremain;	

			NumByteToWrite-=pageremain;			  //减去已经写入了的字节数
			if(NumByteToWrite>256)pageremain=256; //一次可以写入256个字节
			else pageremain=NumByteToWrite; 	  //不够256个字节了
		}
	};	    
} 

/*-------------------------------------------------*/
/*函数名：写SPI FLASH                              */
/*参  数：pBuffer:数据存储区                       */
/*参  数：WriteAddr:开始写入的地址                 */
/*参  数：NumByteToRead:要写入的字节数             */
/*返回值：无                                       */
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
 	secpos=WriteAddr/4096;//扇区地址  
	secoff=WriteAddr%4096;//在扇区内的偏移
	secremain=4096-secoff;//扇区剩余空间大小   
 	if(NumByteToWrite<=secremain)secremain=NumByteToWrite;//不大于4096个字节
	while(1) 
	{	
		W25Q128_Read(W25Q128_BUF,secpos*4096,4096);//读出整个扇区的内容
		for(i=0;i<secremain;i++)//校验数据
		{
			if(W25Q128_BUF[secoff+i]!=0XFF)break;//需要擦除  	  
		}
		if(i<secremain)//需要擦除
		{
			W25Q128_Erase_Sector(secpos);//擦除这个扇区
			for(i=0;i<secremain;i++)	   //复制
			{
				W25Q128_BUF[i+secoff]=pBuffer[i];	  
			}
			W25Q128_Write_NoCheck(W25Q128_BUF,secpos*4096,4096);//写入整个扇区  

		}else W25Q128_Write_NoCheck(pBuffer,WriteAddr,secremain);//写已经擦除了的,直接写入扇区剩余区间. 				   
		if(NumByteToWrite==secremain)break;//写入结束了
		else//写入未结束
		{
			secpos++;//扇区地址增1
			secoff=0;//偏移位置为0 	 

		   	pBuffer+=secremain;  //指针偏移
			WriteAddr+=secremain;//写地址偏移	   
		   	NumByteToWrite-=secremain;				//字节数递减
			if(NumByteToWrite>4096)secremain=4096;	//下一个扇区还是写不完
			else secremain=NumByteToWrite;			//下一个扇区可以写完了
		}	 
	};	 
}

/*-------------------------------------------------*/
/*函数名：擦除整个芯片                             */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void W25Q128_Erase_Chip(void)   
{                                   
    W25Q128_Write_Enable();                  //SET WEL 
    W25Q128_Wait_Busy();                     //等待空闲
  	W25Q128_CS=0;                            //使能器件   
    SPI1_ReadWriteByte(W25X128_ChipErase);      //发送片擦除命令  
	W25Q128_CS=1;                            //取消片选     	      
	W25Q128_Wait_Busy();   				     //等待芯片擦除结束
}  

/*-------------------------------------------------*/
/*函数名：擦除一个扇区                             */
/*参  数：Dst_Addr:扇区地址                        */
/*返回值：无                                       */
/*-------------------------------------------------*/
void W25Q128_Erase_Sector(u32 Dst_Addr)   
{  	  
 	Dst_Addr*=4096;
    W25Q128_Write_Enable();                    //写使能	 
    W25Q128_Wait_Busy();                       //等待空闲  
  	W25Q128_CS=0;                              //使能器件   
    SPI1_ReadWriteByte(W25X128_SectorErase);      //发送扇区擦除指令 
    SPI1_ReadWriteByte((u8)((Dst_Addr)>>16));  //发送24bit地址    
    SPI1_ReadWriteByte((u8)((Dst_Addr)>>8));   
    SPI1_ReadWriteByte((u8)Dst_Addr);  
	W25Q128_CS=1;                              //取消片选     	      
    W25Q128_Wait_Busy();   				       //等待擦除完成
}  

/*-------------------------------------------------*/
/*函数名：等待空闲                                 */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void W25Q128_Wait_Busy(void)   
{   
	while((W25Q128_ReadSR()&0x01)==0x01);   // 等待BUSY位清空
}   
