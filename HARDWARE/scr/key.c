/*-------------------------------------------------*/
/*          ��γ����STM32F407ZGT6������            */
/*    �Ա���ַ�� http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/
/*-------------------------------------------------*/
/*            ����Դ�ļ�,��2������                 */
/*-------------------------------------------------*/
/* void KEY_Init(void);	   ������ʼ��              */     
/* u8 KEY_Scan(u8 mode);   ����������	           */
/*-------------------------------------------------*/

#include "key.h"
#include "delay.h" 

/*-------------------------------------------------*/
/*����������ʼ��KEY��������                        */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void KEY_Init(void)
{	
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(KEY_GROUP_EN, ENABLE);         //ʹ�ܿ��ư�����IO��

	/*��ʼ��K1������*/
	GPIO_InitStructure.GPIO_Pin = KEY1_PIN;                //����K1��IO���
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;           //��ͨ����ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;     //100M
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;         //����
	GPIO_Init(KEY1_GROUP, &GPIO_InitStructure);            //�����ó�ʼ��.
	
	/*��ʼ��K2������*/
	GPIO_InitStructure.GPIO_Pin = KEY2_PIN;                //����K2��IO���
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;           //��ͨ����ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;     //100M
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;           //����
	GPIO_Init(KEY2_GROUP, &GPIO_InitStructure);            //�����ó�ʼ��
	
	/*��ʼ��K3������*/
	GPIO_InitStructure.GPIO_Pin = KEY3_PIN;                //����K3��IO���
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;           //��ͨ����ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;     //100M
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;           //����
	GPIO_Init(KEY3_GROUP, &GPIO_InitStructure);            //�����ó�ʼ��
	
	/*��ʼ��K4������*/
	GPIO_InitStructure.GPIO_Pin = KEY4_PIN;                //����K4��IO���
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;           //��ͨ����ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;     //100M
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;           //����
	GPIO_Init(KEY4_GROUP, &GPIO_InitStructure);            //�����ó�ʼ��	
} 

/*-------------------------------------------------*/
/*������������������                             */
/*��  ����mode��0-���º󴥷�   1-�ͷź󴥷�        */
/*����ֵ������ֵ                                   */
/*-------------------------------------------------*/
u8 KEY_Scan(u8 mode)
{	 	  
	u8 key_num;                //���水��ֵ

	key_num=NONE_PRES_NUM;     //������Ϊ�ް���ֵ(0)
	if(KEY1==1||KEY2==0||KEY3==0||KEY4==0)
	{
		delay_ms(5);                              //ȥ���� 
		if(KEY1==1)        key_num = KEY1_PRES_NUM;//����K1��ֵ
		else if(KEY2==0)   key_num = KEY2_PRES_NUM;//����K2��ֵ
		else if(KEY3==0)   key_num = KEY3_PRES_NUM;//����K3��ֵ
		else if(KEY4==0)   key_num = KEY4_PRES_NUM;//����K4��ֵ
	} 
	if(mode)                                       //modeλ1��ʱ���ͷŰ�����Ŵ���
		while(KEY1==1||KEY2==0||KEY3==0||KEY4==0); //���ּ��
	
	return key_num;                                //���ؼ�ֵ
}




















