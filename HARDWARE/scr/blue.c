/*-------------------------------------------------*/
/*         ��γ����STM32F407ZGT6������             */
/*    �Ա���ַ�� http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/
/*-------------------------------------------------*/
/*            ����Դ�ļ�,�и�����                  */
/*-------------------------------------------------*/

#include "blue.h" 
#include "usart3.h" 
#include "timer3.h" 
#include "delay.h" 

/*-------------------------------------------------*/
/*����������ʼ������                               */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void Blue_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);   //ʹ��PBʱ��

	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_15;        
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;      //��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  //100MHz
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;     //����
	GPIO_Init(GPIOB, &GPIO_InitStructure);              //�����ó�ʼ��                                      //����AT

	Usart3_init(38400);	    //���ڳ�ʼ��������Ϊ38400 
	TIM3_Init(200,8400);    //��ʱ��3��ʼ��,8400���䣬200������������ʱ��20ms	
}

/*-------------------------------------------------*/
/*����������������ATģʽ                           */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void Blue_InAT(void)
{
	BLUE_KEY=1;  //����AT
}

/*-------------------------------------------------*/
/*�������������˳�ATģʽ                           */
/*��  ������                                       */
/*����ֵ��0����ȷ  ����������                      */
/*-------------------------------------------------*/
u8 Blue_QuitAT(void)
{
	BLUE_KEY=0;                                 //����KEY
	delay_ms(100);
	if(Blue_SendCmd_OK("AT+RESET",20))return 1; //���͸�λָ�2s��ʱʱ�䣬���󷵻�1
	return 0;                                   //��ȷ����0
}

/*-------------------------------------------------*/
/*������������ATָ��жϻظ�OK                   */
/*��  ����cmd��ָ��                                */
/*��  ����timeout���ȴ��ظ��ĳ�ʱʱ��              */
/*����ֵ��0����ȷ  ����������                      */
/*-------------------------------------------------*/
u8 Blue_SendCmd_OK(u8 *cmd, u16 timeout)
{
    u16 time=0;
	
	USART3_RX_STA=0;                        
	memset(USART3_RX_BUF,0,USART3_RXBUFF_SIZE);//�������3����������	
	u3_printf("%s\r\n",cmd);                   //����ָ��
	time=0;
	while(time<timeout)
	{
		if(strstr( (const char *)USART3_RX_BUF , (const char *)"OK"))//������ܵ�OK��ʾָ��ɹ�
			break;
		delay_ms(100);//��ʱ100ms
		time++;      
	}
	if(time>=timeout)return 1;                //��ʱ���󣬷���1
	else return 0;		                      //�޴�����0
}

/*-------------------------------------------------*/
/*����������������ģ��                             */
/*��  ������                                       */
/*����ֵ��0����ȷ  ����������                      */
/*-------------------------------------------------*/
u8 Blue_Setup(void)
{                                     
	Blue_InAT();                                        //����ATģʽ
	if(Blue_SendCmd_OK("AT+NAME=CHAOWEI",20))return 1;  //�����������֣�2s��ʱʱ��,����Ļ�����1.
	if(Blue_SendCmd_OK("AT+ROLE=0",20))return 2;        //��������Ϊ�ӻ���2s��ʱʱ��,����Ļ�����2
	if(Blue_SendCmd_OK("AT+CMODE=1",20))return 3;       //����������������ģʽ��2s��ʱʱ��,����Ļ�����3
	return 0;                                           //��ȷ����0
}


		    
