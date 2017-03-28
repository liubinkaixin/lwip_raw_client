/*-------------------------------------------------*/
/*         ��γ����STM32F407ZGT6������             */
/*    �Ա���ַ�� http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/
/*-------------------------------------------------*/
/*            LEDԴ�ļ�,��3������                  */
/*-------------------------------------------------*/
/*void LED_Init(void);     ��ʼ��	               */
/*void LED_AllOn(void);    ��������LED             */
/*void LED_AllOff(void);   Ϩ������LED             */
/*-------------------------------------------------*/

#include "led.h" 
		    
/*-------------------------------------------------*/
/*����������ʼ��LED����                            */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void LED_Init(void)
{    	 
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(LED_GROUP, ENABLE);           //ʹ�ܿ���LED��IO��

	/*LED8���ų�ʼ��*/
	GPIO_InitStructure.GPIO_Pin   = LED8_PIN;            //����D8�����ű��
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;       //��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;       //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;   //100MHz
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;        //����
	GPIO_Init(LED8_GROUP, &GPIO_InitStructure);          //�����ó�ʼ��
	
	/*LED9���ų�ʼ��*/
	GPIO_InitStructure.GPIO_Pin   = LED9_PIN;            //����D9�����ű��
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;       //��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;       //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;   //100MHz
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;        //����
	GPIO_Init(LED9_GROUP, &GPIO_InitStructure);          //�����ó�ʼ��
	
	/*LED10���ų�ʼ��*/
	GPIO_InitStructure.GPIO_Pin   = LED10_PIN;           //����D10�����ű��
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;       //��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;       //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;   //100MHz
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;        //����
	GPIO_Init(LED10_GROUP, &GPIO_InitStructure);         //�����ó�ʼ��
	
	LED_AllOff();       //��ʼ�����ź�����LED����Ϩ��״̬
}

/*-------------------------------------------------*/
/*����������������LED                              */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void LED_AllOn(void)
{
	GPIO_ResetBits(LED8_GROUP,LED8_PIN);   //����D8
	GPIO_ResetBits(LED9_GROUP,LED9_PIN);   //����D9
	GPIO_ResetBits(LED10_GROUP,LED10_PIN); //����D10
}

/*-------------------------------------------------*/
/*��������Ϩ������LED                              */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void LED_AllOff(void)
{
	GPIO_SetBits(LED8_GROUP,LED8_PIN);     //Ϩ��D8
	GPIO_SetBits(LED9_GROUP,LED9_PIN);     //Ϩ��D8
	GPIO_SetBits(LED10_GROUP,LED10_PIN);   //Ϩ��D8
}





