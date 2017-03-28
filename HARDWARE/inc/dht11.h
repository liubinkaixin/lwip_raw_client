/*-------------------------------------------------*/
/*         超纬电子STM32F407ZGT6开发板             */
/*    淘宝地址： http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/

#ifndef __DHT11_H
#define __DHT11_H 

#include "main.h"   

#define DHT11_IO_IN()  {GPIOC->MODER&=~(3<<(0*2));GPIOC->MODER|=0<<0*2;}	//PC0输入模式
#define DHT11_IO_OUT() {GPIOC->MODER&=~(3<<(0*2));GPIOC->MODER|=1<<0*2;} 	//PC0输出模式 
										   
#define	DHT11_DQ_OUT PCout(0) //数据端口	PC0
#define	DHT11_DQ_IN  PCin(0)  //数据端口	PC0

u8 DHT11_Init(void);//初始化DHT11
u8 DHT11_Read_Data(u8 *temp,u8 *humi);//读取温湿度
u8 DHT11_Read_Byte(void);//读出一个字节
u8 DHT11_Read_Bit(void);//读出一个位
u8 DHT11_Check(void);//检测是否存在DHT11
void DHT11_Rst(void);//复位DHT11    

#endif















