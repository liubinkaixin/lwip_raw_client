/*-------------------------------------------------*/
/*         超纬电子STM32F407ZGT6开发板             */
/*    淘宝地址： http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/
/*-------------------------------------------------*/
/*            蓝牙源文件,有个函数                  */
/*-------------------------------------------------*/

#include "blue.h" 
#include "usart3.h" 
#include "timer3.h" 
#include "delay.h" 

/*-------------------------------------------------*/
/*函数名：初始化蓝牙                               */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void Blue_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);   //使能PB时钟

	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_15;        
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;      //普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  //100MHz
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;     //下拉
	GPIO_Init(GPIOB, &GPIO_InitStructure);              //按设置初始化                                      //进入AT

	Usart3_init(38400);	    //串口初始化波特率为38400 
	TIM3_Init(200,8400);    //定时器3初始化,8400分配，200个计数，超市时间20ms	
}

/*-------------------------------------------------*/
/*函数名：蓝牙进入AT模式                           */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void Blue_InAT(void)
{
	BLUE_KEY=1;  //进入AT
}

/*-------------------------------------------------*/
/*函数名：蓝牙退出AT模式                           */
/*参  数：无                                       */
/*返回值：0：正确  其他：错误                      */
/*-------------------------------------------------*/
u8 Blue_QuitAT(void)
{
	BLUE_KEY=0;                                 //拉低KEY
	delay_ms(100);
	if(Blue_SendCmd_OK("AT+RESET",20))return 1; //发送复位指令，2s超时时间，错误返回1
	return 0;                                   //正确返回0
}

/*-------------------------------------------------*/
/*函数名：发送AT指令，判断回复OK                   */
/*参  数：cmd：指令                                */
/*参  数：timeout：等待回复的超时时间              */
/*返回值：0：正确  其他：错误                      */
/*-------------------------------------------------*/
u8 Blue_SendCmd_OK(u8 *cmd, u16 timeout)
{
    u16 time=0;
	
	USART3_RX_STA=0;                        
	memset(USART3_RX_BUF,0,USART3_RXBUFF_SIZE);//清除串口3缓冲区数据	
	u3_printf("%s\r\n",cmd);                   //发送指令
	time=0;
	while(time<timeout)
	{
		if(strstr( (const char *)USART3_RX_BUF , (const char *)"OK"))//如果接受到OK表示指令成功
			break;
		delay_ms(100);//延时100ms
		time++;      
	}
	if(time>=timeout)return 1;                //超时错误，返回1
	else return 0;		                      //无错，返回0
}

/*-------------------------------------------------*/
/*函数名：设置蓝牙模块                             */
/*参  数：无                                       */
/*返回值：0：正确  其他：错误                      */
/*-------------------------------------------------*/
u8 Blue_Setup(void)
{                                     
	Blue_InAT();                                        //进入AT模式
	if(Blue_SendCmd_OK("AT+NAME=CHAOWEI",20))return 1;  //设置蓝牙名字，2s超时时间,出错的话返回1.
	if(Blue_SendCmd_OK("AT+ROLE=0",20))return 2;        //设置蓝牙为从机，2s超时时间,出错的话返回2
	if(Blue_SendCmd_OK("AT+CMODE=1",20))return 3;       //设置蓝牙任意连接模式，2s超时时间,出错的话返回3
	return 0;                                           //正确返回0
}


		    
