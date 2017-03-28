/*-------------------------------------------------*/
/*         超纬电子STM32F407ZGT6开发板             */
/*    淘宝地址： http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/
/*-------------------------------------------------*/
/*              TCP服务器源文件                    */
/*-------------------------------------------------*/
/*-------------------------------------------------*/

#include "tcpserver.h" 

const u8 *tcp_server_sendbuf="你好我是服务器，你发送的数据已收到\r\n";

/*-------------------------------------------------*/
/*函数名：接收回调函数                             */
/*参  数：arg：传入参数                            */
/*参  数：pcb：TCP控制块                           */
/*参  数：p：数据缓冲区指针                        */
/*参  数：err：错误值                              */
/*返回值：无                                       */
/*-------------------------------------------------*/
static err_t tcp_server_recv(void *arg, struct tcp_pcb *pcb,struct pbuf *p,err_t err)
{
	struct pbuf *temp;
	
	temp=arg;        //获取参数
	if(p != NULL)    //p是数据缓冲区指针，不为空表示有数据到
	{
		tcp_recved(pcb, p->tot_len);	              //接收数据			
		tcp_write(pcb,temp->payload,temp->tot_len,1); //发送传入的arg的数据	       
		tcp_write(pcb,p->payload,p->tot_len,1);       //将接收的数据原样返回
		pbuf_free(p); 								  //释放缓冲区
	}
	return ERR_OK;
}

/*-------------------------------------------------*/
/*函数名：连接接入回调函数                         */
/*参  数：arg：传入参数                            */
/*参  数：pcb：TCP控制块                           */
/*参  数：err：错误值                              */
/*返回值：无                                       */
/*-------------------------------------------------*/
static err_t  tcp_server_accept(void *arg,struct tcp_pcb *pcb,err_t err)
{	
	struct pbuf *txpuf;                     //指向数据缓存区的指针               
	
	tcp_setprio(pcb, TCP_PRIO_MIN); 		//设置回调函数优先级，当存在几个连接时特别重要,此函数必须调用*/
	tcp_recv(pcb,tcp_server_recv); 		    //设置接收函数回调函数
	
	txpuf=pbuf_alloc(PBUF_TRANSPORT,strlen((char*)tcp_server_sendbuf),PBUF_POOL);   //申请内存空间
	pbuf_take(txpuf,(char*)tcp_server_sendbuf,strlen((char*)tcp_server_sendbuf));   //填充数据
	
	tcp_arg(pcb,txpuf);  //向TCP控制块传的参数arg=txpuf  

	return ERR_OK;
}

/*-------------------------------------------------*/
/*函数名：TCP服务器的初始化                        */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void TCP_server_init(void)
{
	struct tcp_pcb *pcb;                    

	pcb = tcp_new(); 						    //新建一个TCP控制块
	tcp_bind(pcb,IP_ADDR_ANY,TCP_SERVER_PORT);  //绑定IP和端口号
	pcb = tcp_listen(pcb); 					    //服务器进入监听
	tcp_accept(pcb,tcp_server_accept); 		    //设置连接接入回调函数
}
