/*-------------------------------------------------*/
/*         ��γ����STM32F407ZGT6������             */
/*    �Ա���ַ�� http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/
/*-------------------------------------------------*/
/*              TCP������Դ�ļ�                    */
/*-------------------------------------------------*/
/*-------------------------------------------------*/

#include "tcpserver.h" 

const u8 *tcp_server_sendbuf="������Ƿ��������㷢�͵��������յ�\r\n";

/*-------------------------------------------------*/
/*�����������ջص�����                             */
/*��  ����arg���������                            */
/*��  ����pcb��TCP���ƿ�                           */
/*��  ����p�����ݻ�����ָ��                        */
/*��  ����err������ֵ                              */
/*����ֵ����                                       */
/*-------------------------------------------------*/
static err_t tcp_server_recv(void *arg, struct tcp_pcb *pcb,struct pbuf *p,err_t err)
{
	struct pbuf *temp;
	
	temp=arg;        //��ȡ����
	if(p != NULL)    //p�����ݻ�����ָ�룬��Ϊ�ձ�ʾ�����ݵ�
	{
		tcp_recved(pcb, p->tot_len);	              //��������			
		tcp_write(pcb,temp->payload,temp->tot_len,1); //���ʹ����arg������	       
		tcp_write(pcb,p->payload,p->tot_len,1);       //�����յ�����ԭ������
		pbuf_free(p); 								  //�ͷŻ�����
	}
	return ERR_OK;
}

/*-------------------------------------------------*/
/*�����������ӽ���ص�����                         */
/*��  ����arg���������                            */
/*��  ����pcb��TCP���ƿ�                           */
/*��  ����err������ֵ                              */
/*����ֵ����                                       */
/*-------------------------------------------------*/
static err_t  tcp_server_accept(void *arg,struct tcp_pcb *pcb,err_t err)
{	
	struct pbuf *txpuf;                     //ָ�����ݻ�������ָ��               
	
	tcp_setprio(pcb, TCP_PRIO_MIN); 		//���ûص��������ȼ��������ڼ�������ʱ�ر���Ҫ,�˺����������*/
	tcp_recv(pcb,tcp_server_recv); 		    //���ý��պ����ص�����
	
	txpuf=pbuf_alloc(PBUF_TRANSPORT,strlen((char*)tcp_server_sendbuf),PBUF_POOL);   //�����ڴ�ռ�
	pbuf_take(txpuf,(char*)tcp_server_sendbuf,strlen((char*)tcp_server_sendbuf));   //�������
	
	tcp_arg(pcb,txpuf);  //��TCP���ƿ鴫�Ĳ���arg=txpuf  

	return ERR_OK;
}

/*-------------------------------------------------*/
/*��������TCP�������ĳ�ʼ��                        */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void TCP_server_init(void)
{
	struct tcp_pcb *pcb;                    

	pcb = tcp_new(); 						    //�½�һ��TCP���ƿ�
	tcp_bind(pcb,IP_ADDR_ANY,TCP_SERVER_PORT);  //��IP�Ͷ˿ں�
	pcb = tcp_listen(pcb); 					    //�������������
	tcp_accept(pcb,tcp_server_accept); 		    //�������ӽ���ص�����
}
