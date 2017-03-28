/*-------------------------------------------------*/
/*         ��������STM32F407ZGT6������             */
/*    �Ա���ַ�� https://shop68304486.taobao.com   */
/*-------------------------------------------------*/
/*-------------------------------------------------*/
/*            ��ʱԴ�ļ�,��3������                 */
/*-------------------------------------------------*/
/*void delay_init(void);        ��ʱ��ʼ������     */
/*void delay_ms(u32 ms);        ��ʱ���뺯��       */
/*void delay_us(u32 us);        ��ʱ΢�뺯��       */
/*-------------------------------------------------*/

#include "delay.h"

/*-------------------------------------------------*/
/*����������ʼ���ӳٺ���                           */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void delay_init(void)
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	//SysTick���δ�ʱ�ӣ���Ƶ������Ƶ�ʵ�1/8
	//ͨ����Ƶ��Ϊ����168M����ôSysTick��Ƶ����21M
	//��ôSysTick������1����������(1/21)us
}

/*-------------------------------------------------*/
/*���������ӳ�΢�뺯��                             */
/*��  ����us����ʱ����΢��                         */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void delay_us(u32 us)
{		
	u32 temp;	    	 
	SysTick->LOAD=us*21;                      //SysTick����ֵ��delay_init����������֪���� SysTick������һ������1/21΢�룬���Գ���21
	SysTick->VAL=0x00;                        //��ռ�����
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;  //��ʼ���� 
	do
	{
		temp=SysTick->CTRL;                        //��ȡSysTick״̬�Ĵ���
	}
	while((temp&0x01)&&!(temp&(1<<16)));           //�ȴ�ʱ�䵽���־λ��λ   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;       //�رռ�����
	SysTick->VAL =0X00;                            //��ռ�����	 
}

/*-------------------------------------------------*/
/*���������ӳٺ��뺯��                             */
/*��  ����ms����ʱ���ٺ���                         */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void delay_ms(u32 ms)
{
	//��������ע��һ������SysTickʱ�Ӽ�������24λ�ģ�21MƵ���£��ܹ�����ʱ798.915ms
	//����������700Ϊ���ޣ�С��700����ʱһ�μ������У�����700�Ķ�μ���	
	u32 i;
	u32 temp;
	
	if(ms<700)   //С��700ms
	{
		SysTick->LOAD=(u32)ms*21*1000;           //ʱ�����,SysTickһ����ʹ1/21΢�룬�����ms�Ļ�������21�ٳ���1000
		SysTick->VAL =0x00;                       //��ռ�����
		SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;  //��ʼ����  
		do
		{
			temp=SysTick->CTRL;                        //��ȡSysTick״̬�Ĵ���
		}
		while((temp&0x01)&&!(temp&(1<<16)));           //�ȴ�ʱ�䵽���־λ��λ   
		SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;       //�رռ�����
		SysTick->VAL =0X00;                            //��ռ�����	 
	}
	else   //����700ms
	{
		for(i=0;i<(ms/700);i++)    //����700�������֣�forѭ������ʱ
		{
			SysTick->LOAD=700*21*1000;                //ʱ�����,700ms��ʱ
			SysTick->VAL =0x00;                       //��ռ�����
			SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;  //��ʼ����  
			do
			{
				temp=SysTick->CTRL;                        //��ȡSysTick״̬�Ĵ���
			}
			while((temp&0x01)&&!(temp&(1<<16)));           //�ȴ�ʱ�䵽���־λ��λ   
			SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;       //�رռ�����
			SysTick->VAL =0X00;                            //��ռ�����	 
		}
		//�������������ֵ���ʱ
		SysTick->LOAD=(u32)(ms%700)*21*1000;           //ʱ�����,��������
		SysTick->VAL =0x00;                       //��ռ�����
		SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;  //��ʼ����  
		do
		{
		temp=SysTick->CTRL;                       //��ȡSysTick״̬�Ĵ���
		}
		while((temp&0x01)&&!(temp&(1<<16)));           //�ȴ�ʱ�䵽���־λ��λ   
		SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;       //�رռ�����
		SysTick->VAL =0X00;                            //��ռ�����	
	}
} 

/*-------------------------------------------------*/
/*���������ӳ����뺯��-�Ǿ�ȷ������ʱ��            */
/*��  ����ns����ʱ��������                         */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void delay_ns(u32 ns)
{
  u32 i;
  for(i=0;i<ns;i++)
  {
    __nop();
    __nop();
    __nop();
  }
}	

































