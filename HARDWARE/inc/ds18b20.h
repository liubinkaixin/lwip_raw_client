/*-------------------------------------------------*/
/*         超纬电子STM32F407ZGT6开发板             */
/*    淘宝地址： http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/
#ifndef __DS18B20_H
#define __DS18B20_H 

#include "main.h"   

//IO方向设置
#define DS18B20_IO_IN()  {GPIOC->MODER&=~(3<<(0*2));GPIOC->MODER|=0<<0*2;}	//PC0输入模式
#define DS18B20_IO_OUT() {GPIOC->MODER&=~(3<<(0*2));GPIOC->MODER|=1<<0*2;} 	//PC0输出模式
 
////IO操作函数											   
#define	DS18B20_DQ_OUT PCout(0) //数据端口	PC0
#define	DS18B20_DQ_IN  PCin(0)  //数据端口	PC0 
   	
u8 DS18B20_Init(void);			//初始化DS18B20
double DS18B20_Get_Temp(void);	//获取温度
void DS18B20_Start(void);		//开始温度转换
void DS18B20_Write_Byte(u8 dat);//写入一个字节
u8 DS18B20_Read_Byte(void);		//读出一个字节
u8 DS18B20_Read_Bit(void);		//读出一个位
u8 DS18B20_Check(void);			//检测是否存在DS18B20
void DS18B20_Rst(void);			//复位DS18B20    
#endif















