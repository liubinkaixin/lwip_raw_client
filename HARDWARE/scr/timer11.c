/*-------------------------------------------------*/
/*          ��γ����STM32F407ZGT6������            */
/*    �Ա���ַ�� http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/
/*-----------------------------------------------------*/
/*           ��ʱ��11Դ�ļ�,��1������                   */
/*-----------------------------------------------------*/
/*void TIM11_Init(u16 arr,u16 psc); ��ʱ��11��ʼ��       */
/*-----------------------------------------------------*/

#include "timer11.h"

/*-------------------------------------------------*/
/*����������ʱ��11��ʼ��                            */
/*��  ����arr���Զ���װֵ                          */
/*        psc��ʱ��Ԥ��Ƶ��                        */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void TIM11_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
		
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM11,ENABLE);           //ʹ��TIM11ʱ��	
    TIM_TimeBaseInitStructure.TIM_Period = arr-1; 	              //�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc-1;                //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;     //1��Ƶ
	TIM_TimeBaseInit(TIM11,&TIM_TimeBaseInitStructure);            //����������TIM11
	
	TIM_ITConfig(TIM11,TIM_IT_Update,ENABLE);                      //����ʱ��11�����ж�
    TIM_ClearITPendingBit(TIM11,TIM_IT_Update);                    //����жϱ�־λ
	TIM_Cmd(TIM11,DISABLE);                                        //��ʹ�ܶ�ʱ��11
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM1_TRG_COM_TIM11_IRQn;    //��ʱ��11�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;        //��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=2;               //�����ȼ�11
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;                  //ʹ���ж�
	NVIC_Init(&NVIC_InitStructure);                                //���������� 
}


