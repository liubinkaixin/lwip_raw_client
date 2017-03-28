/*-------------------------------------------------*/
/*         助开发网STM32F407ZGT6开发板             */
/*    淘宝地址： https://shop68304486.taobao.com   */
/*-------------------------------------------------*/
/*-------------------------------------------------*/
/*              TCP客户端源文件                    */
/*-------------------------------------------------*/
/*-------------------------------------------------*/

#include "tcpclient.h" 
#include "CriticalSeg.h"

struct tcp_pcb *tcp_client_pcb;                         //TCP客户端控制块
u8 connect_flag = 0;                                    //连接状态  0：未连接   1：连接
u8 tcp_data[100] = "我是TCP客户端，请服务器发送数据\r\n";  //周期性发送的数据 

u8 echo_flag = 0;


c_srv_connect_t c_srv_ctimout;


u16 TCP_LOCAL_PORT = 5050;          //本地端口

/*-------------------------------------------------*/
/*函数名：关闭连接函数                             */
/*参  数：pcb：TCP控制块                           */
/*返回值：无                                       */
/*-------------------------------------------------*/
void TCP_CLose(struct tcp_pcb *pcb)
{
	tcp_abort(pcb);        //关闭连接
	tcp_arg(pcb,NULL);     //移除回调函数 
	tcp_recv(pcb,NULL);    //移除回调函数 
	tcp_sent(pcb,NULL);    //移除回调函数 
	tcp_err(pcb,NULL);     //移除回调函数 
	tcp_poll(pcb,NULL,0);  //移除回调函数  
}
//500ms
void tcp_connect_check(void)
{
	struct tcp_pcb *cpcb;   //TCP控制块
// 	err_t err;              //错误值
	
	connect_flag = 0;       //连接状态  0：未连接
	for(cpcb = tcp_active_pcbs;cpcb != NULL; cpcb = cpcb->next)                    //轮询TCP活动列表
	{
		if(cpcb->local_port == TCP_LOCAL_PORT && cpcb->remote_port == server_port)  //如果TCP_LOCAL_PORT端口指定的连接没有断开
		{
			connect_flag = 1;  						//连接标志
			break;							   	
		}
	}
	if(connect_flag == 0)  							                   // TCP_LOCAL_PORT指定的端口未连接或已断开
	{
		printf("connect server again\r\n");
		TCP_CLose(tcp_client_pcb);                                     //关闭连接
		TCP_Client_Init(TCP_LOCAL_PORT,server_ip,server_port);         //重新连接
	}
	else
	{
		if(0 == c_srv_ctimout.connect)
		{
			c_srv_ctimout.cnt ++;
			if(c_srv_ctimout.cnt >= 6)
			{
				printf("timeout:connect server again\r\n");
				TCP_CLose(tcp_client_pcb);                                     //关闭连接
				TCP_Client_Init(TCP_LOCAL_PORT,server_ip,server_port);         //重新连接
			}
		}
		else if(tcp_client_pcb->state == LAST_ACK)
		{
			c_srv_ctimout.cnt ++;
			if(c_srv_ctimout.cnt >= 6)
			{
				printf("LAST_ACK ERR:connect server again\r\n");
				TCP_CLose(tcp_client_pcb);                                     //关闭连接
				TCP_Client_Init(TCP_LOCAL_PORT,server_ip,server_port);         //重新连接
			}
		}
	}
}

