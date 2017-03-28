/*-------------------------------------------------*/
/*         超纬电子STM32F407ZGT6开发板             */
/*    淘宝地址： http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/
/*-------------------------------------------------*/
/*            待机唤醒源文件,有1个函数             */
/*-------------------------------------------------*/
/* void STM32_Standby(void)                        */
/*-------------------------------------------------*/

#include "wkup.h"
																	    
/*-------------------------------------------------*/
/*函数名：STM32进入待机状态                        */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void STM32_Standby(void)
{			
	RCC_AHB1PeriphResetCmd(RCC_AHB1Periph_GPIOA,ENABLE);  //复位所有A口
	RCC_AHB1PeriphResetCmd(RCC_AHB1Periph_GPIOB,ENABLE);  //复位所有B口
	RCC_AHB1PeriphResetCmd(RCC_AHB1Periph_GPIOC,ENABLE);  //复位所有C口
	RCC_AHB1PeriphResetCmd(RCC_AHB1Periph_GPIOD,ENABLE);  //复位所有D口
	RCC_AHB1PeriphResetCmd(RCC_AHB1Periph_GPIOE,ENABLE);  //复位所有E口
	RCC_AHB1PeriphResetCmd(RCC_AHB1Periph_GPIOF,ENABLE);  //复位所有F口
	RCC_AHB1PeriphResetCmd(RCC_AHB1Periph_GPIOG,ENABLE);  //复位所有G口
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);   //使能PWR时钟		
	PWR_ClearFlag(PWR_FLAG_WU);                           //清除Wake-up 标志
	PWR_WakeUpPinCmd(ENABLE);                             //设置WKUP用于唤醒
	PWR_EnterSTANDBYMode();                               //进入待机模式	 
} 















