/*-------------------------------------------------*/
/*         超纬电子STM32F407ZGT6开发板             */
/*    淘宝地址： http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/
/*-------------------------------------------------*/
/*            RS485源文件,有2个函数                */
/*-------------------------------------------------*/
/* void RS485_Init(u32 bound)       485初始化串口3 */
/* void RS485_printf(char* fmt,...) 485printf函数  */
/*-------------------------------------------------*/

#include "rs485.h"	 
#include "delay.h"
 
//接收缓存区 	
u8 RS485_RX_BUF[RS485_RXBUFF_SIZE];  
//接收状态标记
u16 RS485_RX_STA=0;   
										 
/*-------------------------------------------------*/
/*函数名：初始化和485连接的串口3                   */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void RS485_Init(u32 bound)
{  	 
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); //使能GPIOB时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE); //使能GPIOF时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//使能USART3时钟
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3); //PB10复用为USART3
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3); //PB11复用为USART3
	   
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;//PB10/11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;            //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	    //速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;          //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;            //上拉
	GPIO_Init(GPIOB,&GPIO_InitStructure);                   //按配置设置
	   
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;              //PF11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;           //输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	    //速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;          //推挽输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;            //上拉
	GPIO_Init(GPIOF,&GPIO_InitStructure);                   //按配置设置
	
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
	
	RS485_RX_TX=0;				                            //接收模式	
}

/*-------------------------------------------------*/
/*函数名：485 printf函数                           */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/

__align(8) u8  RS485_TX_BUF[RS485_TXBUFF_SIZE];   //串口2发送区

void RS485_printf(char* fmt,...)  
{  
	u16 i,length;
	
	va_list ap;
	va_start(ap,fmt);
	vsprintf((char*)RS485_TX_BUF,fmt,ap);
	va_end(ap);
	
	RS485_RX_TX=1;  //发送模式
	delay_ms(1);
	
	length=strlen((const char*)RS485_TX_BUF);	
	while((USART3->SR&0X40)==0);
	for(i = 0;i < length;i ++)
	{			
		USART3->DR = RS485_TX_BUF[i];
		while((USART3->SR&0X40)==0);	
	}	
	
	RS485_RX_TX=0;  //接收模式
	delay_ms(1);
}

