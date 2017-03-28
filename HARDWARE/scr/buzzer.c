/*-------------------------------------------------*/
/*          超纬电子STM32F407ZGT6开发板            */
/*    淘宝地址： http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/
/*-------------------------------------------------*/
/*            蜂鸣器源文件,有1个函数               */
/*-------------------------------------------------*/
/* void Buzzer_Init(void);  初始化蜂鸣器控制引脚   */
/*-------------------------------------------------*/

#include "buzzer.h" 
		    
/*-------------------------------------------------*/
/*函数名：初始化蜂鸣器控制引脚函数                 */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void Buzzer_Init(void)
{    	 
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(BUZZER_GROUP_EN, ENABLE);     //使能控制蜂鸣器的IO组

	/*蜂鸣器引脚初始化*/
	GPIO_InitStructure.GPIO_Pin   = BUZZER_PIN;          //控制蜂鸣器的引脚标号
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;       //普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;       //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;   //100MHz
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;        //上拉
	GPIO_Init(BUZZER_GROUP, &GPIO_InitStructure);        //按设置初始化
	
	BUZZE=0;                                             //初始化后，关闭蜂鸣器
}






