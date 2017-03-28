/*-------------------------------------------------*/
/*          ��γ����STM32F407ZGT6������            */
/*    �Ա���ַ�� http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/
/*-----------------------------------------------------*/
/*           ��ʱ��10Դ�ļ�,��1������                   */
/*-----------------------------------------------------*/
/*void TIM10_Init(u16 arr,u16 psc); ��ʱ��10��ʼ��       */
/*-----------------------------------------------------*/

#include "timer10.h"

/*-------------------------------------------------*/
/*����������ʱ��10��ʼ��                            */
/*��  ����arr���Զ���װֵ                          */
/*        psc��ʱ��Ԥ��Ƶ��                        */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void TIM10_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
		
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10,ENABLE);           //ʹ��TIM10ʱ��	
    TIM_TimeBaseInitStructure.TIM_Period = arr-1; 	              //�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc-1;                //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;     //1��Ƶ
	TIM_TimeBaseInit(TIM10,&TIM_TimeBaseInitStructure);            //����������TIM10
	
	TIM_ITConfig(TIM10,TIM_IT_Update,ENABLE);                      //����ʱ��10�����ж�
    TIM_ClearITPendingBit(TIM10,TIM_IT_Update);                    //����жϱ�־λ
	TIM_Cmd(TIM10,DISABLE);                                        //��ʹ�ܶ�ʱ��10
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM1_UP_TIM10_IRQn;        //��ʱ��10�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;       //��ռ���ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;              //�����ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;                 //ʹ���ж�
	NVIC_Init(&NVIC_InitStructure);                               //���������� 
}


