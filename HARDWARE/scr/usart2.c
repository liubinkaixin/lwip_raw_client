/*-------------------------------------------------*/
/*         超纬电子STM32F407ZGT6开发板             */
/*    淘宝地址： http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/
/*-------------------------------------------------*/
/*            串口2源文件,有2个函数                */
/*-------------------------------------------------*/
/*  void Usart2_init(u32 bound);                   */
/*  void u2_printf(char* fmt,...);                 */
/*-------------------------------------------------*/

#include "main.h"
#include "usart2.h"	

u8  USART2_RX_BUF[USART2_RXBUFF_SIZE];     //接收缓冲数组
u16 USART2_RX_STA=0;                       //接收状态标记	

/*-------------------------------------------------*/
/*函数名：初始化串口2                              */
/*参  数：bound：波特率                            */
/*返回值：无                                       */
/*-------------------------------------------------*/
void Usart2_init(u32 bound)
{  	 	
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //使能GPIOA时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//使能USART2时钟
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2); //PA2复用为USART2
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2); //PA3复用为USART2
 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure);               //按配置设置
	
	USART_InitStructure.USART_BaudRate = bound;                                    //波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;                    //字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;                         //一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;                            //无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	               //收发模式
    USART_Init(USART2, &USART_InitStructure);                                      //按配置设置串口2
	
    USART_Cmd(USART2, ENABLE);                              //使能串口 2
	USART_ClearFlag(USART2, USART_FLAG_TC);	                //清除标志位
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);          //开启接受中断

    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;       //串口2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3; //抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//中断通道使能
	NVIC_Init(&NVIC_InitStructure);	                        //根据配置设置

}

 __align(8) u8  USART2_TX_BUF[USART2_TXBUFF_SIZE];          //串口2发送区

/*-------------------------------------------------*/
/*函数名：串口2printf                              */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void u2_printf(char* fmt,...)  
{  
	u16 i,length;
	
	va_list ap;
	va_start(ap,fmt);
	vsprintf((char*)USART2_TX_BUF,fmt,ap);
	va_end(ap);
	
	length=strlen((const char*)USART2_TX_BUF);	
	while((USART2->SR&0X40)==0);
	for(i = 0;i < length;i ++)
	{			
		USART2->DR = USART2_TX_BUF[i];
		while((USART2->SR&0X40)==0);	
	}	
}
