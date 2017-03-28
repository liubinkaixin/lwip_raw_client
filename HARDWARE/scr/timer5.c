/*-------------------------------------------------*/
/*         超纬电子STM32F407ZGT6开发板             */
/*    淘宝地址： http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/
/*-------------------------------------------------*/
/*            定时器5源文件,有1个函数              */
/*-------------------------------------------------*/
/* void TIM5_CH1_Cap_Init(u32 arr,u16 psc)         */
/*-------------------------------------------------*/

#include "timer5.h"

TIM_ICInitTypeDef  TIM5_ICInitStructure;

/*-------------------------------------------------*/
/*函数名：定时器5通道1输入捕获配置                 */
/*参  数：arr：自动重装值                          */
/*        psc：时钟预分频数                        */
/*返回值：无                                       */
/*-------------------------------------------------*/
void TIM5_CH1_Cap_Init(u32 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);  	//TIM5时钟使能    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	//使能PORTA时钟	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;               //GPIOA0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;            //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	    //速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;          //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;          //下拉
	GPIO_Init(GPIOA,&GPIO_InitStructure);                   //初始化PA0

	GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_TIM5);   //PA0复用位定时器5
 	  
	TIM_TimeBaseStructure.TIM_Prescaler=psc-1;                //定时器分频
	TIM_TimeBaseStructure.TIM_Period=arr-1;                   //自动重装载值
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式	
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;     //1分配	
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseStructure);            //按配置设置
	
	//初始化TIM5输入捕获参数
	TIM5_ICInitStructure.TIM_Channel = TIM_Channel_1;                //输入端 IC1映射到通道1上
	TIM5_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	 //上升沿捕获
	TIM5_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI1上
	TIM5_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	         //1分频 
	TIM5_ICInitStructure.TIM_ICFilter = 0x00;                        //不滤波
	TIM_ICInit(TIM5, &TIM5_ICInitStructure);                         //按配置设置
		
	TIM_ITConfig(TIM5,TIM_IT_Update|TIM_IT_CC1,ENABLE);              //允许更新中断 ,允许CC1IE捕获中断		
    TIM_Cmd(TIM5,ENABLE );                                        	 //使能定时器5

    NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;                  //定时器5中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;          //抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		         //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			         //IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	                                 //按配置设置	
}
