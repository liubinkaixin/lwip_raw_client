/*-------------------------------------------------*/
/*         ��γ����STM32F407ZGT6������             */
/*    �Ա���ַ�� http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/
/*-------------------------------------------------*/
/*            PWMԴ�ļ�,�и�1����                  */
/*-------------------------------------------------*/
/*  void TIM3_PWM_Init(u32 arr,u32 psc)            */
/*-------------------------------------------------*/

#include "pwm.h"

/*-------------------------------------------------*/
/*��������TIM3ͨ��3��PWM��ʼ��                     */
/*��  ����arr���Զ���װֵ                          */
/*��  ����psc��ʱ��Ԥ��Ƶ��                        */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void TIM3_PWM_Init(u32 arr,u32 psc)
{		 					 	
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  	//TIM3ʱ��ʹ��    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 	//ʹ��PORTBʱ��	
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource0,GPIO_AF_TIM3);   //PB0����Ϊ��ʱ��3
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;           //PB0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //����
	GPIO_Init(GPIOB,&GPIO_InitStructure);               //����������
	  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;                  //��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period=arr;                     //�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;     //1��Ƶ 
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);            //����������
		 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;                  //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;      //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;           //�������:TIM����Ƚϼ��Ե�
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);                           //����������

	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);                 //ʹ��TIM3��ͨ��3�ϵ�Ԥװ�ؼĴ��� 
    TIM_ARRPreloadConfig(TIM3,ENABLE);                                //ARPEʹ�� 	
	TIM_Cmd(TIM3, ENABLE);                                            //ʹ��TIM4										  
}  


