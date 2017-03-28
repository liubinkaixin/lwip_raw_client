/*-------------------------------------------------*/
/*         ��γ����STM32F407ZGT6������             */
/*    �Ա���ַ�� http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/
#ifndef __8720_H
#define __8720_H

#include "main.h"  

#define PHY_ADDRESS             0x00  //LAN8720�ĵ�ַ�����ݺ��İ�Ӳ�����Ӿ���
#define LWIP_MAX_DHCP_TRIES		4     //DHCP������������Դ���
   
//8720оƬIP��Ϣ���ƽṹ��
typedef struct  
{
	u8 mac[6];      //MAC��ַ
	u8 ip[4];       //����IP��ַ
	u8 netmask[4]; 	//��������
	u8 gateway[4]; 	//Ĭ�����ص�IP��ַ
}__8720ip_dev;

extern u8  MACaddr[6];     //MAC��ַ������Ψһ��
extern u32 LWipTime;       //LWip����ʱ��
extern u8  dhcpstatus;     //DHCP״̬

err_t ethernetif_init(struct netif *netif); //�����ļ��ں�������
err_t ethernetif_input(struct netif *netif);//�����ļ��ں�������

void MylwIP_Init(void);
void LwIP_Pkt_Handle(void);
void lwip_periodic_handle(void);
void GPIO_Configuration(void);
void NVIC_Configuration(void);
void Ethernet_Configuration(void);
void lwip_dhcp_process_handle(void);

#endif
