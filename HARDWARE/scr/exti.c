/*-------------------------------------------------*/
/*          ��γ����STM32F407ZGT6������            */
/*    �Ա���ַ�� http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/
/*-------------------------------------------------*/
/*          �ⲿ�ж�Դ�ļ�,��1������               */
/*-------------------------------------------------*/
/* void KEY_EXTI_Init(void);	�ⲿ�жϳ�ʼ��     */     
/*-------------------------------------------------*/

#include "exti.h"
#include "key.h"

/*-------------------------------------------------*/
/*�������������ⲿ�жϳ�ʼ������.                  */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/	   
void KEY_EXTI_Init(void)
{
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;

	KEY_Init();       //��ʼ������

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);     //ʹ��SYSCFGʱ��

	SYSCFG_EXTILineConfig(KEY1_EXTI_GPOUP, KEY1_EXTI_SOURCE);  //����KEY1���ڵ��ж�Դ
	SYSCFG_EXTILineConfig(KEY2_EXTI_GPOUP, KEY2_EXTI_SOURCE);  //����KEY2���ڵ��ж�Դ
	SYSCFG_EXTILineConfig(KEY3_EXTI_GPOUP, KEY3_EXTI_SOURCE);  //����KEY3���ڵ��ж�Դ
	SYSCFG_EXTILineConfig(KEY4_EXTI_GPOUP, KEY4_EXTI_SOURCE);  //����KEY4���ڵ��ж�Դ

	EXTI_InitStructure.EXTI_Line    = KEY1_EXTI_LINE;           //KEY1�����ж���
	EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;      //�ж��¼�
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;      //�Ͻ��ش���
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;                   //�ж���ʹ��
	EXTI_Init(&EXTI_InitStructure);                             //�����ж�	
	
	EXTI_InitStructure.EXTI_Line    = KEY2_EXTI_LINE | KEY3_EXTI_LINE  |KEY4_EXTI_LINE;  //KEY2.3.4�����ж���
	EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;      //�ж��¼�
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;     //�½��ش���
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;                   //�ж���ʹ��
	EXTI_Init(&EXTI_InitStructure);                             //�����ж�

	NVIC_InitStructure.NVIC_IRQChannel = KEY1_EXTI_IRQ ;        //����1�����ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;//��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;       //�����ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);                             //���� 

	NVIC_InitStructure.NVIC_IRQChannel = KEY2_EXTI_IRQ ;        //����1�����ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;//��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;       //�����ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);                             //����
	
	NVIC_InitStructure.NVIC_IRQChannel = KEY3_EXTI_IRQ ;        //����1�����ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;//��ռ���ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;       //�����ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);                             //����
	
	NVIC_InitStructure.NVIC_IRQChannel = KEY4_EXTI_IRQ ;        //����1�����ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;       //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);                             //����
}












