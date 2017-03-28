/*-------------------------------------------------*/
/*          ��γ����STM32F407ZGT6������            */
/*    �Ա���ַ�� http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/
#ifndef __KEY_H
#define __KEY_H	

#include "main.h" 

#define  KEY_GROUP_EN  RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOE   //���ư������ŵ�IO��

#define  KEY1_GROUP   GPIOA        //����K1���ŵ�IO��
#define  KEY1_PIN     GPIO_Pin_0   //����K1���ŵı��

#define  KEY2_GROUP   GPIOE        //����K2���ŵ�IO��
#define  KEY2_PIN     GPIO_Pin_2   //����K2���ŵı��

#define  KEY3_GROUP   GPIOE        //����K3���ŵ�IO��
#define  KEY3_PIN     GPIO_Pin_3   //����K3���ŵı��

#define  KEY4_GROUP   GPIOE        //����K4���ŵ�IO��
#define  KEY4_PIN     GPIO_Pin_4   //����K4���ŵı��
 
//��ȡ����״̬
#define KEY1		PAin(0) 
#define KEY2 		PEin(2)	
#define KEY3 		PEin(3) 
#define KEY4 	    PEin(4) 

//����ֵ
#define NONE_PRES_NUM 	 0
#define KEY1_PRES_NUM 	 1
#define KEY2_PRES_NUM	 2
#define KEY3_PRES_NUM	 3
#define KEY4_PRES_NUM    4

void KEY_Init(void);	//������ʼ��
u8 KEY_Scan(u8 mode);   //����������	

#endif
