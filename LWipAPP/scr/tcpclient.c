/*-------------------------------------------------*/
/*         ��������STM32F407ZGT6������             */
/*    �Ա���ַ�� https://shop68304486.taobao.com   */
/*-------------------------------------------------*/
/*-------------------------------------------------*/
/*              TCP�ͻ���Դ�ļ�                    */
/*-------------------------------------------------*/
/*-------------------------------------------------*/

#include "tcpclient.h" 
#include "CriticalSeg.h"

struct tcp_pcb *tcp_client_pcb;                         //TCP�ͻ��˿��ƿ�
u8 connect_flag = 0;                                    //����״̬  0��δ����   1������
u8 tcp_data[100] = "����TCP�ͻ��ˣ����������������\r\n";  //�����Է��͵����� 

u8 echo_flag = 0;


c_srv_connect_t c_srv_ctimout;


u16 TCP_LOCAL_PORT = 5050;          //���ض˿�

/*-------------------------------------------------*/
/*���������ر����Ӻ���                             */
/*��  ����pcb��TCP���ƿ�                           */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void TCP_CLose(struct tcp_pcb *pcb)
{
	tcp_abort(pcb);        //�ر�����
	tcp_arg(pcb,NULL);     //�Ƴ��ص����� 
	tcp_recv(pcb,NULL);    //�Ƴ��ص����� 
	tcp_sent(pcb,NULL);    //�Ƴ��ص����� 
	tcp_err(pcb,NULL);     //�Ƴ��ص����� 
	tcp_poll(pcb,NULL,0);  //�Ƴ��ص�����  
}
//500ms
void tcp_connect_check(void)
{
	struct tcp_pcb *cpcb;   //TCP���ƿ�
// 	err_t err;              //����ֵ
	
	connect_flag = 0;       //����״̬  0��δ����
	for(cpcb = tcp_active_pcbs;cpcb != NULL; cpcb = cpcb->next)                    //��ѯTCP��б�
	{
		if(cpcb->local_port == TCP_LOCAL_PORT && cpcb->remote_port == server_port)  //���TCP_LOCAL_PORT�˿�ָ��������û�жϿ�
		{
			connect_flag = 1;  						//���ӱ�־
			break;							   	
		}
	}
	if(connect_flag == 0)  							                   // TCP_LOCAL_PORTָ���Ķ˿�δ���ӻ��ѶϿ�
	{
		printf("connect server again\r\n");
		TCP_CLose(tcp_client_pcb);                                     //�ر�����
		TCP_Client_Init(TCP_LOCAL_PORT,server_ip,server_port);         //��������
	}
	else
	{
		if(0 == c_srv_ctimout.connect)
		{
			c_srv_ctimout.cnt ++;
			if(c_srv_ctimout.cnt >= 6)
			{
				printf("timeout:connect server again\r\n");
				TCP_CLose(tcp_client_pcb);                                     //�ر�����
				TCP_Client_Init(TCP_LOCAL_PORT,server_ip,server_port);         //��������
			}
		}
		else if(tcp_client_pcb->state == LAST_ACK)
		{
			c_srv_ctimout.cnt ++;
			if(c_srv_ctimout.cnt >= 6)
			{
				printf("LAST_ACK ERR:connect server again\r\n");
				TCP_CLose(tcp_client_pcb);                                     //�ر�����
				TCP_Client_Init(TCP_LOCAL_PORT,server_ip,server_port);         //��������
			}
		}
	}
}

void echo_snd(void)
{
	if(1 == echo_flag)
	{
		echo_flag = 0;
		OS_ENTER_CRITICAL;	//��echo snd����ԭ�Ӳ���
		if(ERR_OK == tcp_write(tcp_client_pcb,"www.zkaifa.com\r\n",strlen("www.zkaifa.com\r\n"),1))
		{
			printf("zkaifa:ERR_OK\r\n");
		}
		else
		{
			printf("zkaifa:ERR_MEM\r\n");
		}
		//tcp_output(tcp_client_pcb);
		if(ERR_OK == tcp_write(tcp_client_pcb,"www.xuanmikeji.com\r\n",strlen("www.xuanmikeji.com\r\n"),1))
		{
			printf("xuanmikeji:ERR_OK\r\n");
		}
		else
		{
			printf("xuanmikeji:ERR_MEM\r\n");
		}	
		OS_EXIT_CRITICAL;
		//tcp_output(tcp_client_pcb);
	}
}

/*-------------------------------------------------*/
/*��������TCP���ջص�����                          */
/*��  ����arg:�������Ĳ���                         */
/*��  ����pcb:TCP���ƿ�                            */
/*��  ����p:���ݻ���ָ��                           */
/*��  ����err:����ֵ                               */
/*����ֵ������ֵ                                   */
/*-------------------------------------------------*/
err_t  TCP_Client_Recv(void *arg, struct tcp_pcb *pcb,struct pbuf *p,err_t err)
{
	if(p != NULL)                                   //���ݻ��治Ϊ��ָ��
	{
		tcp_recved(pcb, p->tot_len);				//��ȡ���ݳ��� tot_len,����д��
		
		echo_flag = 1;
		pbuf_free(p); 			                    //����д�ģ��ͷ�ָ��				    
	}
	else	 										//���ݻ���Ϊ�գ��������Ͽ������ˣ���ͻ���ҲӦ�Ͽ�
	{
		tcp_close(pcb);                             //�Ͽ����� 						
	}
	return ERR_OK;	
}



