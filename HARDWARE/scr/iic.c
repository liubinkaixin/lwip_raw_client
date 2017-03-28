/*-------------------------------------------------*/
/*         ��γ����STM32F407ZGT6������             */
/*    �Ա���ַ�� http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/
/*-------------------------------------------------*/
/*            IIC���߲���Դ�ļ�,��8������          */
/*-------------------------------------------------*/

#include "iic.h"
#include "delay.h"

/*-------------------------------------------------*/
/*����������ʼ��IIC�ӿ�                            */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void IIC_Init(void)
{			
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);   //ʹ��GPIOBʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;  //PB8/9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;           //��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;          //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;      //100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;            //����
	GPIO_Init(GPIOB, &GPIO_InitStructure);                  //��ʼ��
	IIC_SCL=1;   //SCL����
	IIC_SDA=1;   //SDA����
}
/*-------------------------------------------------*/
/*��������IIC��ʼ�ź�                              */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void IIC_Start(void)
{
	SDA_OUT();     //SDA�����ģʽ
	IIC_SDA=1;	   //SDA����  	  
	IIC_SCL=1;     //SCL����
	delay_us(4);   //��ʱ
 	IIC_SDA=0;     //SCL�ߵ�ƽ��ʱ��SDA�ɸߵ��ͣ�����һ����ʼ�ź�
	delay_us(4);   //��ʱ
	IIC_SCL=0;     //SCL����
}

/*-------------------------------------------------*/
/*��������IICֹͣ�ź�                              */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void IIC_Stop(void)
{
	SDA_OUT();     //SDA�����ģʽ
	IIC_SCL=0;     //SCL����
	IIC_SDA=0;     //SDA����//STOP:when CLK is high DATA change form low to high
 	delay_us(4);   //��ʱ
	IIC_SCL=1;     //SCL����
	IIC_SDA=1;     //SDA���� SCL�ߵ�ƽ��SDA�ɵ͵��ߣ�����ֹͣ�ź�
	delay_us(4);   //��ʱ 						   	
}

/*-------------------------------------------------*/
/*���������ȴ�Ӧ��                                 */
/*��  ������                                       */
/*����ֵ��0���ɹ�  1��ʧ��                         */
/*-------------------------------------------------*/
u8 IIC_Wait_Ack(void)
{
	u8 timeout=0;
	SDA_IN();                  //SDA�����ģʽ 
	IIC_SDA=1;delay_us(1);	   //SDA���� ��ʱ  
	IIC_SCL=1;delay_us(1);	   //SCL���� ��ʱ 
	
	while(READ_SDA)            //�ȴ�SDA��أ���ʾӦ���������Ȼһֱwhileѭ����ֱ����ʱ
	{
		timeout++;             //��ʱ����+1
		if(timeout>250)        //�������250
		{
			IIC_Stop();        //����ֹͣ�ź�
			return 1;          //����1����ʾʧ��
		}
	}
	IIC_SCL=0;                 //SCL����
	return 0;                  //����0����ʾ�ɹ�
} 

/*-------------------------------------------------*/
/*������������Ӧ��                                 */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void IIC_Ack(void)
{
	IIC_SCL=0;   //SCL����
	SDA_OUT();   //SDA�����ģʽ 
	IIC_SDA=0;   //SDA���ͣ���ʾӦ��
	delay_us(2); //��ʱ
	IIC_SCL=1;   //SCL����
	delay_us(2); //��ʱ
	IIC_SCL=0;   //SCL����
}

/*-------------------------------------------------*/
/*��������������Ӧ��                               */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/    
void IIC_NAck(void)
{
	IIC_SCL=0;   //SCL����
	SDA_OUT();   //SDA�����ģʽ 
	IIC_SDA=1;   //SDA���ߣ���ʾ��Ӧ��
	delay_us(2); //��ʱ
	IIC_SCL=1;   //SCL����
	delay_us(2); //��ʱ
	IIC_SCL=0;   //SCL����
}	

/*-------------------------------------------------*/
/*������������һ���ֽ�                             */
/*��  ����txd�����͵��ֽ�                          */
/*����ֵ����                                       */
/*-------------------------------------------------*/ 			  
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	SDA_OUT(); 	                  //SDA�����ģʽ 
    IIC_SCL=0;                    //SCL���ͣ���ʼ���ݴ���
    for(t=0;t<8;t++)              //forѭ����һλһλ�ķ��ͣ������λ λ7��ʼ
    {              
        IIC_SDA=(txd&0x80)>>7;    //����λ7�⣬����ȫ����Ϊ0��Ȼ�����Ƶ�λ0����SDA������
        txd<<=1; 	              //����һλ��׼����һ�η���
		delay_us(2);              //��ʱ
		IIC_SCL=1;                //SCL����
		delay_us(2);              //��ʱ
		IIC_SCL=0;	              //SCL����
		delay_us(2);              //��ʱ
    }	 
} 	    

/*-------------------------------------------------*/
/*����������ȡһ���ֽ�                             */
/*��  ����ack���Ƿ���Ӧ�� 1:�� 0������           */
/*����ֵ����ȡ������                               */
/*-------------------------------------------------*/   
u8 IIC_Read_Byte(unsigned char ack)
{
	u8 i,receive=0;
	
	SDA_IN();              //SDA����Ϊ����
    for(i=0;i<8;i++ )      //forѭ����һλһλ�Ķ�ȡ�������λ λ7��ʼ
	{
        IIC_SCL=0;        //SCL����    
        delay_us(2);      //��ʱ
		IIC_SCL=1;        //SCL���� 
        receive<<=1;      //����һλ��׼���´εĶ�ȡ
        if(READ_SDA)receive++;     //�����ȡ���Ǹߵ�ƽ��Ҳ����1��receive+1
		delay_us(1);      //��ʱ
    }					 
    if (!ack)          //����Ҫ����
        IIC_NAck();    //����nACK
    else               //��Ҫ����
        IIC_Ack();     //����ACK   
    return receive;
}


























