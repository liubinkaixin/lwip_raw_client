/*--------------------------------------------------------------------*/
/*                  超纬电子STM32F407ZGT6开发板                       */
/*           淘宝地址： http://chaoweidianzi.taobao.com               */
/*--------------------------------------------------------------------*/
#ifndef __RTC_H
#define __RTC_H	 

#include "main.h" 

u8 Rtc_Init(void);		                                      //RTC初始化
ErrorStatus RTC_Set_Date(u8 year,u8 month,u8 date,u8 week);   //设置日期
ErrorStatus RTC_Set_Time(u8 hour,u8 min,u8 sec,u8 ampm);      //设置时间

#endif

















