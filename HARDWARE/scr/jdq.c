/*-------------------------------------------------*/
/*         超纬电子STM32F407ZGT6开发板             */
/*    淘宝地址： http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/
/*-------------------------------------------------*/
/*            继电器源文件,有1个函数               */
/*-------------------------------------------------*/

/*-------------------------------------------------*/

#include "jdq.h" 
		    
/*-------------------------------------------------*/
/*函数名：初始化继电器函数                            */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void JDQ_Init(void)
{    	 
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(JDQ_GROUP_EN, ENABLE);           //使能控制JDQ的IO组

	/*JDQ引脚初始化*/
	GPIO_InitStructure.GPIO_Pin   = JDQ_PIN;            //控制D8的引脚标号
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;      //普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  //100MHz
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;       //上拉
	GPIO_Init(JDQ_GROUP, &GPIO_InitStructure);          //按设置初始化
	
	JDQ_IO=1; //高电平，初始是 ，关闭电磁锁
}





