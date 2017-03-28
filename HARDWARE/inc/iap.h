/*-------------------------------------------------*/
/*         超纬电子STM32F407ZGT6开发板             */
/*    淘宝地址： http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/

#ifndef __USARTIAP_H__
#define __USARTIAP_H__

#include "stm32f4xx.h" 

typedef  void (*iapfun)(void);

#define APP_START_ADDR		       0x8040000 //应用程序地址，从扇区6开始
   
void IAP_load_app(u32 appxaddr);              //跳转到应用程序执行
void MSR_MSP(u32 addr);	                      //设置堆栈地址 
u8 Erase_APPSector(void);                     //擦除APP用到的扇区5-11 
  
#endif







































