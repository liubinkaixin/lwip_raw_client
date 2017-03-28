/*-------------------------------------------------*/
/*          ��γ����STM32F407ZGT6������            */
/*    �Ա���ַ�� http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/
/*-----------------------------------------------------*/
/*           ��ʱ��2Դ�ļ�,��1������                   */
/*-----------------------------------------------------*/
/*void TIM2_Init(u16 arr,u16 psc); ��ʱ��2��ʼ��       */
/*-----------------------------------------------------*/

#include "timer2.h"

/*-------------------------------------------------*/
/*����������ʱ��2��ʼ��                            */
/*��  ����arr���Զ���װֵ                          */
/*        psc��ʱ��Ԥ��Ƶ��                        */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void TIM2_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);           //ʹ��TIM2ʱ��	
    TIM_TimeBaseInitStructure.TIM_Period = arr-1; 	              //�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc-1;                //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;     //1��Ƶ
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);            //����������TIM2	
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);                      //����ʱ��4�����ж�
    TIM_ClearITPendingBit(TIM2,TIM_IT_Update);                    //����жϱ�־λ
	TIM_Cmd(TIM2,DISABLE); 
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn;                 //��ʱ��2�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;       //��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;              //�����ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;                 //ʹ���ж�
	NVIC_Init(&NVIC_InitStructure);                               //���������� 
	                                      //��ʹ�ܶ�ʱ��2   
}


