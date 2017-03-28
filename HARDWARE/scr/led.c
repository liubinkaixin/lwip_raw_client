/*-------------------------------------------------*/
/*         超纬电子STM32F407ZGT6开发板             */
/*    淘宝地址： http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/
/*-------------------------------------------------*/
/*            LED源文件,有3个函数                  */
/*-------------------------------------------------*/
/*void LED_Init(void);     初始化	               */
/*void LED_AllOn(void);    点亮所有LED             */
/*void LED_AllOff(void);   熄灭所有LED             */
/*-------------------------------------------------*/

#include "led.h" 
		    
/*-------------------------------------------------*/
/*函数名：初始化LED函数                            */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void LED_Init(void)
{    	 
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(LED_GROUP, ENABLE);           //使能控制LED的IO组

	/*LED8引脚初始化*/
	GPIO_InitStructure.GPIO_Pin   = LED8_PIN;            //控制D8的引脚标号
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;       //普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;       //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;   //100MHz
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;        //上拉
	GPIO_Init(LED8_GROUP, &GPIO_InitStructure);          //按设置初始化
	
	/*LED9引脚初始化*/
	GPIO_InitStructure.GPIO_Pin   = LED9_PIN;            //控制D9的引脚标号
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;       //普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;       //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;   //100MHz
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;        //上拉
	GPIO_Init(LED9_GROUP, &GPIO_InitStructure);          //按设置初始化
	
	/*LED10引脚初始化*/
	GPIO_InitStructure.GPIO_Pin   = LED10_PIN;           //控制D10的引脚标号
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;       //普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;       //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;   //100MHz
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;        //上拉
	GPIO_Init(LED10_GROUP, &GPIO_InitStructure);         //按设置初始化
	
	LED_AllOff();       //初始化引脚后，所有LED处于熄灭状态
}

/*-------------------------------------------------*/
/*函数名：点亮所有LED                              */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void LED_AllOn(void)
{
	GPIO_ResetBits(LED8_GROUP,LED8_PIN);   //点亮D8
	GPIO_ResetBits(LED9_GROUP,LED9_PIN);   //点亮D9
	GPIO_ResetBits(LED10_GROUP,LED10_PIN); //点亮D10
}

/*-------------------------------------------------*/
/*函数名：熄灭所有LED                              */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void LED_AllOff(void)
{
	GPIO_SetBits(LED8_GROUP,LED8_PIN);     //熄灭D8
	GPIO_SetBits(LED9_GROUP,LED9_PIN);     //熄灭D8
	GPIO_SetBits(LED10_GROUP,LED10_PIN);   //熄灭D8
}





