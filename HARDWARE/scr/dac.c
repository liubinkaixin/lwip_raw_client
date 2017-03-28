/*-------------------------------------------------*/
/*         ��γ����STM32F407ZGT6������             */
/*    �Ա���ַ�� http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/
/*-------------------------------------------------*/
/*            DACԴ�ļ�,��2������                  */
/*-------------------------------------------------*/
/*  void DAC1_Init(void);	      DACͨ��1��ʼ��   */ 	 
/*  void DAC1_SetVol(double vol); ����ͨ��1�����ѹ*/
/*-------------------------------------------------*/

#include "dac.h"

/*-------------------------------------------------*/
/*����������ʼ��DAC                                */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void DAC1_Init(void)
{  
	GPIO_InitTypeDef  GPIO_InitStructure;
	DAC_InitTypeDef DAC_InitType;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);    //ʹ��GPIOAʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);      //ʹ��DACʱ��
	   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;                //PA4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;             //ģ�⹦��
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;           //����
	GPIO_Init(GPIOA, &GPIO_InitStructure);                   //��ʼ��

	DAC_InitType.DAC_Trigger=DAC_Trigger_None;	                      //��ʹ�ô������� TEN1=0
	DAC_InitType.DAC_WaveGeneration=DAC_WaveGeneration_None;          //��ʹ�ò��η���
	DAC_InitType.DAC_LFSRUnmask_TriangleAmplitude=DAC_LFSRUnmask_Bit0;//���Ρ���ֵ����
	DAC_InitType.DAC_OutputBuffer=DAC_OutputBuffer_Disable ;	      //DAC1�������ر� BOFF1=1
	DAC_Init(DAC_Channel_1,&DAC_InitType);	                          //��ʼ��DACͨ��1

	DAC_Cmd(DAC_Channel_1, ENABLE);           //ʹ��DACͨ��1
	DAC_SetChannel1Data(DAC_Align_12b_R, 0);  //12λ�Ҷ������ݸ�ʽ����DACֵ
}
/*-------------------------------------------------*/
/*������������DAC�������ѹ                        */
/*��  ����vol��0-3300 ��Ӧ0-3.3v                   */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void DAC1_SetVol(double vol)
{
	vol/=1000;
	vol=vol*4096/3.3;
	DAC_SetChannel1Data(DAC_Align_12b_R,vol);    //����DACֵ
}





















































