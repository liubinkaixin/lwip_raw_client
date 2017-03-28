/*-------------------------------------------------*/
/*          超纬电子STM32F407ZGT6开发板            */
/*    淘宝地址： http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/
/*-------------------------------------------------*/
/*            按键源文件,有2个函数                 */
/*-------------------------------------------------*/
/* void KEY_Init(void);	   按键初始化              */     
/* u8 KEY_Scan(u8 mode);   按键处理函数	           */
/*-------------------------------------------------*/

#include "key.h"
#include "delay.h" 

/*-------------------------------------------------*/
/*函数名：初始化KEY按键函数                        */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void KEY_Init(void)
{	
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(KEY_GROUP_EN, ENABLE);         //使能控制按键的IO组

	/*初始化K1的引脚*/
	GPIO_InitStructure.GPIO_Pin = KEY1_PIN;                //控制K1的IO编号
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;           //普通输入模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;     //100M
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;         //下拉
	GPIO_Init(KEY1_GROUP, &GPIO_InitStructure);            //按配置初始化.
	
	/*初始化K2的引脚*/
	GPIO_InitStructure.GPIO_Pin = KEY2_PIN;                //控制K2的IO编号
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;           //普通输入模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;     //100M
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;           //上拉
	GPIO_Init(KEY2_GROUP, &GPIO_InitStructure);            //按配置初始化
	
	/*初始化K3的引脚*/
	GPIO_InitStructure.GPIO_Pin = KEY3_PIN;                //控制K3的IO编号
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;           //普通输入模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;     //100M
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;           //上拉
	GPIO_Init(KEY3_GROUP, &GPIO_InitStructure);            //按配置初始化
	
	/*初始化K4的引脚*/
	GPIO_InitStructure.GPIO_Pin = KEY4_PIN;                //控制K4的IO编号
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;           //普通输入模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;     //100M
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;           //上拉
	GPIO_Init(KEY4_GROUP, &GPIO_InitStructure);            //按配置初始化	
} 

/*-------------------------------------------------*/
/*函数名：按键处理函数                             */
/*参  数：mode：0-按下后触发   1-释放后触发        */
/*返回值：按键值                                   */
/*-------------------------------------------------*/
u8 KEY_Scan(u8 mode)
{	 	  
	u8 key_num;                //保存按键值

	key_num=NONE_PRES_NUM;     //先设置为无按键值(0)
	if(KEY1==1||KEY2==0||KEY3==0||KEY4==0)
	{
		delay_ms(5);                              //去抖动 
		if(KEY1==1)        key_num = KEY1_PRES_NUM;//返回K1键值
		else if(KEY2==0)   key_num = KEY2_PRES_NUM;//返回K2键值
		else if(KEY3==0)   key_num = KEY3_PRES_NUM;//返回K3键值
		else if(KEY4==0)   key_num = KEY4_PRES_NUM;//返回K4键值
	} 
	if(mode)                                       //mode位1的时候，释放按键后才触发
		while(KEY1==1||KEY2==0||KEY3==0||KEY4==0); //松手检测
	
	return key_num;                                //返回键值
}




















