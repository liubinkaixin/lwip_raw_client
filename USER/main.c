/*-------------------------------------------------*/
/*          助开发网STM32F407ZGT6开发板            */
/*    淘宝地址： https://shop68304486.taobao.com   */
/*-------------------------------------------------*/
/*      76-以太网通信之TCP客户端(裸奔-动态IP)      */
/*-------------------------------------------------*/ 
/*             例程简介详看readme.txt              */
/*-------------------------------------------------*/ 

#include "main.h"
#include "delay.h" 
#include "usart1.h" 
#include "8720.h" 
#include "tcpclient.h" 
#include "timer3.h"
#include "CriticalSeg.h"

u8  Server_flag=0;       //标志位 0:未收到服务器的IP和端口号信息   1：收到了服务器的IP和端口号信息
u8  server_ip[20]="121.40.173.197";       //服务器IP
//u8  server_ip[20]="192.168.6.105";       //服务器IP
u16 server_port=8823;         //服务器端口号0-9999

void data_led_cpl(void)
{
	GPIO_WriteBit(GPIOC,GPIO_Pin_6,(BitAction)(1 - GPIO_ReadOutputDataBit(GPIOC,GPIO_Pin_6)));
}

void dma_led_cpl(void)
{
	GPIO_WriteBit(GPIOC,GPIO_Pin_2,(BitAction)(1 - GPIO_ReadOutputDataBit(GPIOC,GPIO_Pin_2)));
}

void connect_led(u8 sts)
{
	GPIO_WriteBit(GPIOC,GPIO_Pin_7,(BitAction)(sts));
}

void fault_led_show(void)
{
	GPIO_WriteBit(GPIOC,GPIO_Pin_8,(BitAction)(1));
}

void led_gpio_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//使能GPIO时钟 RMII接口
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2| GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	//GPIO_ReadOutputDataBit
	GPIO_WriteBit(GPIOC,GPIO_Pin_6,(BitAction)(0));
	GPIO_WriteBit(GPIOC,GPIO_Pin_7,(BitAction)(0));
	GPIO_WriteBit(GPIOC,GPIO_Pin_8,(BitAction)(0));
}

int main(void)
{	
//	u16 len;
	
	delay_init();		        //延时初始化 
	Usart1_init(115200);        //串口1初始化
	printf("hello www.zkaifa.com\r\n");
	Server_flag=0;
	led_gpio_init();
	Server_flag=1;
	
	GPIO_Configuration();       //以太网接口配置
	NVIC_Configuration();       //以太网中断配置
	Ethernet_Configuration();   //以太网配置
  TIM3_Init(100,8400);	    //10ms的延时
	MylwIP_Init();	            //初始化lwip启动前的事宜
	TCP_Client_Init(TCP_LOCAL_PORT,server_ip,server_port);   //客户端初始化
	//OS_ENTER_CRITICAL;	
	while (1)
	{    
		lwip_periodic_handle(); //lwIP周期处理任务
		echo_snd();
	}
}
