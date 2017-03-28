/*-------------------------------------------------*/
/*          ��γ����STM32F407ZGT6������            */
/*    �Ա���ַ�� http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/
/*-----------------------------------------------------*/
/*           ��ʱ��4Դ�ļ�,��1������                   */
/*-----------------------------------------------------*/
/*void TIM4_Init(u16 arr,u16 psc); ��ʱ��4��ʼ��       */
/*-----------------------------------------------------*/

#include "timer4.h"

/*-------------------------------------------------*/
/*����������ʱ��4��ʼ��                            */
/*��  ����arr���Զ���װֵ                          */
/*        psc��ʱ��Ԥ��Ƶ��                        */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void TIM4_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
		
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);           //ʹ��TIM4ʱ��	
    TIM_TimeBaseInitStructure.TIM_Period = arr-1; 	              //�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc-1;                //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;     //1��Ƶ
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);            //����������TIM4
	
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);                      //����ʱ��4�����ж�
    TIM_ClearITPendingBit(TIM4,TIM_IT_Update);                    //����жϱ�־λ
	TIM_Cmd(TIM4,DISABLE); 
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM4_IRQn;                 //��ʱ��4�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01;    //��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x04;           //�����ȼ�4
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;                 //ʹ���ж�
	NVIC_Init(&NVIC_InitStructure);                               //���������� 
}


