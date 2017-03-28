/*-------------------------------------------------*/
/*         超纬电子STM32F407ZGT6开发板             */
/*    淘宝地址： http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/
/*-------------------------------------------------*/
/*            DAC源文件,有2个函数                  */
/*-------------------------------------------------*/
/*  void DAC1_Init(void);	      DAC通道1初始化   */ 	 
/*  void DAC1_SetVol(double vol); 设置通道1输出电压*/
/*-------------------------------------------------*/

#include "dac.h"

/*-------------------------------------------------*/
/*函数名：初始化DAC                                */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void DAC1_Init(void)
{  
	GPIO_InitTypeDef  GPIO_InitStructure;
	DAC_InitTypeDef DAC_InitType;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);    //使能GPIOA时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);      //使能DAC时钟
	   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;                //PA4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;             //模拟功能
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;           //下拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);                   //初始化

	DAC_InitType.DAC_Trigger=DAC_Trigger_None;	                      //不使用触发功能 TEN1=0
	DAC_InitType.DAC_WaveGeneration=DAC_WaveGeneration_None;          //不使用波形发生
	DAC_InitType.DAC_LFSRUnmask_TriangleAmplitude=DAC_LFSRUnmask_Bit0;//屏蔽、幅值设置
	DAC_InitType.DAC_OutputBuffer=DAC_OutputBuffer_Disable ;	      //DAC1输出缓存关闭 BOFF1=1
	DAC_Init(DAC_Channel_1,&DAC_InitType);	                          //初始化DAC通道1

	DAC_Cmd(DAC_Channel_1, ENABLE);           //使能DAC通道1
	DAC_SetChannel1Data(DAC_Align_12b_R, 0);  //12位右对齐数据格式设置DAC值
}
/*-------------------------------------------------*/
/*函数名：设置DAC的输出电压                        */
/*参  数：vol：0-3300 对应0-3.3v                   */
/*返回值：无                                       */
/*-------------------------------------------------*/
void DAC1_SetVol(double vol)
{
	vol/=1000;
	vol=vol*4096/3.3;
	DAC_SetChannel1Data(DAC_Align_12b_R,vol);    //设置DAC值
}





















































