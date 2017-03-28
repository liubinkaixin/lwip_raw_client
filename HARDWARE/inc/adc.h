/*-------------------------------------------------*/
/*         超纬电子STM32F407ZGT6开发板             */
/*    淘宝地址： http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/

#ifndef __ADC_H
#define __ADC_H	

#include "main.h" 
							   
void Adc_Init(void); 				//初始化ADC
u16  Get_Adc(u8 ch); 				//获得ADC结果 
u16 Get_Adc_Average(u8 ch,u8 times);//平均多次ADC结果，提高精度

#endif 
