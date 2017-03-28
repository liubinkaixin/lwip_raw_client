/*-------------------------------------------------*/
/*         ��γ����STM32F407ZGT6������             */
/*    �Ա���ַ�� http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/
/*-------------------------------------------------*/
/*            ��������Դ�ļ�,��1������             */
/*-------------------------------------------------*/
/* void STM32_Standby(void)                        */
/*-------------------------------------------------*/

#include "wkup.h"
																	    
/*-------------------------------------------------*/
/*��������STM32�������״̬                        */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void STM32_Standby(void)
{			
	RCC_AHB1PeriphResetCmd(RCC_AHB1Periph_GPIOA,ENABLE);  //��λ����A��
	RCC_AHB1PeriphResetCmd(RCC_AHB1Periph_GPIOB,ENABLE);  //��λ����B��
	RCC_AHB1PeriphResetCmd(RCC_AHB1Periph_GPIOC,ENABLE);  //��λ����C��
	RCC_AHB1PeriphResetCmd(RCC_AHB1Periph_GPIOD,ENABLE);  //��λ����D��
	RCC_AHB1PeriphResetCmd(RCC_AHB1Periph_GPIOE,ENABLE);  //��λ����E��
	RCC_AHB1PeriphResetCmd(RCC_AHB1Periph_GPIOF,ENABLE);  //��λ����F��
	RCC_AHB1PeriphResetCmd(RCC_AHB1Periph_GPIOG,ENABLE);  //��λ����G��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);   //ʹ��PWRʱ��		
	PWR_ClearFlag(PWR_FLAG_WU);                           //���Wake-up ��־
	PWR_WakeUpPinCmd(ENABLE);                             //����WKUP���ڻ���
	PWR_EnterSTANDBYMode();                               //�������ģʽ	 
} 















