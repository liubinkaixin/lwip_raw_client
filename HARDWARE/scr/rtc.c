/*--------------------------------------------------------------------*/
/*                  ��γ����STM32F407ZGT6������                       */
/*           �Ա���ַ�� http://chaoweidianzi.taobao.com               */
/*--------------------------------------------------------------------*/
/*--------------------------------------------------------------------*/
/*                    ʵʱʱ��Դ�ļ�,��3������                        */
/*--------------------------------------------------------------------*/
/* u8 Rtc_Init(void);		                                          */                       
/* ErrorStatus RTC_Set_Date(u8 year,u8 month,u8 date,u8 week);        */ 
/* ErrorStatus RTC_Set_Time(u8 hour,u8 min,u8 sec,u8 ampm);           */ 
/*--------------------------------------------------------------------*/

#include "rtc.h"
#include "delay.h"

/*-------------------------------------------------*/
/*��������RTCʱ�����ú���                          */
/*��  ����hour��ʱ                                 */
/*��  ����min�� ��                                 */
/*��  ����sec�� ��                                 */
/*��  ����ampm������/����                          */
/*����ֵ��ErrorStatus  SUCCESS���ɹ�  ERROR��ʧ��  */
/*-------------------------------------------------*/
ErrorStatus RTC_Set_Time(u8 hour,u8 min,u8 sec,u8 ampm)
{
	RTC_TimeTypeDef RTC_TimeTypeInitStructure;	
	
	RTC_TimeTypeInitStructure.RTC_Hours=hour;    //���� ʱ
	RTC_TimeTypeInitStructure.RTC_Minutes=min;   //���� ��
	RTC_TimeTypeInitStructure.RTC_Seconds=sec;   //���� ��
	RTC_TimeTypeInitStructure.RTC_H12=ampm;      //���� ������
	
	return RTC_SetTime(RTC_Format_BIN,&RTC_TimeTypeInitStructure);	  //����ʱ�����óɹ����
}

/*-------------------------------------------------*/
/*��������RTC�������ú���                          */
/*��  ����year�� ��                                */
/*��  ����month����                                */
/*��  ����date�� ��                                */
/*��  ����week�� ��                                */
/*����ֵ��ErrorStatus  SUCCESS���ɹ�  ERROR��ʧ��  */
/*-------------------------------------------------*/
ErrorStatus RTC_Set_Date(u8 year,u8 month,u8 date,u8 week)
{	
	RTC_DateTypeDef RTC_DateTypeInitStructure;
	
	RTC_DateTypeInitStructure.RTC_Date=date;     //���� ��
	RTC_DateTypeInitStructure.RTC_Month=month;   //���� ��
	RTC_DateTypeInitStructure.RTC_WeekDay=week;  //���� ��
	RTC_DateTypeInitStructure.RTC_Year=year;     //���� ��
	
	return RTC_SetDate(RTC_Format_BIN,&RTC_DateTypeInitStructure);  //�����������óɹ����
}
/*-------------------------------------------------*/
/*��������RTC��ʼ��                                */
/*��  ������                                       */
/*����ֵ��0�ɹ�  1ʧ��                             */
/*-------------------------------------------------*/
u8 Rtc_Init(void)
{
	RTC_InitTypeDef RTC_InitStructure;
	u16 retry=0; 
	
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);     //ʹ��PWRʱ��
	PWR_BackupAccessCmd(ENABLE);	                        //ʹ�ܺ󱸼Ĵ������� 
	
	if(RTC_ReadBackupRegister(RTC_BKP_DR0)!=0xAAAA)		    //�ж��ǲ��ǳ������ã����ú󣬺󱳼Ĵ�����¼0xAAAA
	{
		RCC_LSEConfig(RCC_LSE_ON);                                          //����ʱ��    
		while ((RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)&&(retry<500))	//���ָ���پ���������
		{
			retry++;          //��ʱ��������5s�ĳ�ʱʱ��
			delay_ms(10);     //��ʱ10ms
		}
		if(retry>=500)        //��ʱ
			return 1;         //����1����ʾ����
		
		//û�г�ʱ��RTCʱ������	
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);		 //����RTCʱ��(RTCCLK),ѡ��LSE��ΪRTCʱ��    
		RCC_RTCCLKCmd(ENABLE);	                     //ʹ��RTCʱ�� 

		RTC_InitStructure.RTC_AsynchPrediv = 0x7F;             //RTC�첽��Ƶϵ��(1~0X7F)
		RTC_InitStructure.RTC_SynchPrediv  = 0xFF;             //RTCͬ����Ƶϵ��(0~7FFF)
		RTC_InitStructure.RTC_HourFormat   = RTC_HourFormat_24;//RTC����Ϊ,24Сʱ��ʽ
		RTC_Init(&RTC_InitStructure);                          //����������
 
		RTC_Set_Time(13,00,00,RTC_H12_AM);	//����ʱ��
		RTC_Set_Date(16,7,27,3);		    //��������
	 
		RTC_WriteBackupRegister(RTC_BKP_DR0,0xAAAA);	//����Ѿ���ʼ������
	} 	
	return 0;                     //����0����ʾ�ɹ�
}

 












