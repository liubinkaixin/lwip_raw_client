/*-------------------------------------------------*/
/*         超纬电子STM32F407ZGT6开发板             */
/*    淘宝地址： http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/
/*-------------------------------------------------*/
/*            18b20源文件,有8个函数                */
/*-------------------------------------------------*/
/* u8 DS18B20_Init(void);			初始化DS18B20  */
/* double DS18B20_Get_Temp(void);	获取温度       */
/* void DS18B20_Start(void);		开始温度转换   */
/* void DS18B20_Write_Byte(u8 dat); 写入一个字节   */
/* u8 DS18B20_Read_Byte(void);		读出一个字节   */
/* u8 DS18B20_Read_Bit(void);		读出一个位     */
/* u8 DS18B20_Check(void);			检测DS18B20    */
/* void DS18B20_Rst(void);			复位DS18B20    */ 
/*-------------------------------------------------*/

#include "ds18b20.h"
#include "delay.h"	
  
/*-------------------------------------------------*/
/*函数名：复位DS18B20函数                          */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void DS18B20_Rst(void)	   
{                 
	DS18B20_IO_OUT(); //SET PG11 OUTPUT
    DS18B20_DQ_OUT=0; //拉低DQ
    delay_us(750);    //拉低750us
    DS18B20_DQ_OUT=1; //DQ=1 
	delay_us(15);     //15US
}

/*-------------------------------------------------*/
/*函数名：等待DS18B20的回应                        */
/*参  数：无                                       */
/*返回值：1:未检测到DS18B20的存在                  */
/*返回值：0:存在                                   */  
/*-------------------------------------------------*/
u8 DS18B20_Check(void) 	   
{   
	u8 retry=0;
	DS18B20_IO_IN(); 
    while (DS18B20_DQ_IN&&retry<200)
	{
		retry++;
		delay_us(1);
	};	 
	if(retry>=200)return 1;
	else retry=0;
    while (!DS18B20_DQ_IN&&retry<240)
	{
		retry++;
		delay_us(1);
	};
	if(retry>=240)return 1;	    
	return 0;
}

/*-------------------------------------------------*/
/*函数名：读取一个位                               */
/*参  数：无                                       */
/*返回值：1或0                                     */ 
/*-------------------------------------------------*/
u8 DS18B20_Read_Bit(void) 		
{
	u8 data;
	
	DS18B20_IO_OUT();
	DS18B20_DQ_OUT=0; 
	delay_us(2);
	DS18B20_DQ_OUT=1; 
	DS18B20_IO_IN();
	delay_us(12);
	if(DS18B20_DQ_IN)data=1;
	else data=0;	 
	delay_us(50);           
	return data;
}

/*-------------------------------------------------*/
/*函数名：读取一个字节                             */
/*参  数：无                                       */
/*返回值：数据                                     */ 
/*-------------------------------------------------*/
u8 DS18B20_Read_Byte(void)    
{        
    u8 i,j,dat;
    dat=0;
	for (i=1;i<=8;i++) 
	{
        j=DS18B20_Read_Bit();
        dat=(j<<7)|(dat>>1);
    }						    
    return dat;
}

/*-------------------------------------------------*/
/*函数名：写一个字节                               */
/*参  数：dat：数据                                */
/*返回值：无                                       */ 
/*-------------------------------------------------*/
void DS18B20_Write_Byte(u8 dat)     
 {             
    u8 j;
    u8 testb;
	  DS18B20_IO_OUT();
    for (j=1;j<=8;j++) 
	{
        testb=dat&0x01;
        dat=dat>>1;
        if (testb) 
        {
            DS18B20_DQ_OUT=0;
            delay_us(2);                            
            DS18B20_DQ_OUT=1;
            delay_us(60);             
        }
        else 
        {
            DS18B20_DQ_OUT=0;// Write 0
            delay_us(60);             
            DS18B20_DQ_OUT=1;
            delay_us(2);                          
        }
    }
}

/*-------------------------------------------------*/
/*函数名：温度转换                                 */
/*参  数：无                                       */
/*返回值：无                                       */ 
/*-------------------------------------------------*/
void DS18B20_Start(void)
{   						               
    DS18B20_Rst();	   
	DS18B20_Check();	 
    DS18B20_Write_Byte(0xcc);
    DS18B20_Write_Byte(0x44);
} 

/*-------------------------------------------------*/
/*函数名：初始化DS18B20                            */
/*参  数：无                                       */
/*返回值：0：正确  1：错误                         */ 
/*-------------------------------------------------*/  	 
u8 DS18B20_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//使能GPIOC时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//50MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;          //上拉
	GPIO_Init(GPIOC, &GPIO_InitStructure);                //初始化
 
 	DS18B20_Rst();
	return DS18B20_Check();
} 

/*-------------------------------------------------*/
/*函数名：得到温度值                               */
/*参  数：无                                       */
/*返回值：温度值                                   */ 
/*-------------------------------------------------*/ 
double DS18B20_Get_Temp(void)
{
    u8 temp;
    u8 TL,TH;
	short tem_temp;
	double tem;
	
    DS18B20_Start ();                   
    DS18B20_Rst();
    DS18B20_Check();	 
    DS18B20_Write_Byte(0xcc);
    DS18B20_Write_Byte(0xbe);
    TL=DS18B20_Read_Byte();  
    TH=DS18B20_Read_Byte(); 
    if(TH>7)
    {
        TH=~TH;
        TL=~TL; 
        temp=0;//温度为负  
    }else temp=1;//温度为正	  	  
    tem_temp=TH; //获得高八位
    tem_temp<<=8;    
    tem_temp+=TL;//获得底八位
    tem=(double)tem_temp*0.0625;//转换     
	
	if(temp)return tem; //返回温度值
	else return -tem;    
}
