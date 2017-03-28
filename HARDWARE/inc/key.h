/*-------------------------------------------------*/
/*          超纬电子STM32F407ZGT6开发板            */
/*    淘宝地址： http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/
#ifndef __KEY_H
#define __KEY_H	

#include "main.h" 

#define  KEY_GROUP_EN  RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOE   //控制按键引脚的IO组

#define  KEY1_GROUP   GPIOA        //控制K1引脚的IO组
#define  KEY1_PIN     GPIO_Pin_0   //控制K1引脚的编号

#define  KEY2_GROUP   GPIOE        //控制K2引脚的IO组
#define  KEY2_PIN     GPIO_Pin_2   //控制K2引脚的编号

#define  KEY3_GROUP   GPIOE        //控制K3引脚的IO组
#define  KEY3_PIN     GPIO_Pin_3   //控制K3引脚的编号

#define  KEY4_GROUP   GPIOE        //控制K4引脚的IO组
#define  KEY4_PIN     GPIO_Pin_4   //控制K4引脚的编号
 
//读取按键状态
#define KEY1		PAin(0) 
#define KEY2 		PEin(2)	
#define KEY3 		PEin(3) 
#define KEY4 	    PEin(4) 

//按键值
#define NONE_PRES_NUM 	 0
#define KEY1_PRES_NUM 	 1
#define KEY2_PRES_NUM	 2
#define KEY3_PRES_NUM	 3
#define KEY4_PRES_NUM    4

void KEY_Init(void);	//按键初始化
u8 KEY_Scan(u8 mode);   //按键处理函数	

#endif
