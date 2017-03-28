/*-------------------------------------------------*/
/*         超纬电子STM32F407ZGT6开发板             */
/*    淘宝地址： http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/
/*-------------------------------------------------*/
/*              TCP客户端头文件                    */
/*-------------------------------------------------*/
/*-------------------------------------------------*/

#ifndef  _TCP_CLIENT_H_
#define  _TCP_CLIENT_H_

#include "main.h"

extern u16 TCP_LOCAL_PORT;          //本地端口

typedef struct{
	u8 connect;
	u8 cnt;
}c_srv_connect_t;

extern  u8 tcp_data[100];

void TCP_CLose(struct tcp_pcb *pcb);
err_t TCP_Client_Send_Data(u8 *buff,u16 length);
err_t  TCP_Client_Recv(void *arg, struct tcp_pcb *pcb,struct pbuf *p,err_t err);
err_t TCP_Connected(void *arg,struct tcp_pcb *pcb,err_t err);
void TCP_Client_Init(u16 local_port, u8 *server_ip, u16 server_port);
struct ip_addr IP_stringTO(u8 *ip);
u16 Char_to_Hex(u8 *char_data, u16 len);
void tcp_connect_check(void);
void echo_snd(void);
#endif
