/*-------------------------------------------------*/
/*         超纬电子STM32F407ZGT6开发板             */
/*    淘宝地址： http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/
/*-------------------------------------------------*/
/*            串口3源文件,有2个函数                */
/*-------------------------------------------------*/
/* void Usart3_init(u32 bound)   初始化串口3       */
/* void u3_printf(char* fmt,...) 串口3printf函数   */
/*-------------------------------------------------*/

#include "usart3.h"	 
#include "delay.h"
#include "timer3.h"
 
//接收缓存区 	
u8 USART3_RX_BUF[USART3_RXBUFF_SIZE];  
//接收状态标记
u16 USART3_RX_STA=0;   
										 
/*-------------------------------------------------*/
/*函数名：初始化串口3                              */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void Usart3_init(u32 bound)
{  	 
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); //使能GPIOB时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//使能USART3时钟
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3); //PB10复用为USART3
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3); //PB11复用为USART3
	   
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;//PB10/11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;            //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	    //速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;          //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;            //上拉
	GPIO_Init(GPIOB,&GPIO_InitStructure);                   //按配置设置
	   
	USART_InitStructure.USART_BaudRate = bound;                  //波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;  //字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;       //一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;          //无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	               //收发模式
    USART_Init(USART3, &USART_InitStructure);                                      //按配置设置串口3
	
    USART_Cmd(USART3, ENABLE);                              //使能串口3	
	USART_ClearFlag(USART3, USART_FLAG_TC);                 //清除标志位
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);          //开启接受中断

    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;       //串口3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3; //抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	                        //根据指定的参数初始化VIC寄存器、
	
	TIM3_Init(200,8400);	                                //定时器时钟84M，分频系数8400，所以84M/8400=10Khz的计数频率，计数200次为20ms 
    USART3_RX_STA=0;                                        //串口3状态标识清零  
}

/*-------------------------------------------------*/
/*函数名：串口3printf函数                           */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/

__align(8) u8  USART3_TX_BUF[USART3_TXBUFF_SIZE];   //串口2发送区

void u3_printf(char* fmt,...)  
{  
	u16 i,length;
	
	va_list ap;
	va_start(ap,fmt);
	vsprintf((char*)USART3_TX_BUF,fmt,ap);
	va_end(ap);
		
	length=strlen((const char*)USART3_TX_BUF);	
	while((USART3->SR&0X40)==0);
	for(i = 0;i < length;i ++)
	{			
		USART3->DR = USART3_TX_BUF[i];
		while((USART3->SR&0X40)==0);	
	}	
}

