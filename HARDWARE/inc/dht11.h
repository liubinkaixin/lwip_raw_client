/*-------------------------------------------------*/
/*         ��γ����STM32F407ZGT6������             */
/*    �Ա���ַ�� http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/

#ifndef __DHT11_H
#define __DHT11_H 

#include "main.h"   

#define DHT11_IO_IN()  {GPIOC->MODER&=~(3<<(0*2));GPIOC->MODER|=0<<0*2;}	//PC0����ģʽ
#define DHT11_IO_OUT() {GPIOC->MODER&=~(3<<(0*2));GPIOC->MODER|=1<<0*2;} 	//PC0���ģʽ 
										   
#define	DHT11_DQ_OUT PCout(0) //���ݶ˿�	PC0
#define	DHT11_DQ_IN  PCin(0)  //���ݶ˿�	PC0

u8 DHT11_Init(void);//��ʼ��DHT11
u8 DHT11_Read_Data(u8 *temp,u8 *humi);//��ȡ��ʪ��
u8 DHT11_Read_Byte(void);//����һ���ֽ�
u8 DHT11_Read_Bit(void);//����һ��λ
u8 DHT11_Check(void);//����Ƿ����DHT11
void DHT11_Rst(void);//��λDHT11    

#endif















