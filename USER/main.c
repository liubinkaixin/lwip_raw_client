/*-------------------------------------------------*/
/*          ��������STM32F407ZGT6������            */
/*    �Ա���ַ�� https://shop68304486.taobao.com   */
/*-------------------------------------------------*/
/*      76-��̫��ͨ��֮TCP�ͻ���(�㱼-��̬IP)      */
/*-------------------------------------------------*/ 
/*             ���̼���꿴readme.txt              */
/*-------------------------------------------------*/ 

#include "main.h"
#include "delay.h" 
#include "usart1.h" 
#include "8720.h" 
#include "tcpclient.h" 
#include "timer3.h"
#include "CriticalSeg.h"

u8  Server_flag=0;       //��־λ 0:δ�յ���������IP�Ͷ˿ں���Ϣ   1���յ��˷�������IP�Ͷ˿ں���Ϣ
u8  server_ip[20]="121.40.173.197";       //������IP
//u8  server_ip[20]="192.168.6.105";       //������IP
u16 server_port=8823;         //�������˿ں�0-9999

void data_led_cpl(void)
{
	GPIO_WriteBit(GPIOC,GPIO_Pin_6,(BitAction)(1 - GPIO_ReadOutputDataBit(GPIOC,GPIO_Pin_6)));
}

void dma_led_cpl(void)
{
	GPIO_WriteBit(GPIOC,GPIO_Pin_2,(BitAction)(1 - GPIO_ReadOutputDataBit(GPIOC,GPIO_Pin_2)));
}

void connect_led(u8 sts)
{
	GPIO_WriteBit(GPIOC,GPIO_Pin_7,(BitAction)(sts));
}

void fault_led_show(void)
{
	GPIO_WriteBit(GPIOC,GPIO_Pin_8,(BitAction)(1));
}

void led_gpio_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//ʹ��GPIOʱ�� RMII�ӿ�
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2| GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	//GPIO_ReadOutputDataBit
	GPIO_WriteBit(GPIOC,GPIO_Pin_6,(BitAction)(0));
	GPIO_WriteBit(GPIOC,GPIO_Pin_7,(BitAction)(0));
	GPIO_WriteBit(GPIOC,GPIO_Pin_8,(BitAction)(0));
}

int main(void)
{	
//	u16 len;
	
	delay_init();		        //��ʱ��ʼ�� 
	Usart1_init(115200);        //����1��ʼ��
	printf("hello www.zkaifa.com\r\n");
	Server_flag=0;
	led_gpio_init();
	Server_flag=1;
	
	GPIO_Configuration();       //��̫���ӿ�����
	NVIC_Configuration();       //��̫���ж�����
	Ethernet_Configuration();   //��̫������
  TIM3_Init(100,8400);	    //10ms����ʱ
	MylwIP_Init();	            //��ʼ��lwip����ǰ������
	TCP_Client_Init(TCP_LOCAL_PORT,server_ip,server_port);   //�ͻ��˳�ʼ��
	//OS_ENTER_CRITICAL;	
	while (1)
	{    
		lwip_periodic_handle(); //lwIP���ڴ�������
		echo_snd();
	}
}
