/*-------------------------------------------------*/
/*         ��γ����STM32F407ZGT6������             */
/*    �Ա���ַ�� http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/
/*-------------------------------------------------*/
/*            ����2Դ�ļ�,��2������                */
/*-------------------------------------------------*/
/*  void Usart2_init(u32 bound);                   */
/*  void u2_printf(char* fmt,...);                 */
/*-------------------------------------------------*/

#include "main.h"
#include "usart2.h"	

u8  USART2_RX_BUF[USART2_RXBUFF_SIZE];     //���ջ�������
u16 USART2_RX_STA=0;                       //����״̬���	

/*-------------------------------------------------*/
/*����������ʼ������2                              */
/*��  ����bound��������                            */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void Usart2_init(u32 bound)
{  	 	
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //ʹ��GPIOAʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//ʹ��USART2ʱ��
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2); //PA2����ΪUSART2
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2); //PA3����ΪUSART2
 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //����
	GPIO_Init(GPIOA,&GPIO_InitStructure);               //����������
	
	USART_InitStructure.USART_BaudRate = bound;                                    //����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;                    //�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;                         //һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;                            //����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	               //�շ�ģʽ
    USART_Init(USART2, &USART_InitStructure);                                      //���������ô���2
	
    USART_Cmd(USART2, ENABLE);                              //ʹ�ܴ��� 2
	USART_ClearFlag(USART2, USART_FLAG_TC);	                //�����־λ
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);          //���������ж�

    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;       //����2�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3; //��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//�ж�ͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	                        //������������

}

 __align(8) u8  USART2_TX_BUF[USART2_TXBUFF_SIZE];          //����2������

/*-------------------------------------------------*/
/*������������2printf                              */
/*��  ������                                       */
/*����ֵ����                                       */
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
