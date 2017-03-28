/*-------------------------------------------------*/
/*         ��γ����STM32F407ZGT6������             */
/*    �Ա���ַ�� http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/
/*-------------------------------------------------*/
/*            ��ʱ��5Դ�ļ�,��1������              */
/*-------------------------------------------------*/
/* void TIM5_CH1_Cap_Init(u32 arr,u16 psc)         */
/*-------------------------------------------------*/

#include "timer5.h"

TIM_ICInitTypeDef  TIM5_ICInitStructure;

/*-------------------------------------------------*/
/*����������ʱ��5ͨ��1���벶������                 */
/*��  ����arr���Զ���װֵ                          */
/*        psc��ʱ��Ԥ��Ƶ��                        */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void TIM5_CH1_Cap_Init(u32 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);  	//TIM5ʱ��ʹ��    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	//ʹ��PORTAʱ��	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;               //GPIOA0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;            //���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	    //�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;          //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;          //����
	GPIO_Init(GPIOA,&GPIO_InitStructure);                   //��ʼ��PA0

	GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_TIM5);   //PA0����λ��ʱ��5
 	  
	TIM_TimeBaseStructure.TIM_Prescaler=psc-1;                //��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_Period=arr-1;                   //�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ	
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;     //1����	
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseStructure);            //����������
	
	//��ʼ��TIM5���벶�����
	TIM5_ICInitStructure.TIM_Channel = TIM_Channel_1;                //����� IC1ӳ�䵽ͨ��1��
	TIM5_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	 //�����ز���
	TIM5_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI1��
	TIM5_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	         //1��Ƶ 
	TIM5_ICInitStructure.TIM_ICFilter = 0x00;                        //���˲�
	TIM_ICInit(TIM5, &TIM5_ICInitStructure);                         //����������
		
	TIM_ITConfig(TIM5,TIM_IT_Update|TIM_IT_CC1,ENABLE);              //��������ж� ,����CC1IE�����ж�		
    TIM_Cmd(TIM5,ENABLE );                                        	 //ʹ�ܶ�ʱ��5

    NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;                  //��ʱ��5�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;          //��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		         //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			         //IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	                                 //����������	
}
