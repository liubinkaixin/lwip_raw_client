/*-------------------------------------------------*/
/*         超纬电子STM32F407ZGT6开发板             */
/*    淘宝地址： http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/
#ifndef __8720_H
#define __8720_H

#include "main.h"  

#define PHY_ADDRESS             0x00  //LAN8720的地址，根据核心板硬件连接决定
#define LWIP_MAX_DHCP_TRIES		4     //DHCP服务器最大重试次数
   
//8720芯片IP信息控制结构体
typedef struct  
{
	u8 mac[6];      //MAC地址
	u8 ip[4];       //本机IP地址
	u8 netmask[4]; 	//子网掩码
	u8 gateway[4]; 	//默认网关的IP地址
}__8720ip_dev;

extern u8  MACaddr[6];     //MAC地址，具有唯一性
extern u32 LWipTime;       //LWip周期时钟
extern u8  dhcpstatus;     //DHCP状态

err_t ethernetif_init(struct netif *netif); //其他文件内函数声明
err_t ethernetif_input(struct netif *netif);//其他文件内函数声明

void MylwIP_Init(void);
void LwIP_Pkt_Handle(void);
void lwip_periodic_handle(void);
void GPIO_Configuration(void);
void NVIC_Configuration(void);
void Ethernet_Configuration(void);
void lwip_dhcp_process_handle(void);

#endif
