/*-------------------------------------------------*/
/*          超纬电子STM32F407ZGT6开发板            */
/*    淘宝地址： http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/
/*-----------------------------------------------------*/
/*           定时器2源文件,有1个函数                   */
/*-----------------------------------------------------*/
/*void TIM2_Init(u16 arr,u16 psc); 定时器2初始化       */
/*-----------------------------------------------------*/

#include "timer2.h"

/*-------------------------------------------------*/
/*函数名：定时器2初始化                            */
/*参  数：arr：自动重装值                          */
/*        psc：时钟预分频数                        */
/*返回值：无                                       */
/*-------------------------------------------------*/
void TIM2_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);           //使能TIM2时钟	
    TIM_TimeBaseInitStructure.TIM_Period = arr-1; 	              //自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc-1;                //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;     //1分频
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);            //按配置设置TIM2	
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);                      //允许定时器4更新中断
    TIM_ClearITPendingBit(TIM2,TIM_IT_Update);                    //清除中断标志位
	TIM_Cmd(TIM2,DISABLE); 
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn;                 //定时器2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;       //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;              //子优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;                 //使能中断
	NVIC_Init(&NVIC_InitStructure);                               //按配置设置 
	                                      //不使能定时器2   
}


