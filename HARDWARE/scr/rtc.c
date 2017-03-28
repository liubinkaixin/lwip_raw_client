/*--------------------------------------------------------------------*/
/*                  超纬电子STM32F407ZGT6开发板                       */
/*           淘宝地址： http://chaoweidianzi.taobao.com               */
/*--------------------------------------------------------------------*/
/*--------------------------------------------------------------------*/
/*                    实时时钟源文件,有3个函数                        */
/*--------------------------------------------------------------------*/
/* u8 Rtc_Init(void);		                                          */                       
/* ErrorStatus RTC_Set_Date(u8 year,u8 month,u8 date,u8 week);        */ 
/* ErrorStatus RTC_Set_Time(u8 hour,u8 min,u8 sec,u8 ampm);           */ 
/*--------------------------------------------------------------------*/

#include "rtc.h"
#include "delay.h"

/*-------------------------------------------------*/
/*函数名：RTC时间设置函数                          */
/*参  数：hour：时                                 */
/*参  数：min： 分                                 */
/*参  数：sec： 秒                                 */
/*参  数：ampm：上午/下午                          */
/*返回值：ErrorStatus  SUCCESS：成功  ERROR：失败  */
/*-------------------------------------------------*/
ErrorStatus RTC_Set_Time(u8 hour,u8 min,u8 sec,u8 ampm)
{
	RTC_TimeTypeDef RTC_TimeTypeInitStructure;	
	
	RTC_TimeTypeInitStructure.RTC_Hours=hour;    //设置 时
	RTC_TimeTypeInitStructure.RTC_Minutes=min;   //设置 分
	RTC_TimeTypeInitStructure.RTC_Seconds=sec;   //设置 秒
	RTC_TimeTypeInitStructure.RTC_H12=ampm;      //设置 上下午
	
	return RTC_SetTime(RTC_Format_BIN,&RTC_TimeTypeInitStructure);	  //返回时间设置成功与否
}

/*-------------------------------------------------*/
/*函数名：RTC日期设置函数                          */
/*参  数：year： 年                                */
/*参  数：month：月                                */
/*参  数：date： 日                                */
/*参  数：week： 周                                */
/*返回值：ErrorStatus  SUCCESS：成功  ERROR：失败  */
/*-------------------------------------------------*/
ErrorStatus RTC_Set_Date(u8 year,u8 month,u8 date,u8 week)
{	
	RTC_DateTypeDef RTC_DateTypeInitStructure;
	
	RTC_DateTypeInitStructure.RTC_Date=date;     //设置 年
	RTC_DateTypeInitStructure.RTC_Month=month;   //设置 月
	RTC_DateTypeInitStructure.RTC_WeekDay=week;  //设置 日
	RTC_DateTypeInitStructure.RTC_Year=year;     //设置 周
	
	return RTC_SetDate(RTC_Format_BIN,&RTC_DateTypeInitStructure);  //返回日期设置成功与否
}
/*-------------------------------------------------*/
/*函数名：RTC初始化                                */
/*参  数：无                                       */
/*返回值：0成功  1失败                             */
/*-------------------------------------------------*/
u8 Rtc_Init(void)
{
	RTC_InitTypeDef RTC_InitStructure;
	u16 retry=0; 
	
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);     //使能PWR时钟
	PWR_BackupAccessCmd(ENABLE);	                        //使能后备寄存器访问 
	
	if(RTC_ReadBackupRegister(RTC_BKP_DR0)!=0xAAAA)		    //判断是不是初次设置，设置后，后背寄存器记录0xAAAA
	{
		RCC_LSEConfig(RCC_LSE_ON);                                          //开启时钟    
		while ((RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)&&(retry<500))	//检查指低速晶振就绪与否
		{
			retry++;          //超时计数器，5s的超时时间
			delay_ms(10);     //延时10ms
		}
		if(retry>=500)        //超时
			return 1;         //返回1，表示出错
		
		//没有超时，RTC时钟起振	
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);		 //设置RTC时钟(RTCCLK),选择LSE作为RTC时钟    
		RCC_RTCCLKCmd(ENABLE);	                     //使能RTC时钟 

		RTC_InitStructure.RTC_AsynchPrediv = 0x7F;             //RTC异步分频系数(1~0X7F)
		RTC_InitStructure.RTC_SynchPrediv  = 0xFF;             //RTC同步分频系数(0~7FFF)
		RTC_InitStructure.RTC_HourFormat   = RTC_HourFormat_24;//RTC设置为,24小时格式
		RTC_Init(&RTC_InitStructure);                          //按配置设置
 
		RTC_Set_Time(13,00,00,RTC_H12_AM);	//设置时间
		RTC_Set_Date(16,7,27,3);		    //设置日期
	 
		RTC_WriteBackupRegister(RTC_BKP_DR0,0xAAAA);	//标记已经初始化过了
	} 	
	return 0;                     //返回0，表示成功
}

 












