/*-------------------------------------------------*/
/*         ��γ����STM32F407ZGT6������             */
/*    �Ա���ַ�� http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/

#ifndef __USARTIAP_H__
#define __USARTIAP_H__

#include "stm32f4xx.h" 

typedef  void (*iapfun)(void);

#define APP_START_ADDR		       0x8040000 //Ӧ�ó����ַ��������6��ʼ
   
void IAP_load_app(u32 appxaddr);              //��ת��Ӧ�ó���ִ��
void MSR_MSP(u32 addr);	                      //���ö�ջ��ַ 
u8 Erase_APPSector(void);                     //����APP�õ�������5-11 
  
#endif







