/*-------------------------------------------------*/
/*�����������Ϸ������Ļص�����                     */
/*��  ����arg:��Ҫ���ݵĲ���������Ҫ���Ͳ���       */
/*��  ����pcb:TCP���ƿ�                            */
/*��  ����err:����ֵ                               */
/*����ֵ������ֵ                                   */
/*-------------------------------------------------*/
err_t TCP_Connected(void *arg,struct tcp_pcb *pcb,err_t err)
{
	c_srv_ctimout.connect = 1;
	printf("connect server ok\r\n");
	tcp_recv(pcb,TCP_Client_Recv);  //����tcp���ջص�����
	return ERR_OK;                  //����OK
}

void  TCP_Client_Err(void *arg, err_t err)
{
	printf("---------------TCP_Client_Err:%d---------------\r\n",err);
}

/*-------------------------------------------------*/
/*��������TCP�ͻ��˵ĳ�ʼ��                        */
/*��  ����local_port:�ͻ��˶˿ں�                  */
/*��  ����server_ip:����IP                         */
/*��  ����server_port:�������˿ں�                 */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void TCP_Client_Init(u16 local_port, u8 *server_ip, u16 server_port)
{
	struct ip_addr serverip;                                       //ip��ַ
	
	TCP_LOCAL_PORT += 1;
	if(TCP_LOCAL_PORT < 5000)
	{
		TCP_LOCAL_PORT = 5000;
	}
	local_port = TCP_LOCAL_PORT;
	serverip=IP_stringTO(server_ip);                               //���ַ��͵�IPת�������ֵ�
	
	tcp_client_pcb = tcp_new();                                    //����TCP���ƿ�
 	tcp_bind(tcp_client_pcb,IP_ADDR_ANY,local_port);               //�󶨱���IP��ַ�Ͷ˿ں� 
  tcp_connect(tcp_client_pcb,&serverip,server_port,TCP_Connected);//ע�������Ϸ������Ļص�����
	tcp_client_pcb->so_options |= SOF_KEEPALIVE;
	
	tcp_err(tcp_client_pcb,TCP_Client_Err);
	
	c_srv_ctimout.connect = 0;
	c_srv_ctimout.cnt = 0;
}

/*-------------------------------------------------*/
/*�����������ַ���....��ʽ��IPת��������IP         */
/*��  ����char_data���ַ�������                    */
/*����ֵ������IP                                   */
/*-------------------------------------------------*/
struct ip_addr IP_stringTO(u8 *ip)
{
	u8  ip1[4],ip2[4],ip3[4],ip4[4];   //�ַ�����ʽ��IP
	struct ip_addr ipaddr;            //ip��ַ
	u8 a,b,c,d;
	
	sscanf((const char *)ip,"%[^.].%[^.].%[^.].%[^.]",ip1,ip2,ip3,ip4);  //��ʽ����ȡ
	
	a=Char_to_Hex(ip1,strlen((const char *)ip1));
	b=Char_to_Hex(ip2,strlen((const char *)ip2));
	c=Char_to_Hex(ip3,strlen((const char *)ip3));
	d=Char_to_Hex(ip4,strlen((const char *)ip4));

	IP4_ADDR(&ipaddr,a,b,c,d);          
	
	return ipaddr;
}

/*-------------------------------------------------*/
/*���������ַ�������ת�������֣�0-99999            */
/*��  ����char_data���ַ�������                    */
/*��  ����len��      λ��                          */
/*����ֵ��ת���������                             */
/*-------------------------------------------------*/
u16 Char_to_Hex(u8 *char_data, u16 len)
{
	u16 temp;
	
	switch(len)
	{
		case 1 : temp=(char_data[0]-0x30)*1;
				 break;
		case 2 : temp=(char_data[0]-0x30)*10;
				 temp+=(char_data[1]-0x30)*1;
				 break;
		case 3 : temp=(char_data[0]-0x30)*100;
				 temp+=(char_data[1]-0x30)*10;
				 temp+=(char_data[2]-0x30)*1;
				 break;
		case 4 : temp=(char_data[0]-0x30)*1000;
				 temp+=(char_data[1]-0x30)*100;
				 temp+=(char_data[2]-0x30)*10;
				 temp+=(char_data[3]-0x30)*1;
				 break;
		case 5 : temp=(char_data[0]-0x30)*10000;
				 temp+=(char_data[1]-0x30)*1000;
				 temp+=(char_data[2]-0x30)*100;
				 temp+=(char_data[3]-0x30)*10;
				 temp+=(char_data[4]-0x30)*1;
				 break;
	}
	return temp;
}

