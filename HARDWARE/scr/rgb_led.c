/*-------------------------------------------------*/
/*         ��γ����STM32F407ZGT6������             */
/*    �Ա���ַ�� http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/
/*-------------------------------------------------*/
/*            ���LEDԴ�ļ�,��2������              */
/*-------------------------------------------------*/
/* void TIM4_PWM_Init(u32 arr,u32 psc);            */  
/* void TIM3_PWM_Init(u32 arr,u32 psc);            */
/*-------------------------------------------------*/

#include "rgb_led.h"

/*-------------------------------------------------*/
/*����������ʱ��4PWM��ʼ��                         */
/*��  ����arr���Զ���װֵ                          */
/*��  ����psc��ʱ��Ԥ��Ƶ��                        */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void TIM4_PWM_Init(u32 arr,u32 psc)
{		 					 
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  	//TIM4ʱ��ʹ��    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 	//ʹ��PORTBʱ��		
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource6,GPIO_AF_TIM4);   //PB6����Ϊ��ʱ��4pwm

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;           //PB6          
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //����
	GPIO_Init(GPIOB,&GPIO_InitStructure);               //����������
	  
	TIM_TimeBaseStructure.TIM_Prescaler=psc-1;                //��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period=arr-1;                   //�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 	  //1����
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);            //����������
	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;             //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;      //�������:TIM����Ƚϼ��Ե�
	
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);           //�������ö�ʱ��4��ͨ��1
    TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);  //ʹ��TIM4��ͨ��1�ϵ�Ԥװ�ؼĴ���
 
    TIM_ARRPreloadConfig(TIM4,ENABLE);//ARPEʹ�� 	
	TIM_Cmd(TIM4, ENABLE);            //ʹ��TIM4										  
}  

/*-------------------------------------------------*/
/*����������ʱ��3PWM��ʼ��                         */
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
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource4,GPIO_AF_TIM4);   //PB4����Ϊ��ʱ��3PWM
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource5,GPIO_AF_TIM4);   //PB5����Ϊ��ʱ��3PWM
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;    //PB4/5         
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;            //���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	    //�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;          //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;            //����
	GPIO_Init(GPIOB,&GPIO_InitStructure);                   //����������
	  
	TIM_TimeBaseStructure.TIM_Prescaler=psc-1;                //��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period=arr-1;                   //�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;     //1��Ƶ 	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);            //����������
		 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;             //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;      //�������:TIM����Ƚϼ��Ե�
	
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);           //�������ö�ʱ��3��ͨ��1
    TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIM3��ͨ��1�ϵ�Ԥװ�ؼĴ���
	
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);           //�������ö�ʱ��3��ͨ��2
    TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIM3��ͨ��2�ϵ�Ԥװ�ؼĴ���
 
    TIM_ARRPreloadConfig(TIM3,ENABLE);//ARPEʹ�� 	
	TIM_Cmd(TIM3, ENABLE);            //ʹ��TIM3										  
} 

/*-------------------------------------------------*/
/*��������3ɫ��IO�ڳ�ʼ��                          */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void RGB_LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  	//TIM3ʱ��ʹ��    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 	//ʹ��PORTBʱ��	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;    //PB4.5.6      
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;           //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	    //�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;          //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;            //����
	GPIO_Init(GPIOB,&GPIO_InitStructure);                   //����������	

	LED_Bout=1;         
	LED_Rout=1;        
	LED_Gout=1;  	
}
/*-------------------------------------------------*/
/*��������������ɫPWM                              */
/*��  ����num���޸�ռ�ձȣ��ı�����                */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void BLUE_LED(u8 num)
{
	TIM_SetCompare1(TIM4,num);	//�޸ıȽ�ֵ���޸�ռ�ձ�
}

/*-------------------------------------------------*/
/*�����������ú�ɫPWM                              */
/*��  ����num���޸�ռ�ձȣ��ı�����                */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void RED_LED(u8 num)
{
	TIM_SetCompare2(TIM3,num);	//�޸ıȽ�ֵ���޸�ռ�ձ�
}

/*-------------------------------------------------*/
/*��������������ɫPWM                              */
/*��  ����num���޸�ռ�ձȣ��ı�����                */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void GREEN_LED(u8 num)
{
	TIM_SetCompare1(TIM3,num);	//�޸ıȽ�ֵ���޸�ռ�ձ�
}


