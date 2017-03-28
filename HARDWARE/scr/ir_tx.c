/*-------------------------------------------------*/
/*         超纬电子STM32F407ZGT6开发板             */
/*    淘宝地址： http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/
/*-------------------------------------------------*/
/*            红外发送源文件,有个函数             */
/*-------------------------------------------------*/

/*-------------------------------------------------*/

#include "ir_tx.h"
#include "delay.h"

/*-------------------------------------------------*/
/*函数名：红外发送初始化                           */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void IR_SentInit(u32 arr,u32 psc)
{		 					 
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  	//TIM3时钟使能    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); 	//使能PORTC时钟	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);  //使能SYSCFG时钟
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_7;          //PC7
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;       //普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;       //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;   //100MHz
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;        //上拉
	GPIO_Init(GPIOC, &GPIO_InitStructure);               //按设置初始化
	
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_TIM3);   //PC6复用为定时器3	
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6;          //PC6
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
	GPIO_Init(GPIOC,&GPIO_InitStructure);               //按配置设置
	  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;                  //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=arr;                     //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;     //1分配
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);            //按配置设置
		 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;             
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     
	
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);           
    TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable); 

    TIM_ARRPreloadConfig(TIM3,ENABLE);                 	
	TIM_Cmd(TIM3, ENABLE);  	

	TIM_SetCompare1(TIM3,9);  //修改比较值，修改占空比
} 

/*-------------------------------------------------*/
/*函数名：发送一个二进制位0                        */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void Sent_0(void)     //560us低电平+560us低电平  代表一个0
{
	IR_SEND=0;
	delay_us(560);    //560us低电平
	IR_SEND=1; 
	delay_us(560);	  //560us低电平
}

/*-------------------------------------------------*/
/*函数名：发送一个二进制位1                        */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void Sent_1(void)    //560us低电平+1680us低电平  代表一个1
{
	IR_SEND=0;
	delay_us(560);  //560us低电平
	IR_SEND=1;
	delay_us(1680);	//1680us低电平
}

/*-------------------------------------------------*/
/*函数名：发送红外信号                             */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void IR_Send(u8 data)   //data是键值
{
	u8 i,temp;
	u8 t_data;
	
	t_data=~data;  //t_data是data的反码
	
	IR_SEND=0;     
	delay_us(9000);//9ms高电平
	IR_SEND=1;
	delay_us(4500);//4.5ms低电平  同步码

	for(i=0;i<8;i++)   //用户码1，我们自定义0x00
	{
		Sent_0();		
	}
	for(i=0;i<8;i++)   //用户码2，我们自定义0xFF
	{
		Sent_1();	
	}
	for(i=0;i<8;i++)   //键值，一位一位的发
	{
		temp=(data&0x80);
		if(temp)Sent_1();	
		else Sent_0();
		data<<=1;
	}
	for(i=0;i<8;i++)   //反键值，一位一位的发
	{
		temp=(t_data&0x80);
		if(temp)Sent_1();	
		else Sent_0();
		t_data<<=1;
	}

	Sent_0();      //停止码
}






























