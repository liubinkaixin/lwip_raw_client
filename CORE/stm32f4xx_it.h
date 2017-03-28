/*-------------------------------------------------*/
/*          ��γ����STM32F407ZGT6������            */
/*    �Ա���ַ�� http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/
/*-------------------------------------------------*/
/*                                                 */
/*    �жϴ���ͷ�ļ�ģ���ļ�(ST�ṩ,�����޸�)      */
/*                                                 */
/*-------------------------------------------------*/
/*-------------------------------------------------*/

#ifndef __STM32F4xx_IT_H
#define __STM32F4xx_IT_H

#include "stm32f4xx.h"

/*-------------------------------------------------*/
/*                                                 */
/*                    ϵͳ�ж�                     */
/*                                                 */
/*-------------------------------------------------*/

void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);


/*-------------------------------------------------*/
/*                                                 */
/*                    Ӧ���ж�                     */
/*                                                 */
/*-------------------------------------------------*/

#endif 