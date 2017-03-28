/*-------------------------------------------------*/
/*         ��γ����STM32F407ZGT6������             */
/*    �Ա���ַ�� http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/
#ifndef __DS18B20_H
#define __DS18B20_H 

#include "main.h"   

//IO��������
#define DS18B20_IO_IN()  {GPIOC->MODER&=~(3<<(0*2));GPIOC->MODER|=0<<0*2;}	//PC0����ģʽ
#define DS18B20_IO_OUT() {GPIOC->MODER&=~(3<<(0*2));GPIOC->MODER|=1<<0*2;} 	//PC0���ģʽ
 
////IO��������											   
#define	DS18B20_DQ_OUT PCout(0) //���ݶ˿�	PC0
#define	DS18B20_DQ_IN  PCin(0)  //���ݶ˿�	PC0 
   	
u8 DS18B20_Init(void);			//��ʼ��DS18B20
double DS18B20_Get_Temp(void);	//��ȡ�¶�
void DS18B20_Start(void);		//��ʼ�¶�ת��
void DS18B20_Write_Byte(u8 dat);//д��һ���ֽ�
u8 DS18B20_Read_Byte(void);		//����һ���ֽ�
u8 DS18B20_Read_Bit(void);		//����һ��λ
u8 DS18B20_Check(void);			//����Ƿ����DS18B20
void DS18B20_Rst(void);			//��λDS18B20    
#endif















