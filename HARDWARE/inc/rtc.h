/*--------------------------------------------------------------------*/
/*                  ��γ����STM32F407ZGT6������                       */
/*           �Ա���ַ�� http://chaoweidianzi.taobao.com               */
/*--------------------------------------------------------------------*/
#ifndef __RTC_H
#define __RTC_H	 

#include "main.h" 

u8 Rtc_Init(void);		                                      //RTC��ʼ��
ErrorStatus RTC_Set_Date(u8 year,u8 month,u8 date,u8 week);   //��������
ErrorStatus RTC_Set_Time(u8 hour,u8 min,u8 sec,u8 ampm);      //����ʱ��

#endif

















