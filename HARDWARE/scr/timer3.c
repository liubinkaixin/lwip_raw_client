/*-------------------------------------------------*/
/*          助开发网STM32F407ZGT6开发板            */
/*    淘宝地址： https://shop68304486.taobao.com   */
/*-------------------------------------------------*/
/*-----------------------------------------------------*/
/*           定时器3源文件,有1个函数                   */
/*-----------------------------------------------------*/
/*void TIM3_Init(u16 arr,u16 psc); 定时器3初始化       */
/*-----------------------------------------------------*/

#include "timer3.h"

/*-------------------------------------------------*/
/*函数名：定时器3初始化                            */
/*参  数：arr：自动重装值                          */
/*        psc：时钟预分频数                        */
/*返回值：无                                       */
/*-------------------------------------------------*/
void TIM3_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);           //使能TIM3时钟	
    TIM_TimeBaseInitStructure.TIM_Period = arr-1; 	              //自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc-1;                //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;     //1分频
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);            //按配置设置TIM3
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);                      //允许定时器3更新中断
	TIM_Cmd(TIM3,ENABLE);                                         //先不使能定时器3
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn;                 //定时器3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;       //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;               //子优先级1
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;                 //使能中断
	NVIC_Init(&NVIC_InitStructure);                               //按配置设置 
}
