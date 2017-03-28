/*-------------------------------------------------*/
/*         ��γ����STM32F407ZGT6������             */
/*    �Ա���ַ�� http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/
/*-------------------------------------------------*/
/*            DHT11Դ�ļ�,��6������                 */
/*-------------------------------------------------*/
/* u8 DHT11_Init(void);                  ��ʼ��    */
/* u8 DHT11_Read_Data(u8 *temp,u8 *humi);����ʪ��  */
/* u8 DHT11_Read_Byte(void);             ��һ���ֽ�*/
/* u8 DHT11_Read_Bit(void);              ��һ��λ  */
/* u8 DHT11_Check(void);                 ���DHT11 */
/* void DHT11_Rst(void);                 ��λDHT11 */  
/*-------------------------------------------------*/

#include "dht11.h"
#include "delay.h"
  
/*-------------------------------------------------*/
/*����������λDHT11                                */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/  
void DHT11_Rst(void)	   
{                 
  DHT11_IO_OUT(); 	//SET OUTPUT
  DHT11_DQ_OUT=0; 	//����DQ
  delay_ms(20);    	//��������18ms
  DHT11_DQ_OUT=1; 	//DQ=1 
  delay_us(30);     //��������20~40us
}

/*-------------------------------------------------*/
/*���������ȴ�DHT11�Ļ�Ӧ                          */
/*��  ������                                       */
/*����ֵ��1���� 0��ȷ                              */
/*-------------------------------------------------*/ 
u8 DHT11_Check(void) 	   
{   
	u8 retry=0;
	DHT11_IO_IN();
    while (DHT11_DQ_IN&&retry<100)//DHT11������40~80us
	{
		retry++;
		delay_us(1);
	};	 
	if(retry>=100)return 1;
	else retry=0;
    while (!DHT11_DQ_IN&&retry<100)//DHT11���ͺ���ٴ�����40~80us
	{
		retry++;
		delay_us(1);
	};
	if(retry>=100)return 1;	    
	return 0;
}

/*-------------------------------------------------*/
/*����������ȡһ��λ                               */
/*��  ������                                       */
/*����ֵ��1��0                                     */
/*-------------------------------------------------*/ 
u8 DHT11_Read_Bit(void) 			 
{
 	u8 retry=0;
	while(DHT11_DQ_IN&&retry<100)//�ȴ���Ϊ�͵�ƽ
	{
		retry++;
		delay_us(1);
	}
	retry=0;
	while(!DHT11_DQ_IN&&retry<100)//�ȴ���ߵ�ƽ
	{
		retry++;
		delay_us(1);
	}
	delay_us(40);//�ȴ�40us
	if(DHT11_DQ_IN)return 1;
	else return 0;		   
}

/*-------------------------------------------------*/
/*����������ȡһ���ֽ�                             */
/*��  ������                                       */
/*����ֵ������                                     */
/*-------------------------------------------------*/ 
u8 DHT11_Read_Byte(void)    
{        
    u8 i,dat;
    dat=0;
	for (i=0;i<8;i++) 
	{
   		dat<<=1; 
	    dat|=DHT11_Read_Bit();
    }						    
    return dat;
}

/*-------------------------------------------------*/
/*����������ȡһ��������ʪ��                       */
/*��  ����temp:�¶�ֵ                              */
/*��  ����humi:ʪ��ֵ                              */
/*����ֵ��1���� 0��ȷ                              */
/*-------------------------------------------------*/ 
u8 DHT11_Read_Data(u8 *temp,u8 *humi)    
{        
 	u8 buf[5];
	u8 i;
	DHT11_Rst();
	if(DHT11_Check()==0)
	{
		for(i=0;i<5;i++)//��ȡ40λ����
		{
			buf[i]=DHT11_Read_Byte();
		}
		if((buf[0]+buf[1]+buf[2]+buf[3])==buf[4])
		{
			*humi=buf[0];
			*temp=buf[2];
		}
	}else return 1;
	return 0;	    
}

/*-------------------------------------------------*/
/*����������ʼ��DHT11                              */
/*��  ������                                       */
/*����ֵ��1���� 0��ȷ                              */
/*-------------------------------------------------*/    	 
u8 DHT11_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//ʹ��GPIOCʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//50MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��
	DHT11_Rst();
	return DHT11_Check();
}
























