/*-------------------------------------------------*/
/*         超纬电子STM32F407ZGT6开发板             */
/*    淘宝地址： http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/
/*-------------------------------------------------*/
/*            多彩LED源文件,有2个函数              */
/*-------------------------------------------------*/
/* void TIM4_PWM_Init(u32 arr,u32 psc);            */  
/* void TIM3_PWM_Init(u32 arr,u32 psc);            */
/*-------------------------------------------------*/

#include "rgb_led.h"

/*-------------------------------------------------*/
/*函数名：定时器4PWM初始化                         */
/*参  数：arr：自动重装值                          */
/*参  数：psc：时钟预分频数                        */
/*返回值：无                                       */
/*-------------------------------------------------*/
void TIM4_PWM_Init(u32 arr,u32 psc)
{		 					 
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  	//TIM4时钟使能    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 	//使能PORTB时钟		
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource6,GPIO_AF_TIM4);   //PB6复用为定时器4pwm

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;           //PB6          
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
	GPIO_Init(GPIOB,&GPIO_InitStructure);               //按配置设置
	  
	TIM_TimeBaseStructure.TIM_Prescaler=psc-1;                //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=arr-1;                   //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 	  //1分配
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);            //按配置设置
	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;             //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;      //输出极性:TIM输出比较极性低
	
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);           //配置设置定时器4的通道1
    TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM4在通道1上的预装载寄存器
 
    TIM_ARRPreloadConfig(TIM4,ENABLE);//ARPE使能 	
	TIM_Cmd(TIM4, ENABLE);            //使能TIM4										  
}  

/*-------------------------------------------------*/
/*函数名：定时器3PWM初始化                         */
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
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource4,GPIO_AF_TIM4);   //PB4复用为定时器3PWM
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource5,GPIO_AF_TIM4);   //PB5复用为定时器3PWM
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;    //PB4/5         
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;            //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	    //速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;          //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;            //上拉
	GPIO_Init(GPIOB,&GPIO_InitStructure);                   //按配置设置
	  
	TIM_TimeBaseStructure.TIM_Prescaler=psc-1;                //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=arr-1;                   //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;     //1分频 	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);            //按配置设置
		 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;             //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;      //输出极性:TIM输出比较极性低
	
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);           //配置设置定时器3的通道1
    TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM3在通道1上的预装载寄存器
	
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);           //配置设置定时器3的通道2
    TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM3在通道2上的预装载寄存器
 
    TIM_ARRPreloadConfig(TIM3,ENABLE);//ARPE使能 	
	TIM_Cmd(TIM3, ENABLE);            //使能TIM3										  
} 

/*-------------------------------------------------*/
/*函数名：3色灯IO口初始化                          */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void RGB_LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  	//TIM3时钟使能    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 	//使能PORTB时钟	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;    //PB4.5.6      
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;           //输出功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	    //速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;          //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;            //上拉
	GPIO_Init(GPIOB,&GPIO_InitStructure);                   //按配置设置	

	LED_Bout=1;         
	LED_Rout=1;        
	LED_Gout=1;  	
}
/*-------------------------------------------------*/
/*函数名：设置蓝色PWM                              */
/*参  数：num：修改占空比，改变亮度                */
/*返回值：无                                       */
/*-------------------------------------------------*/
void BLUE_LED(u8 num)
{
	TIM_SetCompare1(TIM4,num);	//修改比较值，修改占空比
}

/*-------------------------------------------------*/
/*函数名：设置红色PWM                              */
/*参  数：num：修改占空比，改变亮度                */
/*返回值：无                                       */
/*-------------------------------------------------*/
void RED_LED(u8 num)
{
	TIM_SetCompare2(TIM3,num);	//修改比较值，修改占空比
}

/*-------------------------------------------------*/
/*函数名：设置绿色PWM                              */
/*参  数：num：修改占空比，改变亮度                */
/*返回值：无                                       */
/*-------------------------------------------------*/
void GREEN_LED(u8 num)
{
	TIM_SetCompare1(TIM3,num);	//修改比较值，修改占空比
}


