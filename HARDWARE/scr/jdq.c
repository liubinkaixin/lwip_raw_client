/*-------------------------------------------------*/
/*         ��γ����STM32F407ZGT6������             */
/*    �Ա���ַ�� http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/
/*-------------------------------------------------*/
/*            �̵���Դ�ļ�,��1������               */
/*-------------------------------------------------*/

/*-------------------------------------------------*/

#include "jdq.h" 
		    
/*-------------------------------------------------*/
/*����������ʼ���̵�������                            */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void JDQ_Init(void)
{    	 
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(JDQ_GROUP_EN, ENABLE);           //ʹ�ܿ���JDQ��IO��

	/*JDQ���ų�ʼ��*/
	GPIO_InitStructure.GPIO_Pin   = JDQ_PIN;            //����D8�����ű��
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;      //��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  //100MHz
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;       //����
	GPIO_Init(JDQ_GROUP, &GPIO_InitStructure);          //�����ó�ʼ��
	
	JDQ_IO=1; //�ߵ�ƽ����ʼ�� ���رյ����
}