void echo_snd(void)
{
	if(1 == echo_flag)
	{
		echo_flag = 0;
		OS_ENTER_CRITICAL;	//给echo snd增加原子操作
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
/*函数名：TCP接收回调函数                          */
/*参  数：arg:传递来的参数                         */
/*参  数：pcb:TCP控制块                            */
/*参  数：p:数据缓存指针                           */
/*参  数：err:错误值                               */
/*返回值：错误值                                   */
/*-------------------------------------------------*/
err_t  TCP_Client_Recv(void *arg, struct tcp_pcb *pcb,struct pbuf *p,err_t err)
{
	if(p != NULL)                                   //数据缓存不为空指针
	{
		tcp_recved(pcb, p->tot_len);				//获取数据长度 tot_len,必须写的
		
		echo_flag = 1;
		pbuf_free(p); 			                    //必须写的，释放指针				    
	}
	else	 										//数据缓存为空，服务器断开连接了，则客户端也应断开
	{
		tcp_close(pcb);                             //断开连接 						
	}
	return ERR_OK;	
}



/*-------------------------------------------------*/
/*函数名：接上服务器的回调函数                     */
/*参  数：arg:需要传递的参数，不需要传就不用       */
/*参  数：pcb:TCP控制块                            */
/*参  数：err:错误值                               */
/*返回值：错误值                                   */
/*-------------------------------------------------*/
err_t TCP_Connected(void *arg,struct tcp_pcb *pcb,err_t err)
{
	c_srv_ctimout.connect = 1;
	printf("connect server ok\r\n");
	tcp_recv(pcb,TCP_Client_Recv);  //设置tcp接收回调函数
	return ERR_OK;                  //返回OK
}

void  TCP_Client_Err(void *arg, err_t err)
{
	printf("---------------TCP_Client_Err:%d---------------\r\n",err);
}

/*-------------------------------------------------*/
/*函数名：TCP客户端的初始化                        */
/*参  数：local_port:客户端端口号                  */
/*参  数：server_ip:务器IP                         */
/*参  数：server_port:服务器端口号                 */
/*返回值：无                                       */
/*-------------------------------------------------*/
void TCP_Client_Init(u16 local_port, u8 *server_ip, u16 server_port)
{
	struct ip_addr serverip;                                       //ip地址
	
	TCP_LOCAL_PORT += 1;
	if(TCP_LOCAL_PORT < 5000)
	{
		TCP_LOCAL_PORT = 5000;
	}
	local_port = TCP_LOCAL_PORT;
	serverip=IP_stringTO(server_ip);                               //将字符型的IP转换成数字的
	
	tcp_client_pcb = tcp_new();                                    //建立TCP控制块
 	tcp_bind(tcp_client_pcb,IP_ADDR_ANY,local_port);               //绑定本地IP地址和端口号 
  tcp_connect(tcp_client_pcb,&serverip,server_port,TCP_Connected);//注册连接上服务器的回调函数
	tcp_client_pcb->so_options |= SOF_KEEPALIVE;
	
	tcp_err(tcp_client_pcb,TCP_Client_Err);
	
	c_srv_ctimout.connect = 0;
	c_srv_ctimout.cnt = 0;
}

/*-------------------------------------------------*/
/*函数名：将字符串....格式的IP转化成数字IP         */
/*参  数：char_data：字符型数字                    */
/*返回值：数字IP                                   */
/*-------------------------------------------------*/
struct ip_addr IP_stringTO(u8 *ip)
{
	u8  ip1[4],ip2[4],ip3[4],ip4[4];   //字符串格式的IP
	struct ip_addr ipaddr;            //ip地址
	u8 a,b,c,d;
	
	sscanf((const char *)ip,"%[^.].%[^.].%[^.].%[^.]",ip1,ip2,ip3,ip4);  //格式化截取
	
	a=Char_to_Hex(ip1,strlen((const char *)ip1));
	b=Char_to_Hex(ip2,strlen((const char *)ip2));
	c=Char_to_Hex(ip3,strlen((const char *)ip3));
	d=Char_to_Hex(ip4,strlen((const char *)ip4));

	IP4_ADDR(&ipaddr,a,b,c,d);          
	
	return ipaddr;
}

/*-------------------------------------------------*/
/*函数名：字符型数字转换成数字，0-99999            */
/*参  数：char_data：字符型数字                    */
/*参  数：len：      位数                          */
/*返回值：转换后的数字                             */
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

