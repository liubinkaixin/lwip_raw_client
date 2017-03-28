/*-------------------------------------------------*/
/*         超纬电子STM32F407ZGT6开发板             */
/*    淘宝地址： http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/

#ifndef __CAN_H
#define __CAN_H	 

#include "main.h"	    
							  										 							 				    
u8  CAN1_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode);//CAN初始化
u8  CAN1_Send_Msg(u8* msg,u8 len);						     //发送数据
u8  CAN1_Receive_Msg(u8 *buf);							     //接收数据

#endif

















