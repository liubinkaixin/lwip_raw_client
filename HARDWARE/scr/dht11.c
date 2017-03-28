/*-------------------------------------------------*/
/*         超纬电子STM32F407ZGT6开发板             */
/*    淘宝地址： http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/
/*-------------------------------------------------*/
/*            DHT11源文件,有6个函数                 */
/*-------------------------------------------------*/
/* u8 DHT11_Init(void);                  初始化    */
/* u8 DHT11_Read_Data(u8 *temp,u8 *humi);读温湿度  */
/* u8 DHT11_Read_Byte(void);             读一个字节*/
/* u8 DHT11_Read_Bit(void);              读一个位  */
/* u8 DHT11_Check(void);                 检测DHT11 */
/* void DHT11_Rst(void);                 复位DHT11 */  
/*-------------------------------------------------*/

#include "dht11.h"
#include "delay.h"
  
/*-------------------------------------------------*/
/*函数名：复位DHT11                                */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/  
void DHT11_Rst(void)	   
{                 
  DHT11_IO_OUT(); 	//SET OUTPUT
  DHT11_DQ_OUT=0; 	//拉低DQ
  delay_ms(20);    	//拉低至少18ms
  DHT11_DQ_OUT=1; 	//DQ=1 
  delay_us(30);     //主机拉高20~40us
}

/*-------------------------------------------------*/
/*函数名：等待DHT11的回应                          */
/*参  数：无                                       */
/*返回值：1错误 0正确                              */
/*-------------------------------------------------*/ 
u8 DHT11_Check(void) 	   
{   
	u8 retry=0;
	DHT11_IO_IN();
    while (DHT11_DQ_IN&&retry<100)//DHT11会拉低40~80us
	{
		retry++;
		delay_us(1);
	};	 
	if(retry>=100)return 1;
	else retry=0;
    while (!DHT11_DQ_IN&&retry<100)//DHT11拉低后会再次拉高40~80us
	{
		retry++;
		delay_us(1);
	};
	if(retry>=100)return 1;	    
	return 0;
}

/*-------------------------------------------------*/
/*函数名：读取一个位                               */
/*参  数：无                                       */
/*返回值：1或0                                     */
/*-------------------------------------------------*/ 
u8 DHT11_Read_Bit(void) 			 
{
 	u8 retry=0;
	while(DHT11_DQ_IN&&retry<100)//等待变为低电平
	{
		retry++;
		delay_us(1);
	}
	retry=0;
	while(!DHT11_DQ_IN&&retry<100)//等待变高电平
	{
		retry++;
		delay_us(1);
	}
	delay_us(40);//等待40us
	if(DHT11_DQ_IN)return 1;
	else return 0;		   
}

/*-------------------------------------------------*/
/*函数名：读取一个字节                             */
/*参  数：无                                       */
/*返回值：数据                                     */
/*-------------------------------------------------*/ 
u8 DHT11_Read_Byte(void)    
{        
    u8 i,dat;
    dat=0;
	for (i=0;i<8;i++) 
	{
   		dat<<=1; 
	    dat|=DHT11_Read_Bit();
    }						    
    return dat;
}

/*-------------------------------------------------*/
/*函数名：读取一次数据温湿度                       */
/*参  数：temp:温度值                              */
/*参  数：humi:湿度值                              */
/*返回值：1错误 0正确                              */
/*-------------------------------------------------*/ 
u8 DHT11_Read_Data(u8 *temp,u8 *humi)    
{        
 	u8 buf[5];
	u8 i;
	DHT11_Rst();
	if(DHT11_Check()==0)
	{
		for(i=0;i<5;i++)//读取40位数据
		{
			buf[i]=DHT11_Read_Byte();
		}
		if((buf[0]+buf[1]+buf[2]+buf[3])==buf[4])
		{
			*humi=buf[0];
			*temp=buf[2];
		}
	}else return 1;
	return 0;	    
}

/*-------------------------------------------------*/
/*函数名：初始化DHT11                              */
/*参  数：无                                       */
/*返回值：1错误 0正确                              */
/*-------------------------------------------------*/    	 
u8 DHT11_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//使能GPIOC时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//50MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化
	DHT11_Rst();
	return DHT11_Check();
}
























