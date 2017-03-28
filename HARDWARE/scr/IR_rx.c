/*-------------------------------------------------*/
/*         ��γ����STM32F407ZGT6������             */
/*    �Ա���ַ�� http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/
/*-------------------------------------------------*/
/*            �������Դ�ļ�,��2������             */
/*-------------------------------------------------*/
/* void IR_Init(void);  �����ʼ��                 */
/* u8 IR_recive(void);  �������                   */
/*-------------------------------------------------*/

#include "ir_rx.h"
#include "delay.h"

u8 	RmtSta=0;	//״̬ 	  
u16 Dval;		//�½���ʱ��������ֵ
u32 RmtRec=0;	//������յ�������	   		    
u8  RmtCnt=0;	//�������µĴ���	

/*-------------------------------------------------*/
/*������������ң�س�ʼ����ʱ��                     */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void IR_Init(void)    			  
{		
	GPIO_InitTypeDef  GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef  TIM1_ICInitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//ʹ��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);//TIM1ʱ��ʹ�� 

	//GPIOA8  ���ù���,����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��

	GPIO_PinAFConfig(GPIOA,GPIO_PinSource8,GPIO_AF_TIM1); //GPIOA8����ΪTIM1

	TIM_TimeBaseStructure.TIM_Prescaler=167;  ////Ԥ��Ƶ��,1M�ļ���Ƶ��,1us��1.	
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period=10000;   //�趨�������Զ���װֵ ���10ms���  
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure); 
	  
	//��ʼ��TIM2���벶�����
	TIM1_ICInitStructure.TIM_Channel = TIM_Channel_1; //CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
	TIM1_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//�����ز���
	TIM1_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI1��
	TIM1_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //���������Ƶ,����Ƶ 
	TIM1_ICInitStructure.TIM_ICFilter = 0x03;//IC1F=0003 8����ʱ��ʱ�������˲�
	TIM_ICInit(TIM1, &TIM1_ICInitStructure);//��ʼ����ʱ��2���벶��ͨ��

	TIM_ITConfig(TIM1,TIM_IT_Update|TIM_IT_CC1,ENABLE);//��������ж� ,����CC1IE�����ж�	
	TIM_Cmd(TIM1,ENABLE ); 	 	                       //ʹ�ܶ�ʱ��1

	NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//��ʼ��NVIC�Ĵ���

	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_TIM10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;		//�����ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//��ʼ��NVIC�Ĵ���
}

/*-------------------------------------------------*/
/*������������ң���źŲɼ�                         */
/*��  ������                                       */
/*����ֵ��0:û��  ��������ֵ                       */
/*-------------------------------------------------*/ 
u8 IR_recive(void)
{        
	u8 sta=0;       
    u8 t1,t2;  
	if(RmtSta&(1<<6))           //�õ�һ��������������Ϣ��
	{ 
	    t1=RmtRec>>24;			//�õ���ַ��
	    t2=(RmtRec>>16)&0xff;	//�õ���ַ���� 
 	    if((t1==(u8)~t2))       //����ң��ʶ����(ID)����ַ 
	    { 
	        t1=RmtRec>>8;
	        t2=RmtRec; 	
	        if(t1==(u8)~t2)sta=t1;//��ֵ��ȷ	 
		}   
		if((sta==0)||((RmtSta&0X80)==0))//�������ݴ���/ң���Ѿ�û�а�����
		{
		 	RmtSta&=~(1<<6);//������յ���Ч������ʶ
			RmtCnt=0;		//�����������������
		}
	}  
    return sta;
}
