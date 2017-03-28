/*-------------------------------------------------*/
/*          ��γ����STM32F407ZGT6������            */
/*    �Ա���ַ�� http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/
/*-----------------------------------------------------*/
/*           ��ʱ��9Դ�ļ�,��1������                   */
/*-----------------------------------------------------*/
/*void TIM9_Init(u16 arr,u16 psc); ��ʱ��9��ʼ��       */
/*-----------------------------------------------------*/

#include "timer9.h"

/*-------------------------------------------------*/
/*����������ʱ��9��ʼ��                            */
/*��  ����arr���Զ���װֵ                          */
/*        psc��ʱ��Ԥ��Ƶ��                        */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void TIM9_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
		
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9,ENABLE);           //ʹ��TIM9ʱ��	
    TIM_TimeBaseInitStructure.TIM_Period = arr-1; 	              //�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc-1;                //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;     //1��Ƶ
	TIM_TimeBaseInit(TIM9,&TIM_TimeBaseInitStructure);            //����������TIM9
	
	TIM_ITConfig(TIM9,TIM_IT_Update,ENABLE);                      //����ʱ��9�����ж�
    TIM_ClearITPendingBit(TIM9,TIM_IT_Update);                    //����жϱ�־λ
	TIM_Cmd(TIM9,DISABLE);                                        //��ʹ�ܶ�ʱ��9
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM1_BRK_TIM9_IRQn;                 //��ʱ��9�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;       //��ռ���ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;              //�����ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;                 //ʹ���ж�
	NVIC_Init(&NVIC_InitStructure);                               //���������� 
}


