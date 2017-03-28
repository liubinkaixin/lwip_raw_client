/*-------------------------------------------------*/
/*         ��������STM32F407ZGT6������             */
/*    �Ա���ַ�� https://shop68304486.taobao.com   */
/*-------------------------------------------------*/
/*-------------------------------------------------*/
/*            ����1Դ�ļ�,��1������                */
/*-------------------------------------------------*/
/*  void   Usart1_init(u32 bound);                 */
/*-------------------------------------------------*/

#include "main.h"
#include "usart1.h"	
 
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 0
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
_sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
	USART1->DR = (u8) ch;      
	return ch;
}
#endif

u8  USART1_RX_BUF[USART1_RXBUFF_SIZE];     //���ջ�������
u16 USART1_RX_STA=0;                       //����״̬���	

/*-------------------------------------------------*/
/*����������ʼ������1                              */
/*��  ����bound��������                            */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void Usart1_init(u32 bound)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);     //ʹ��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);    //ʹ��USART1ʱ��
 
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);  //GPIOA9����ΪUSART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10����ΪUSART1
	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;  //GPIOA9��GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;             //���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	     //�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;           //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;             //����
	GPIO_Init(GPIOA,&GPIO_InitStructure);                    //��ʼ��PA9��PA10

	USART_InitStructure.USART_BaudRate = bound;                //����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;     //һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;        //����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	               //���տɷ�
    USART_Init(USART1, &USART_InitStructure);                                      //��ʼ������1
	
    USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ���1 	
	USART_ClearFlag(USART1, USART_FLAG_TC);       //�������1��־λ
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//���������ж�
 
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;      //����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;	   //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		   //IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	                       //����ָ���Ĳ�����ʼ��VIC�Ĵ�����	
}

 


