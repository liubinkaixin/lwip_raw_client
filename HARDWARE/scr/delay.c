/*-------------------------------------------------*/
/*         助开发网STM32F407ZGT6开发板             */
/*    淘宝地址： https://shop68304486.taobao.com   */
/*-------------------------------------------------*/
/*-------------------------------------------------*/
/*            延时源文件,有3个函数                 */
/*-------------------------------------------------*/
/*void delay_init(void);        延时初始化函数     */
/*void delay_ms(u32 ms);        延时毫秒函数       */
/*void delay_us(u32 us);        延时微秒函数       */
/*-------------------------------------------------*/

#include "delay.h"

/*-------------------------------------------------*/
/*函数名：初始化延迟函数                           */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void delay_init(void)
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	//SysTick（滴答时钟）的频率是主频率的1/8
	//通常主频率为最大的168M，那么SysTick的频率是21M
	//那么SysTick计数器1个数，代表(1/21)us
}

/*-------------------------------------------------*/
/*函数名：延迟微秒函数                             */
/*参  数：us：延时多少微秒                         */
/*返回值：无                                       */
/*-------------------------------------------------*/
void delay_us(u32 us)
{		
	u32 temp;	    	 
	SysTick->LOAD=us*21;                      //SysTick加载值，delay_init函数中我们知道， SysTick计数器一个数是1/21微秒，所以乘以21
	SysTick->VAL=0x00;                        //清空计数器
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;  //开始倒数 
	do
	{
		temp=SysTick->CTRL;                        //读取SysTick状态寄存器
	}
	while((temp&0x01)&&!(temp&(1<<16)));           //等待时间到达，标志位置位   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;       //关闭计数器
	SysTick->VAL =0X00;                            //清空计数器	 
}

/*-------------------------------------------------*/
/*函数名：延迟毫秒函数                             */
/*参  数：ms：延时多少毫秒                         */
/*返回值：无                                       */
/*-------------------------------------------------*/
void delay_ms(u32 ms)
{
	//我们首先注意一个问题SysTick时钟计数器是24位的，21M频率下，总共能延时798.915ms
	//所有我们以700为界限，小于700的延时一次计数就行，大于700的多次计数	
	u32 i;
	u32 temp;
	
	if(ms<700)   //小于700ms
	{
		SysTick->LOAD=(u32)ms*21*1000;           //时间加载,SysTick一个数使1/21微秒，换算成ms的话，乘以21再乘以1000
		SysTick->VAL =0x00;                       //清空计数器
		SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;  //开始倒数  
		do
		{
			temp=SysTick->CTRL;                        //读取SysTick状态寄存器
		}
		while((temp&0x01)&&!(temp&(1<<16)));           //等待时间到达，标志位置位   
		SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;       //关闭计数器
		SysTick->VAL =0X00;                            //清空计数器	 
	}
	else   //大于700ms
	{
		for(i=0;i<(ms/700);i++)    //除以700整数部分，for循环的延时
		{
			SysTick->LOAD=700*21*1000;                //时间加载,700ms延时
			SysTick->VAL =0x00;                       //清空计数器
			SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;  //开始倒数  
			do
			{
				temp=SysTick->CTRL;                        //读取SysTick状态寄存器
			}
			while((temp&0x01)&&!(temp&(1<<16)));           //等待时间到达，标志位置位   
			SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;       //关闭计数器
			SysTick->VAL =0X00;                            //清空计数器	 
		}
		//接下来余数部分的延时
		SysTick->LOAD=(u32)(ms%700)*21*1000;           //时间加载,余数部分
		SysTick->VAL =0x00;                       //清空计数器
		SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;  //开始倒数  
		do
		{
		temp=SysTick->CTRL;                       //读取SysTick状态寄存器
		}
		while((temp&0x01)&&!(temp&(1<<16)));           //等待时间到达，标志位置位   
		SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;       //关闭计数器
		SysTick->VAL =0X00;                            //清空计数器	
	}
} 

/*-------------------------------------------------*/
/*函数名：延迟纳秒函数-非精确，做延时用            */
/*参  数：ns：延时多少纳秒                         */
/*返回值：无                                       */
/*-------------------------------------------------*/
void delay_ns(u32 ns)
{
  u32 i;
  for(i=0;i<ns;i++)
  {
    __nop();
    __nop();
    __nop();
  }
}	


































