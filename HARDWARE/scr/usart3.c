/*-------------------------------------------------*/
/*         ��γ����STM32F407ZGT6������             */
/*    �Ա���ַ�� http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/
/*-------------------------------------------------*/
/*            ����3Դ�ļ�,��2������                */
/*-------------------------------------------------*/
/* void Usart3_init(u32 bound)   ��ʼ������3       */
/* void u3_printf(char* fmt,...) ����3printf����   */
/*-------------------------------------------------*/

#include "usart3.h"	 
#include "delay.h"
#include "timer3.h"
 
//���ջ����� 	
u8 USART3_RX_BUF[USART3_RXBUFF_SIZE];  
//����״̬���
u16 USART3_RX_STA=0;   
										 
/*-------------------------------------------------*/
/*����������ʼ������3                              */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void Usart3_init(u32 bound)
{  	 
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); //ʹ��GPIOBʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//ʹ��USART3ʱ��
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3); //PB10����ΪUSART3
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3); //PB11����ΪUSART3
	   
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;//PB10/11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;            //���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	    //�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;          //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;            //����
	GPIO_Init(GPIOB,&GPIO_InitStructure);                   //����������
	   
	USART_InitStructure.USART_BaudRate = bound;                  //����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;  //�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;       //һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;          //����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	               //�շ�ģʽ
    USART_Init(USART3, &USART_InitStructure);                                      //���������ô���3
	
    USART_Cmd(USART3, ENABLE);                              //ʹ�ܴ���3	
	USART_ClearFlag(USART3, USART_FLAG_TC);                 //�����־λ
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);          //���������ж�

    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;       //����3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3; //��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	                        //����ָ���Ĳ�����ʼ��VIC�Ĵ�����
	
	TIM3_Init(200,8400);	                                //��ʱ��ʱ��84M����Ƶϵ��8400������84M/8400=10Khz�ļ���Ƶ�ʣ�����200��Ϊ20ms 
    USART3_RX_STA=0;                                        //����3״̬��ʶ����  
}

/*-------------------------------------------------*/
/*������������3printf����                           */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/

__align(8) u8  USART3_TX_BUF[USART3_TXBUFF_SIZE];   //����2������

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

