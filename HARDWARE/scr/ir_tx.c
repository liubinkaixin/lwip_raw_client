/*-------------------------------------------------*/
/*         ��γ����STM32F407ZGT6������             */
/*    �Ա���ַ�� http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/
/*-------------------------------------------------*/
/*            ���ⷢ��Դ�ļ�,�и�����             */
/*-------------------------------------------------*/

/*-------------------------------------------------*/

#include "ir_tx.h"
#include "delay.h"

/*-------------------------------------------------*/
/*�����������ⷢ�ͳ�ʼ��                           */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void IR_SentInit(u32 arr,u32 psc)
{		 					 
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  	//TIM3ʱ��ʹ��    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); 	//ʹ��PORTCʱ��	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);  //ʹ��SYSCFGʱ��
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_7;          //PC7
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;       //��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;       //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;   //100MHz
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;        //����
	GPIO_Init(GPIOC, &GPIO_InitStructure);               //�����ó�ʼ��
	
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_TIM3);   //PC6����Ϊ��ʱ��3	
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6;          //PC6
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //����
	GPIO_Init(GPIOC,&GPIO_InitStructure);               //����������
	  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;                  //��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period=arr;                     //�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;     //1����
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);            //����������
		 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;             
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     
	
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);           
    TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable); 

    TIM_ARRPreloadConfig(TIM3,ENABLE);                 	
	TIM_Cmd(TIM3, ENABLE);  	

	TIM_SetCompare1(TIM3,9);  //�޸ıȽ�ֵ���޸�ռ�ձ�
} 

/*-------------------------------------------------*/
/*������������һ��������λ0                        */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void Sent_0(void)     //560us�͵�ƽ+560us�͵�ƽ  ����һ��0
{
	IR_SEND=0;
	delay_us(560);    //560us�͵�ƽ
	IR_SEND=1; 
	delay_us(560);	  //560us�͵�ƽ
}

/*-------------------------------------------------*/
/*������������һ��������λ1                        */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void Sent_1(void)    //560us�͵�ƽ+1680us�͵�ƽ  ����һ��1
{
	IR_SEND=0;
	delay_us(560);  //560us�͵�ƽ
	IR_SEND=1;
	delay_us(1680);	//1680us�͵�ƽ
}

/*-------------------------------------------------*/
/*�����������ͺ����ź�                             */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void IR_Send(u8 data)   //data�Ǽ�ֵ
{
	u8 i,temp;
	u8 t_data;
	
	t_data=~data;  //t_data��data�ķ���
	
	IR_SEND=0;     
	delay_us(9000);//9ms�ߵ�ƽ
	IR_SEND=1;
	delay_us(4500);//4.5ms�͵�ƽ  ͬ����

	for(i=0;i<8;i++)   //�û���1�������Զ���0x00
	{
		Sent_0();		
	}
	for(i=0;i<8;i++)   //�û���2�������Զ���0xFF
	{
		Sent_1();	
	}
	for(i=0;i<8;i++)   //��ֵ��һλһλ�ķ�
	{
		temp=(data&0x80);
		if(temp)Sent_1();	
		else Sent_0();
		data<<=1;
	}
	for(i=0;i<8;i++)   //����ֵ��һλһλ�ķ�
	{
		temp=(t_data&0x80);
		if(temp)Sent_1();	
		else Sent_0();
		t_data<<=1;
	}

	Sent_0();      //ֹͣ��
}






























