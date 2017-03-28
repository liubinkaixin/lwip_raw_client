/*-------------------------------------------------*/
/*         ��γ����STM32F407ZGT6������             */
/*    �Ա���ַ�� http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/
/*-------------------------------------------------*/
/*            18b20Դ�ļ�,��8������                */
/*-------------------------------------------------*/
/* u8 DS18B20_Init(void);			��ʼ��DS18B20  */
/* double DS18B20_Get_Temp(void);	��ȡ�¶�       */
/* void DS18B20_Start(void);		��ʼ�¶�ת��   */
/* void DS18B20_Write_Byte(u8 dat); д��һ���ֽ�   */
/* u8 DS18B20_Read_Byte(void);		����һ���ֽ�   */
/* u8 DS18B20_Read_Bit(void);		����һ��λ     */
/* u8 DS18B20_Check(void);			���DS18B20    */
/* void DS18B20_Rst(void);			��λDS18B20    */ 
/*-------------------------------------------------*/

#include "ds18b20.h"
#include "delay.h"	
  
/*-------------------------------------------------*/
/*����������λDS18B20����                          */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void DS18B20_Rst(void)	   
{                 
	DS18B20_IO_OUT(); //SET PG11 OUTPUT
    DS18B20_DQ_OUT=0; //����DQ
    delay_us(750);    //����750us
    DS18B20_DQ_OUT=1; //DQ=1 
	delay_us(15);     //15US
}

/*-------------------------------------------------*/
/*���������ȴ�DS18B20�Ļ�Ӧ                        */
/*��  ������                                       */
/*����ֵ��1:δ��⵽DS18B20�Ĵ���                  */
/*����ֵ��0:����                                   */  
/*-------------------------------------------------*/
u8 DS18B20_Check(void) 	   
{   
	u8 retry=0;
	DS18B20_IO_IN(); 
    while (DS18B20_DQ_IN&&retry<200)
	{
		retry++;
		delay_us(1);
	};	 
	if(retry>=200)return 1;
	else retry=0;
    while (!DS18B20_DQ_IN&&retry<240)
	{
		retry++;
		delay_us(1);
	};
	if(retry>=240)return 1;	    
	return 0;
}

/*-------------------------------------------------*/
/*����������ȡһ��λ                               */
/*��  ������                                       */
/*����ֵ��1��0                                     */ 
/*-------------------------------------------------*/
u8 DS18B20_Read_Bit(void) 		
{
	u8 data;
	
	DS18B20_IO_OUT();
	DS18B20_DQ_OUT=0; 
	delay_us(2);
	DS18B20_DQ_OUT=1; 
	DS18B20_IO_IN();
	delay_us(12);
	if(DS18B20_DQ_IN)data=1;
	else data=0;	 
	delay_us(50);           
	return data;
}

/*-------------------------------------------------*/
/*����������ȡһ���ֽ�                             */
/*��  ������                                       */
/*����ֵ������                                     */ 
/*-------------------------------------------------*/
u8 DS18B20_Read_Byte(void)    
{        
    u8 i,j,dat;
    dat=0;
	for (i=1;i<=8;i++) 
	{
        j=DS18B20_Read_Bit();
        dat=(j<<7)|(dat>>1);
    }						    
    return dat;
}

/*-------------------------------------------------*/
/*��������дһ���ֽ�                               */
/*��  ����dat������                                */
/*����ֵ����                                       */ 
/*-------------------------------------------------*/
void DS18B20_Write_Byte(u8 dat)     
 {             
    u8 j;
    u8 testb;
	  DS18B20_IO_OUT();
    for (j=1;j<=8;j++) 
	{
        testb=dat&0x01;
        dat=dat>>1;
        if (testb) 
        {
            DS18B20_DQ_OUT=0;
            delay_us(2);                            
            DS18B20_DQ_OUT=1;
            delay_us(60);             
        }
        else 
        {
            DS18B20_DQ_OUT=0;// Write 0
            delay_us(60);             
            DS18B20_DQ_OUT=1;
            delay_us(2);                          
        }
    }
}

/*-------------------------------------------------*/
/*���������¶�ת��                                 */
/*��  ������                                       */
/*����ֵ����                                       */ 
/*-------------------------------------------------*/
void DS18B20_Start(void)
{   						               
    DS18B20_Rst();	   
	DS18B20_Check();	 
    DS18B20_Write_Byte(0xcc);
    DS18B20_Write_Byte(0x44);
} 

/*-------------------------------------------------*/
/*����������ʼ��DS18B20                            */
/*��  ������                                       */
/*����ֵ��0����ȷ  1������                         */ 
/*-------------------------------------------------*/  	 
u8 DS18B20_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//ʹ��GPIOCʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//50MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;          //����
	GPIO_Init(GPIOC, &GPIO_InitStructure);                //��ʼ��
 
 	DS18B20_Rst();
	return DS18B20_Check();
} 

/*-------------------------------------------------*/
/*���������õ��¶�ֵ                               */
/*��  ������                                       */
/*����ֵ���¶�ֵ                                   */ 
/*-------------------------------------------------*/ 
double DS18B20_Get_Temp(void)
{
    u8 temp;
    u8 TL,TH;
	short tem_temp;
	double tem;
	
    DS18B20_Start ();                   
    DS18B20_Rst();
    DS18B20_Check();	 
    DS18B20_Write_Byte(0xcc);
    DS18B20_Write_Byte(0xbe);
    TL=DS18B20_Read_Byte();  
    TH=DS18B20_Read_Byte(); 
    if(TH>7)
    {
        TH=~TH;
        TL=~TL; 
        temp=0;//�¶�Ϊ��  
    }else temp=1;//�¶�Ϊ��	  	  
    tem_temp=TH; //��ø߰�λ
    tem_temp<<=8;    
    tem_temp+=TL;//��õװ�λ
    tem=(double)tem_temp*0.0625;//ת��     
	
	if(temp)return tem; //�����¶�ֵ
	else return -tem;    
}
