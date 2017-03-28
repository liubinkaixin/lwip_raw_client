/*-------------------------------------------------*/
/*         超纬电子STM32F407ZGT6开发板             */
/*    淘宝地址： http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/

#ifndef __IR_H
#define __IR_H 

#include "main.h"   
  
#define IR_in PAin(8)	    //红外数据输入脚

extern u8  RmtSta;	//状态 	  
extern u16 Dval;    //下降沿时计数器的值
extern u32 RmtRec;	//红外接收到的数据	   		    
extern u8  RmtCnt;	//按键按下的次数	

void IR_Init(void);  //红外遥控初始化定时器
u8 IR_recive(void);  //红外遥控信号采集 

#endif

