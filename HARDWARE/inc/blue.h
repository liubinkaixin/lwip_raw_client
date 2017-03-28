/*-------------------------------------------------*/
/*         超纬电子STM32F407ZGT6开发板             */
/*    淘宝地址： http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/
#ifndef __BLUE_H
#define __BLUE_H

#include "main.h"   

#define   BLUE_KEY    PBout(15)   //控制蓝牙模块的KEY脚，退出或是进入AT控制模式

void Blue_init(void);  //初始化蓝牙 
void Blue_InAT(void);  //蓝牙进入AT模式 
u8 Blue_QuitAT(void);  //蓝牙退出AT模式
u8 Blue_SendCmd_OK(u8 *cmd, u16 timeout);//发送AT指令，判断回复OK
u8 Blue_Setup(void);   //设置蓝牙模块

#endif
