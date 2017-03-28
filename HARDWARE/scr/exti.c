/*-------------------------------------------------*/
/*          超纬电子STM32F407ZGT6开发板            */
/*    淘宝地址： http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/
/*-------------------------------------------------*/
/*          外部中断源文件,有1个函数               */
/*-------------------------------------------------*/
/* void KEY_EXTI_Init(void);	外部中断初始化     */     
/*-------------------------------------------------*/

#include "exti.h"
#include "key.h"

/*-------------------------------------------------*/
/*函数名：按键外部中断初始化程序.                  */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/	   
void KEY_EXTI_Init(void)
{
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;

	KEY_Init();       //初始化按键

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);     //使能SYSCFG时钟

	SYSCFG_EXTILineConfig(KEY1_EXTI_GPOUP, KEY1_EXTI_SOURCE);  //配置KEY1所在的中断源
	SYSCFG_EXTILineConfig(KEY2_EXTI_GPOUP, KEY2_EXTI_SOURCE);  //配置KEY2所在的中断源
	SYSCFG_EXTILineConfig(KEY3_EXTI_GPOUP, KEY3_EXTI_SOURCE);  //配置KEY3所在的中断源
	SYSCFG_EXTILineConfig(KEY4_EXTI_GPOUP, KEY4_EXTI_SOURCE);  //配置KEY4所在的中断源

	EXTI_InitStructure.EXTI_Line    = KEY1_EXTI_LINE;           //KEY1所在中断线
	EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;      //中断事件
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;      //上降沿触发
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;                   //中断线使能
	EXTI_Init(&EXTI_InitStructure);                             //配置中断	
	
	EXTI_InitStructure.EXTI_Line    = KEY2_EXTI_LINE | KEY3_EXTI_LINE  |KEY4_EXTI_LINE;  //KEY2.3.4所在中断线
	EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;      //中断事件
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;     //下降沿触发
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;                   //中断线使能
	EXTI_Init(&EXTI_InitStructure);                             //配置中断

	NVIC_InitStructure.NVIC_IRQChannel = KEY1_EXTI_IRQ ;        //按键1所在中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;//抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;       //子优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);                             //配置 

	NVIC_InitStructure.NVIC_IRQChannel = KEY2_EXTI_IRQ ;        //按键1所在中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;//抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;       //子优先级1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);                             //配置
	
	NVIC_InitStructure.NVIC_IRQChannel = KEY3_EXTI_IRQ ;        //按键1所在中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;//抢占优先级2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;       //子优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);                             //配置
	
	NVIC_InitStructure.NVIC_IRQChannel = KEY4_EXTI_IRQ ;        //按键1所在中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;       //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);                             //配置
}












