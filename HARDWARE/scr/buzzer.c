/*-------------------------------------------------*/
/*          ��γ����STM32F407ZGT6������            */
/*    �Ա���ַ�� http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/
/*-------------------------------------------------*/
/*            ������Դ�ļ�,��1������               */
/*-------------------------------------------------*/
/* void Buzzer_Init(void);  ��ʼ����������������   */
/*-------------------------------------------------*/

#include "buzzer.h" 
		    
/*-------------------------------------------------*/
/*����������ʼ���������������ź���                 */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void Buzzer_Init(void)
{    	 
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(BUZZER_GROUP_EN, ENABLE);     //ʹ�ܿ��Ʒ�������IO��

	/*���������ų�ʼ��*/
	GPIO_InitStructure.GPIO_Pin   = BUZZER_PIN;          //���Ʒ����������ű��
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;       //��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;       //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;   //100MHz
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;        //����
	GPIO_Init(BUZZER_GROUP, &GPIO_InitStructure);        //�����ó�ʼ��
	
	BUZZE=0;                                             //��ʼ���󣬹رշ�����
}






