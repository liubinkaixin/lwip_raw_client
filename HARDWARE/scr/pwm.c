/*-------------------------------------------------*/
/*         超纬电子STM32F407ZGT6开发板             */
/*    淘宝地址： http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/
/*-------------------------------------------------*/
/*            PWM源文件,有个1函数                  */
/*-------------------------------------------------*/
/*  void TIM3_PWM_Init(u32 arr,u32 psc)            */
/*-------------------------------------------------*/

#include "pwm.h"

/*-------------------------------------------------*/
/*函数名：TIM3通道3的PWM初始化                     */
/*参  数：arr：自动重装值                          */
/*参  数：psc：时钟预分频数                        */
/*返回值：无                                       */
/*-------------------------------------------------*/
void TIM3_PWM_Init(u32 arr,u32 psc)
{		 					 	
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  	//TIM3时钟使能    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 	//使能PORTB时钟	
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource0,GPIO_AF_TIM3);   //PB0复用为定时器3
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;           //PB0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
	GPIO_Init(GPIOB,&GPIO_InitStructure);               //按配置设置
	  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;                  //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=arr;                     //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;     //1分频 
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);            //按配置设置
		 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;                  //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;      //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;           //输出极性:TIM输出比较极性低
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);                           //按配置设置

	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);                 //使能TIM3在通道3上的预装载寄存器 
    TIM_ARRPreloadConfig(TIM3,ENABLE);                                //ARPE使能 	
	TIM_Cmd(TIM3, ENABLE);                                            //使能TIM4										  
}  


