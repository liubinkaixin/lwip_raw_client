#define _CRITICALSEG_C_
#include "stm32f4xx.h" 
#include "CriticalSeg.h"
#include "8720.h"

//关键区域嵌套深度最大值
#define CRITICAL_DEEP	30

static uint32_t IRQStatus[CRITICAL_DEEP];
static uint16_t IRQIndex = 0;
uint16_t maxIndex = 0;
uint8_t eth_en = 1;

void STM32_EnableIRQ( void )
{
	if(IRQIndex > 0)
	{
		IRQIndex --;
		if(IRQStatus[IRQIndex])
		{
			ETH_DMAITConfig(ETH_DMA_IT_NIS|ETH_DMA_IT_R,ENABLE);  	//使能以太网接收中断
			eth_en = 1;
		}
		//__set_PRIMASK(IRQStatus[IRQIndex]);
	}
}

void STM32_DisableIRQ( void )
{
	if(IRQIndex < CRITICAL_DEEP)
	{
		IRQStatus[IRQIndex] = eth_en;//__get_PRIMASK();
		IRQIndex ++;
		ETH_DMAITConfig(ETH_DMA_IT_NIS|ETH_DMA_IT_R,DISABLE);  	//使能以太网接收中断
		eth_en = 0;
		//可以写入以下代码进行测试关键区域深度，避免出现溢出情况，从而调整CRITICAL_DEEP大小
//		if(maxIndex < IRQIndex)
//		{
//			maxIndex = IRQIndex;
//		}
//		__asm("CPSID I");
	}
	else//中断关键区域溢出
	{
		while(1);
	}
}


