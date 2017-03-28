/*-------------------------------------------------*/
/*         ��γ����STM32F407ZGT6������             */
/*    �Ա���ַ�� http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/
/*-------------------------------------------------*/
/*                ESP8266Դ�ļ�                    */
/*-------------------------------------------------*/
/*-------------------------------------------------*/

#include "esp8266.h"
#include "usart1.h"
#include "usart3.h"
#include "delay.h"
#include "string.h"
#include "lcd.h"
#include "buzzer.h" 
#include "jdq.h" 
#include "led.h"
#include "rgb_led.h"
#include "dht11.h"
#include "bh1750.h"

u8 Esp8266_Buff[BUFF_LEN];      //���ݻ�����
u8 IP_Buff[20];                 //���8266��IP
u8 Transmission_Sta;            //����״̬��1��ָ��״̬    0������״̬

/*-------------------------------------------------*/
/*��������ESP8266�������Ӻ���                      */
/*��  ����timeout����ʱʱ�䣨100ms�ı�����         */
/*����ֵ��0����ȷ   ����������                     */
/*-------------------------------------------------*/
u8 ESP8266_SmartLink(u16 timeout)
{
	u16  time=0;                                              //��ʱ������
	
	if(ESP8266_SoftReset(50))return 1;                        //�����λ8266����ʱʱ��5s��ʧ�ܾͷ���1
	else printf("��λ�ɹ�,�ȴ�10���,������������\r\n");    //������ʾ��λ�ɹ�
	
 	delay_ms(10000);                                          //��ʱ10s
	
	if(ESP8266_SendCmd_OK("AT+CWSMARTSTART=1",50))return 2;   //����8266�������ӣ���ʱʱ��5s��ʧ�ܷ���2
	else printf("�����������ӳɹ�����ʹ��APP�������\r\n");   //������ʾ��Ϣ
 
	USART3_RX_STA=0;                            //����3״̬��ʶ����                            
	memset(USART3_RX_BUF,0,USART3_RXBUFF_SIZE); //��մ���3���ջ�����	
	time=0;                                     //��ʱ����������
	while(time<timeout)                         //�ȴ���ʱ���
	{
		if(strstr( (const char *)USART3_RX_BUF , (const char *)"SMART SUCCESS"))//������ڽ��ܵ�SMART SUCCESS��ʾSmartLink�ɹ�
			break;       //����whileѭ��
		delay_ms(100);   //��ʱ100ms
		time++;          //��ʱ������+1   
	}	
	if(time>=timeout)return 3;  //SmartLink��ʱ���󣬷���3
	else                                      
	{
		return 0;              //��ȷ����0
	}
}

/*-------------------------------------------------*/
/*��������8266����ָ��ڴ��Ļش���OK             */
/*��  ����cmd��ָ��                                */
/*��  ����timeout����ʱʱ�䣨100ms�ı�����         */
/*����ֵ��0����ȷ   ����������                     */
/*-------------------------------------------------*/
u8 ESP8266_SendCmd_OK(u8 *cmd, u16 timeout)
{
    u16 time;                                   //��ʱ������
	
	USART3_RX_STA=0;                            //����3״̬��ʶ����                            
	memset(USART3_RX_BUF,0,USART3_RXBUFF_SIZE); //��մ���3���ջ�����
	u3_printf("%s\r\n",cmd);                    //����ָ��
	time=0;                                     //��ʱ����������
	
	while(time<timeout)                         //�ȴ���ʱ���
	{
		if(strstr( (const char *)USART3_RX_BUF , (const char *)"\r\nOK\r\n"))//������ܵ�OK��ʾָ��ɹ�
			break;       //����whileѭ��
		delay_ms(100);   //��ʱ100ms
		time++;          //��ʱ������+1
	}
	if(time>=timeout)return 1;   //��ʱ���󣬷���1
	else return 0;		         //�޴�����0
}

/*-------------------------------------------------*/
/*��������ESP8266�����λ                          */
/*��  ����timeout����ʱʱ�䣨100ms�ı�����         */
/*����ֵ��0����ȷ   ����������                     */
/*-------------------------------------------------*/
u8 ESP8266_SoftReset(u16 timeout)
{
    u16 time;                                   //��ʱ������
	
	USART3_RX_STA=0;                            //����3״̬��ʶ����                            
	memset(USART3_RX_BUF,0,USART3_RXBUFF_SIZE); //��մ���3���ջ�����                       
	u3_printf("AT+RST\r\n");                    //���������λָ��
	time=0;                                     //��ʱ����������
	
	while(time<timeout)                         //�ȴ���ʱ���
	{
		if(strstr( (const char *)USART3_RX_BUF , (const char *)"Ai-Thinker Technology"))//������ܵ�Ai-Thinker Technology��ʾ��λ�ɹ�
			break;       //����whileѭ��
		delay_ms(100);   //��ʱ100ms
		time++;          //��ʱ������+1   
	}
	if(time>=timeout)return 1;       //��ʱ���󣬷���1
	else return 0;	                 //�޴�����0
}

/*-------------------------------------------------*/
/*���������ȴ�����wifi����ȡIP��ַ                 */
/*��  ����ip������IP������                         */
/*��  ����timeout����ʱʱ�䣨100ms�ı�����         */
/*����ֵ��0����ȷ   ����������                     */
/*-------------------------------------------------*/
u8 ESP8266_GetIP(u8 *ip, u16 timeout)
{
	u16  time=0;                //��ʱ������
	char *presult1,*presult2;
	
	printf("\r\n2����ʱ��׼�������λ\r\n");        //������ʾ��Ϣ
	delay_ms(2000);                                 //��ʱ2s
	ESP8266_Exit_Transfer(10);                      //����Ƿ���͸��״̬����������˳�͸��״̬	
	if(ESP8266_SoftReset(50))return 1;              //�����λ8266����ʱʱ��5s��ʧ�ܾͷ���1
	else printf("��λ�ɹ�,�ȴ�������wifi\r\n");   //������ʾ��Ϣ
	
	time=0;                                //��ʱ����������	
	while(time<timeout)                    //�ȴ���ʱ���
	{
		if(strstr( (const char *)USART3_RX_BUF , (const char *)"WIFI GOT IP"))//�ȴ�����WIFI GOT IP
			break;       //����whileѭ��
		delay_ms(100);   //��ʱ100ms
		time++;          //��ʱ������+1         
	}
	if(time>=timeout)return 2; //��ʱ����û�еõ�IP����2
	else                       //��������wifi���õ���IP��׼�����Ͳ�ѯIPָ��      
	{
		printf("������wifi����ѯģ��õ���IP��ַ\r\n");   //������ʾ��Ϣ
		USART3_RX_STA=0;                                  //����3״̬��ʶ����                            
		memset(USART3_RX_BUF,0,USART3_RXBUFF_SIZE);       //��մ���3���ջ����� 	  
		u3_printf("AT+CIFSR\r\n");                        //���Ͳ�ѯIPָ��
		time=0;                                           //��ʱ����������	
		while(time<timeout)                               //�ȴ���ʱ���
		{
			if(strstr( (const char *)USART3_RX_BUF , (const char *)"\r\nOK\r\n"))//������ܵ�OK��ʾָ��ɹ�
				break;       //����whileѭ��
			delay_ms(100);   //��ʱ100ms
			time++;          //��ʱ������+1     
		}
		if(time>=timeout)return 3;   //��ѯIP��ʱ���󣬷���3
		else                         //��ȷ   
		{
			presult1 = strstr( (const char *)USART3_RX_BUF , (const char *)"\"");
			if( presult1 != NULL )
			{
				presult2 = strstr( (const char *)presult1+1 ,    (const char *)"\"");
				if( presult2 != NULL )
				{
					memcpy(ip,presult1+1,presult2-presult1-1);
					printf("ESP8266��IP��ַ��%s\r\n",ip);  //������ʾIP��ַ
					return 0;   //��ȷ����0
				}
				else return 4;  //δ�յ�Ԥ������
			}
			else return 5;      //δ�յ�Ԥ������
		}		
	}		
}

/*-------------------------------------------------*/
/*����������⣬���͸��ģʽ���˳�͸��             */
/*��  ����timeout����ʱʱ�䣨100ms�ı�����         */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void ESP8266_Exit_Transfer(u16 timeout)
{
	u16 time=0;                                       //��ʱ������
	
	USART3_RX_STA=0;                                  //����3״̬��ʶ����                            
	memset(USART3_RX_BUF,0,USART3_RXBUFF_SIZE);       //��մ���3���ջ����� 
	u3_printf("AT\r\n");                              //����AT����ָ��
	
	time=0;                                           //��ʱ����������	
    while(time<timeout)                               //�ȴ���ʱ���
	{
		if(strstr( (const char *)USART3_RX_BUF , (const char *)"\r\nOK\r\n"))//����յ�OK����ʾ��������ģʽ
			break;       //����whileѭ��
		delay_ms(100);   //��ʱ100ms
		time++;          //��ʱ������+1   
	}
	if(time>=timeout)    //��ʱ����ʾ����͸��״̬
	{
		printf("ESP8266����͸��״̬��׼���˳�͸��\r\n");   //������ʾ��Ϣ
		u3_printf("+++");                                  //�����˳�͸��ָ���	
		delay_ms(1000);                                    //��ʱ1s
	}
}

/*-------------------------------------------------*/
/*������������STAģʽ��TCP������                   */
/*��  ������                                       */
/*����ֵ��0����ȷ   ����������                     */
/*-------------------------------------------------*/
u8 ESP8266_STA_TCPServer(void)
{
	if(ESP8266_SendCmd_OK("AT+CWMODE=1",50))return 1;                    //����8266ΪSTAģʽ����ʱʱ��5s��ʧ�ܷ���1
	else printf("ESP8266����STAģʽ�ɹ�\r\n");
 		
	if(ESP8266_SendCmd_OK("AT+CIPMODE=0",50))return 2;                   //�ر�͸������ʱʱ��5s��ʧ�ܷ���2
 	else printf("ESP8266�ر�͸���ɹ�\r\n");
	
	if(ESP8266_SendCmd_OK("AT+CIPMUX=1",50))return 3;                    //������·���ӣ���ʱʱ��5s��ʧ�ܷ���3
 	else printf("ESP8266������·���ӳɹ�\r\n");
	
	if(ESP8266_SendCmd_OK("AT+CIPSERVER=1,5050",50))return 4;            //����TCP���������˿�5050����ʱʱ��5s��ʧ�ܷ���4	
	 
	return 0;                                                            //�ɹ�����STA_TCP����������0
}

/*-------------------------------------------------*/
/*����������ȡ����״̬                             */
/*��  ������                                       */
/*����ֵ������״̬                                 */
/*        0����״̬                                */
/*        1���пͻ��˽���                          */
/*        2���пͻ��˶Ͽ�                          */
/*-------------------------------------------------*/
u8 ESP8266_Get_LinkSta(void)
{
	u8 id_temp[10]={0};    //�����������ID
	u8 sta_temp[10]={0};   //�����������״̬
	
	if(strstr( (const char *)USART3_RX_BUF , (const char *)"CONNECT"))//������ܵ�CONNECT��ʾ�пͻ�������
	{
		sscanf((const char *)USART3_RX_BUF,"%[^,],%[^,]",id_temp,sta_temp);
		printf("�пͻ��˽��룬ID=%s\r\n",id_temp);        //������ʾ��Ϣ
		USART3_RX_STA=0;                                  //����3״̬��ʶ����                            
		memset(USART3_RX_BUF,0,USART3_RXBUFF_SIZE);       //��մ���3���ջ�����	
		return 1;                                         //�пͻ��˽���
	}
	if(strstr( (const char *)USART3_RX_BUF , (const char *)"CLOSED"))//������ܵ�CLOSED��ʾ�����ӶϿ�
	{
		sscanf((const char *)USART3_RX_BUF,"%[^,],%[^,]",id_temp,sta_temp);
		printf("�пͻ��˶Ͽ���ID=%s\r\n",id_temp);        //������ʾ��Ϣ
		USART3_RX_STA=0;                                  //����3״̬��ʶ����                            
		memset(USART3_RX_BUF,0,USART3_RXBUFF_SIZE);       //��մ���3���ջ�����	
		return 2;                                         //�пͻ��˶Ͽ�
	}
    else return 0; //��״̬�ı�	
}
/*-------------------------------------------------*/
/*����������ȡ�ͻ�������                           */
/*        ����س����з�\r\n\r\n��Ϊ���ݵĽ�����   */
/*��  ����data�����ݻ�����                         */
/*��  ����len�� ������                             */
/*��  ����id��  �������ݵĿͻ��˵�����ID           */
/*����ֵ������״̬                                 */
/*        0��������                                */
/*        1��������                                */
/*-------------------------------------------------*/
u8 ESP8266_Get_Data(u8 *data, u8 *len, u8 *id)
{
	u8 temp[10]={0};      //��������
	char *presult;

	if(strstr( (const char *)USART3_RX_BUF , (const char *)"\r\n\r\n"))     //�������ŵĻس�������Ϊ���ݵĽ�����
	{
		sscanf((const char *)USART3_RX_BUF,"%[^,],%[^,],%[^:]",temp,id,len);//��ȡ�������ݣ���Ҫ��id�����ݳ���	
		presult = strstr( (const char *)USART3_RX_BUF , (const char *)":"); //����ð�š�ð�ź��������
		if( presult != NULL )                             //�ҵ�ð��
			sprintf((char *)data,"%s",(presult+1));       //ð�ź�����ݣ����Ƶ�data
		USART3_RX_STA=0;                                  //����3״̬��ʶ����                            
		memset(USART3_RX_BUF,0,USART3_RXBUFF_SIZE);       //��մ���3���ջ�����	
		return 1;                                         //�����ݵ���
	}
    else return 0;                                        //�����ݵ���
}

/*-------------------------------------------------*/
/*����������������������                           */
/*��  ����databuff�����ݻ�����<2048                */
/*��  ����data_len�����ݳ���                       */
/*��  ����id��      �ͻ��˵�����ID                 */
/*��  ����timeout�� ��ʱʱ�䣨10ms�ı�����         */
/*����ֵ������ֵ                                   */
/*        0���޴���                                */
/*        1���ȴ��������ݳ�ʱ                      */
/*        2�����ӶϿ���                            */
/*        3���������ݳ�ʱ                          */
/*-------------------------------------------------*/
u8 ESP8266_SendData(u8 id, u8 *databuff, u16 data_len, u16 timeout)
{
	u16 time=0;        //��ʱ������
	u16 i;
	
    USART3_RX_STA=0;                                  //����3״̬��ʶ����                            
	memset(USART3_RX_BUF,0,USART3_RXBUFF_SIZE);       //��մ���3���ջ�����		        
	u3_printf("AT+CIPSEND=%d,%d\r\n",id,data_len);    //���ͷ�������ָ��
	
	time=0;                                           //��ʱ����������	
    while(time<timeout)                               //�ȴ���ʱ���
	{
		if(strstr( (const char *)USART3_RX_BUF , (const char *)">"))//������ܵ�>��ʾָ��ɹ�,׼����������
			break;       //����whileѭ��
		delay_ms(10);    //��ʱ10ms
		time++;          //��ʱ������+1     
	}
	if(time>=timeout)return 1;     //��ʱ���󣬷���1
	else                           //û��ʱ����ȷ       
	{
		USART3_RX_STA=0;                                     //����3״̬��ʶ����                            
		memset(USART3_RX_BUF,0,USART3_RXBUFF_SIZE);          //��մ���3���ջ�����		
		for(i=0;i<data_len;i++)u3_printf("%c",databuff[i]);  //��������	
		
		time=0;                                              //��ʱ����������	
		while(time<timeout)                                  //�ȴ���ʱ���
		{
			if(strstr( (const char *)USART3_RX_BUF , (const char *)"SEND OK"))//�������SEND OK����ʾ���ͳɹ�
			{ 
				USART3_RX_STA=0;                                              //����3״̬��ʶ����                            
				memset(USART3_RX_BUF,0,USART3_RXBUFF_SIZE);                   //��մ���3���ջ�����		
				break;                                                        //����whileѭ��
			} 
			if(strstr( (const char *)USART3_RX_BUF , (const char *)"link is not valid"))//�������link is not valid����ʾ���ӶϿ�
			{
				USART3_RX_STA=0;                                              //����3״̬��ʶ����                            
				memset(USART3_RX_BUF,0,USART3_RXBUFF_SIZE);                   //��մ���3���ջ�����		
				return 2;                                                     //����2
			}
			delay_ms(10);              //��ʱ10ms
			time++;                    //��ʱ������+1 
	    }
		if(time>=timeout)return 3;     //��ʱ���󣬷���3
		else return 0;	               //��ȷ������0
	}	
}

/*-------------------------------------------------*/
/*������������TCP��������������͸��ģʽ            */
/*��  ����ip��������IP��ַ                         */
/*��  ����port���������˿ں�                       */
/*��  ����timeout�� ��ʱʱ�䣨100ms�ı�����        */
/*����ֵ��0����ȷ  ����������                      */
/*-------------------------------------------------*/
u8 ESP8266_Connect_Server(u8 *ip, u16 port,u16 timeout)
{
	u16 time=0;
	
	USART3_RX_STA=0;                                       //����3״̬��ʶ����                            
	memset(USART3_RX_BUF,0,USART3_RXBUFF_SIZE);            //��մ���3���ջ�����	  
	u3_printf("AT+CIPSTART=\"TCP\",\"%s\",%d\r\n",ip,port);//�������ӷ�����ָ��
	
	time=0;                                              //��ʱ����������	
	while(time<timeout)                                  //�ȴ���ʱ���
	{
		if(strstr( (const char *)USART3_RX_BUF , (const char *)"CONNECT"))//������ܵ�CONNECT��ʾ���ӳɹ�
			break;                                                        //����whileѭ��
		if(strstr( (const char *)USART3_RX_BUF , (const char *)"CLOSED")) //������ܵ�CLOSED��ʾ������δ����
			return 1;                                                     //������δ��������1
		if(strstr( (const char *)USART3_RX_BUF , (const char *)"ALREADY CONNECTED"))//������ܵ�ALREADY CONNECTED�Ѿ���������
			return 2;                                                               //�Ѿ��������ӷ���2
		delay_ms(100);  //��ʱ100ms
		time++;         //��ʱ������+1    
	}
	if(time>=timeout)return 3;                //��ʱ���󣬷���3
	else                                      //���ӳɹ���׼������͸��
	{
		printf("ESP8266���ӷ������ɹ���׼������͸��\r\n");    //������ʾ��Ϣ
		USART3_RX_STA=0;                                      //����3״̬��ʶ����                            
  	    memset(USART3_RX_BUF,0,USART3_RXBUFF_SIZE);           //��մ���3���ջ�����	  
		u3_printf("AT+CIPSEND\r\n");                          //���ͽ���͸��ָ��
		
		time=0;                                               //��ʱ����������	
		while(time<timeout)                                   //�ȴ���ʱ���
		{
			if(strstr( (const char *)USART3_RX_BUF , (const char *)"\r\nOK\r\n\r\n>"))//���������ʾ����͸���ɹ�
				break;       //����whileѭ��
			delay_ms(100);   //��ʱ100ms
			time++;          //��ʱ������+1     
		}
		if(time>=timeout)return 4;                //͸����ʱ���󣬷���4
		else return 0;	                          //�ɹ�����0		
	}
}

/*-------------------------------------------------*/
/*������������STAģʽ��TCP������                   */
/*��  ����ip��������IP��ַ                         */
/*��  ����port���������˿ں�                       */
/*����ֵ��0����ȷ  ����������                      */
/*-------------------------------------------------*/
u8 ESP8266_STA_TCPCLient(u8 *ip, u16 port)
{
	u8 res;
	 	
	if(ESP8266_SendCmd_OK("AT+CWMODE=1",50))return 1;          //����8266ΪSTAģʽ����ʱʱ��5s��ʧ�ܷ���1
	else printf("ESP8266����STAģʽ�ɹ�\r\n");                 //������ʾ��Ϣ
 		
	if(ESP8266_SendCmd_OK("AT+CIPMODE=1",50))return 2;         //����͸������ʱʱ��5s��ʧ�ܷ���2
 	else printf("ESP8266����͸���ɹ�\r\n");                    //������ʾ��Ϣ
	
	if(ESP8266_SendCmd_OK("AT+CIPMUX=0",50))return 3;          //�رն�·���ӣ���ʱʱ��5s��ʧ�ܷ���3
 	else printf("ESP8266�رն�·���ӳɹ�\r\n");                //������ʾ��Ϣ
	
	printf("ESP8266���ӷ�������... ...\r\n");                  //������ʾ��Ϣ
	res=ESP8266_Connect_Server(ip,port,50);                    //����TCP������,������͸������ʱʱ��5s��ʧ�ܷ���7	
	switch(res)
	{
		case 0 : break;        //0��ȷ      
		case 1 : return 4;     //������δ����  ����4
		case 2 : return 5;     //�Ѿ�����      ����5
		case 3 : return 6;     //���ӳ�ʱ��    ����6
		case 4 : return 7;     //����͸��ʧ��  ����7
	}
	
	return 0;                  //����0
}

/*-------------------------------------------------*/
/*�������������޶���UDPԶ������                    */
/*��  ����ip��UDPԶ������ IP��ַ                   */
/*��  ����port��UDPԶ������ �˿ں�                 */
/*��  ����local_port:�������� �˿ں�               */
/*��  ����timeout�� ��ʱʱ�䣨100ms�ı�����        */
/*����ֵ��0����ȷ  ����������                      */
/*-------------------------------------------------*/
u8 ESP8266_Set_UDPlimit(u8 *ip, u16 port, u16 local_port, u16 timeout)
{
	u16 time=0;
	
	USART3_RX_STA=0;                                                       //����3״̬��ʶ����                            
	memset(USART3_RX_BUF,0,USART3_RXBUFF_SIZE);                            //��մ���3���ջ����� 
	u3_printf("AT+CIPSTART=\"UDP\",\"%s\",%d,%d,0\r\n",ip,port,local_port);//����UDP��������
	
	time=0;                                              //��ʱ����������	
	while(time<timeout)                                  //�ȴ���ʱ���
	{
		if(strstr( (const char *)USART3_RX_BUF , (const char *)"OK"))//������ܵ�OK��ʾ�ɹ�
			break;       //����whileѭ��
		delay_ms(100);   //��ʱ100ms
		time++;          //��ʱ������+1       
	}
	if(time>=timeout)return 1;                //��ʱ���󣬷���1
	else                                      //û��ʱ��׼������͸��
	{
		printf("ESP8266����UDP�ɹ���׼������͸��\r\n");       //������ʾ��Ϣ
		USART3_RX_STA=0;                                      //����3״̬��ʶ����                            
	    memset(USART3_RX_BUF,0,USART3_RXBUFF_SIZE);           //��մ���3���ջ�����	  		
		u3_printf("AT+CIPSEND\r\n");                          //���ͽ���͸��ָ��
		
		time=0;                                              //��ʱ����������	
		while(time<timeout)                                  //�ȴ���ʱ���
		{
			if(strstr( (const char *)USART3_RX_BUF , (const char *)"\r\nOK\r\n\r\n>"))//���������ʾ����͸���ɹ�
				break;       //����whileѭ��
			delay_ms(100);   //��ʱ100ms
			time++;          //��ʱ������+1       
		}
		if(time>=timeout)return 2;                //͸����ʱ���󣬷���2
		else return 0;	                          //�ɹ�����0		
	}
}

/*-------------------------------------------------*/
/*������������STAģʽ��UDP�޶�͸��                 */
/*��  ����ip��UDPԶ������ IP��ַ                   */
/*��  ����port��UDPԶ������ �˿ں�                 */
/*����ֵ��0����ȷ  ����������                      */
/*-------------------------------------------------*/
u8 ESP8266_STA_UDPlimit_Init(u8 *ip, u16 port)
{
	u8 res;
	
	if(ESP8266_SendCmd_OK("AT+CWMODE=1",50))return 1;          //����8266ΪSTAģʽ����ʱʱ��5s��ʧ�ܷ���1
	else printf("ESP8266����STAģʽ�ɹ�\r\n");                 //������ʾ��Ϣ
	
	if(ESP8266_SendCmd_OK("AT+CIPMODE=1",50))return 2;         //����͸������ʱʱ��5s��ʧ�ܷ���1
 	else printf("ESP8266����͸���ɹ�\r\n");                    //������ʾ��Ϣ
	
	if(ESP8266_SendCmd_OK("AT+CIPMUX=0",50))return 3;          //�رն�·���ӣ���ʱʱ��5s��ʧ�ܷ���3
 	else printf("ESP8266�رն�·���ӳɹ�\r\n");                //������ʾ��Ϣ
	
	printf("ESP8266����UDP��... ...\r\n");                     //������ʾ��Ϣ
	res=ESP8266_Set_UDPlimit(ip,port,5050,500);                //����UDP,������͸������ʱʱ��5s
	switch(res)
	{
		case 0 : break;       //��ȷ
		case 1 : return 4;    //����UDPʧ�ܣ� ����4
		case 2 : return 5;    //����͸��ʧ�ܣ�����5
	}
	
	return 0;                 //��ȷ,����0
}

/*-------------------------------------------------*/
/*����������������ģʽ��UDP                        */
/*��  ����ip��UDPԶ������ IP��ַ                   */
/*��  ����port��UDPԶ������ �˿ں�                 */
/*��  ����local_port:�������� �˿ں�               */
/*��  ����timeout�� ��ʱʱ�䣨100ms�ı�����        */
/*����ֵ��0����ȷ  ����������                      */
/*-------------------------------------------------*/
u8 ESP8266_Set_UDPfreedom(u8 *ip, u16 port, u16 local_port, u16 timeout)
{
	u16 time=0;
	
	USART3_RX_STA=0;                                                       //����3״̬��ʶ����                            
	memset(USART3_RX_BUF,0,USART3_RXBUFF_SIZE);                            //��մ���3���ջ�����  
	u3_printf("AT+CIPSTART=\"UDP\",\"%s\",%d,%d,2\r\n",ip,port,local_port);//����UDP��������
	
	time=0;                                              //��ʱ����������	
	while(time<timeout)                                  //�ȴ���ʱ���
	{
		if(strstr( (const char *)USART3_RX_BUF , (const char *)"OK"))//������ܵ�OK��ʾ�ɹ�
			break;       //����whileѭ��
		delay_ms(100);   //��ʱ100ms
		time++;          //��ʱ������+1      
	}
	if(time>=timeout)return 1;                //��ʱ���󣬷���1
    else return 0;	                          //�ɹ�����0		
}

/*-------------------------------------------------*/
/*������������STAģʽ��UDP����ģʽ                 */
/*��  ����ip��UDPԶ������ IP��ַ                   */
/*��  ����port��UDPԶ������ �˿ں�                 */
/*����ֵ��0����ȷ  ����������                      */
/*-------------------------------------------------*/
u8 ESP8266_STA_UDPfreedom_Init(u8 *ip, u16 port)
{		
	if(ESP8266_SendCmd_OK("AT+CWMODE=1",50))return 1;          //����8266ΪSTAģʽ����ʱʱ��5s��ʧ�ܷ���1
	else printf("ESP8266����STAģʽ�ɹ�\r\n");                 //������ʾ��Ϣ
 		
	if(ESP8266_SendCmd_OK("AT+CIPMODE=0",50))return 2;         //�ر�͸������ʱʱ��5s��ʧ�ܷ���1
 	else printf("ESP8266�ر�͸���ɹ�\r\n");                    //������ʾ��Ϣ
	
	if(ESP8266_SendCmd_OK("AT+CIPMUX=0",50))return 3;          //�رն�·���ӣ���ʱʱ��5s��ʧ�ܷ���2
 	else printf("ESP8266�رն�·���ӳɹ�\r\n");                //������ʾ��Ϣ
	 
	printf("ESP8266����UDP��... ...\r\n");                     //������ʾ��Ϣ
	if(ESP8266_Set_UDPfreedom(ip,port,5050,500))return 4;      //����UDP,������͸������ʱʱ��5s��ʧ�ܷ���7	
		
	return 0;                                                  //��ȷ����0
}

/*-------------------------------------------------*/
/*��������UDP����ģʽ��ȡ�ͻ�������                */
/*��  ����data�����ݻ�����                         */
/*��  ����len�� ���ݴ�С                           */
/*����ֵ��0��������  1��������                     */
/*-------------------------------------------------*/
u8 ESP8266_UDPGet_Data(u8 *data, u8 *len)
{
	u8 temp[10]={0};      //��������
	char *presult;

	if(strstr( (const char *)USART3_RX_BUF , (const char *)"\r\n\r\n"))//�������ŵĻس�������Ϊ���ݵĽ�����
	{		
		sscanf((const char *)USART3_RX_BUF,"%[^,],%[^:]:",temp,len);//��ȡ��������	
		presult = strstr( (const char *)USART3_RX_BUF , (const char *)":"); //����ð�š�ð�ź��������
		if( presult != NULL )                             //�ҵ�ð��
			sprintf((char *)data,"%s",(presult+1));       //ð�ź�����ݣ����Ƶ�data
		USART3_RX_STA=0;                                  //����3״̬��ʶ����                            
		memset(USART3_RX_BUF,0,USART3_RXBUFF_SIZE);       //��մ���3���ջ�����	
		return 1;                                         //�����ݵ���
	} 
    else return 0; //�����ݵ���
}

/*-------------------------------------------------*/
/*��������UDP��������                              */
/*��  ����databuff�����ݻ�����<2048                */
/*��  ����data_len�����ݳ���                       */
/*��  ����timeout�� ��ʱʱ�䣨10ms�ı�����         */
/*����ֵ������ֵ                                   */
/*        0���޴���                                */
/*        1���ȴ��������ݳ�ʱ                      */
/*        2���������ݳ�ʱ                          */
/*-------------------------------------------------*/
u8 ESP8266_UDP_SendData(u8 *databuff, u16 data_len, u16 timeout)
{
	u16 time=0;
	u16 i;
	
    USART3_RX_STA=0;                                      //����3״̬��ʶ����                            
	memset(USART3_RX_BUF,0,USART3_RXBUFF_SIZE);           //��մ���3���ջ�����          
	u3_printf("AT+CIPSEND=%d\r\n",data_len);              //���ͷ�������ָ��
	
	time=0;                                               //��ʱ����������	
	while(time<timeout)                                   //�ȴ���ʱ���
	{
		if(strstr( (const char *)USART3_RX_BUF , (const char *)">"))//������ܵ�>��ʾָ��ɹ�,׼����������
			break;       //����whileѭ��
		delay_ms(10);    //��ʱ100ms
		time++;          //��ʱ������+1      
	}
	if(time>=timeout)return 1;                //��ʱ���󣬷���1
	else                                      //�޳�ʱ
	{
		USART3_RX_STA=0;                                      //����3״̬��ʶ����                            
	    memset(USART3_RX_BUF,0,USART3_RXBUFF_SIZE);           //��մ���3���ջ����� 	
		for(i=0;i<data_len;i++)u3_printf("%c",databuff[i]);   //��������	
		
		time=0;                                               //��ʱ����������	
		while(time<timeout)                                   //�ȴ���ʱ���
		{
			if(strstr( (const char *)USART3_RX_BUF , (const char *)"SEND OK"))//�������SEND OK����ʾ���ͳɹ�
			{
				USART3_RX_STA=0;                                      //����3״̬��ʶ����                            
				memset(USART3_RX_BUF,0,USART3_RXBUFF_SIZE);           //��մ���3���ջ�����	
				break;                                                //����while
			}
			delay_ms(10);   //��ʱ10ms
			time++;         //��ʱ������+1
	    }
		if(time>=timeout)return 2;                //��ʱ���󣬷���2
		else return 0;	                          //��ȷ����0
	}	
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
				 temp+=(char_data[2]-0x30)*10;
				 temp+=(char_data[3]-0x30)*1;
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

/*-------------------------------------------------*/
/*����������ȡYeelink�������п��ص�״̬            */
/*��  ����device_id���豸��                        */
/*��  ����sensors_id����������                     */
/*��  ����value������ֵ                            */
/*��  ����timeout�� ��ʱʱ�䣨10ms�ı�����         */
/*����ֵ��ת���������                             */
/*-------------------------------------------------*/

char timestamp[64]={0}; 
char timestampTmp[64]={0};
char valueTmp[64]={0};

u8 Yeelink_get(const char *device_id,const char *sensors_id, char *value, u16 timeout)
{
	u16 ret,time;
	char* presult;
	char remote_server[] = "api.yeelink.net";
	char str_tmp[128] = {0};
	static char http_request[1024] = {0};	//����Ϊ��̬��������ֹ��ջ���
	static char http_response[1024] = {0};	//����Ϊ��̬��������ֹ��ջ���

	sprintf(str_tmp,"/v1.0/device/%s/sensor/%s/datapoints",device_id,sensors_id);
	sprintf( http_request , "GET %s HTTP/1.1\r\n",str_tmp);
	sprintf( str_tmp , "Host:%s\r\n" , remote_server);
	strcat( http_request , str_tmp);
	sprintf( str_tmp , "U-ApiKey:%s\r\n" , API_KEY);
	strcat( http_request , str_tmp);
	strcat( http_request , "Accept: */*\r\n");
	strcat( http_request , "Content-Type: application/x-www-form-urlencoded\r\n");
	strcat( http_request , "Connection: close\r\n");
	strcat( http_request , "\r\n");
	
	u3_printf("%s",http_request);   //����Http���ĸ�������
	time=0;                         //��ʱ����������	
	while(time<timeout)             //�ȴ���ʱ���
	{
		if(USART3_RX_STA&0x8000)    //�ȴ����ݵ���
			break;                  //����whileѭ��
		delay_ms(10);               //��ʱ100ms
		time++;                     //��ʱ������+1       
	}
	if(time>=timeout)return 1;      //��ʱ���󣬷���1
	else                            //û�г�ʱ
	{
		ret=USART3_RX_STA&0x7fff;                    //��ȡ������
		memcpy(http_response,USART3_RX_BUF,ret);     //��������
		USART3_RX_STA=0;                             //����3״̬��ʶ����                            
		memset(USART3_RX_BUF,0,USART3_RXBUFF_SIZE);  //��մ���3���ջ�����	
		
		http_response[ret] = '\0';		             //���������
		presult = strstr( (const char *)http_response , (const char *)"200 OK\r\n");//�ж��Ƿ��յ�200 OK
		if( presult != NULL )   //�ҵ�200 OK
		{			
			static char strTmp[1024]={0};//����Ϊ��̬��������ֹ��ջ���		
			printf("HTTP OK!\r\n");
			sscanf(http_response,"%*[^{]{%[^}]",strTmp);
			sscanf(strTmp,"%[^,],%[^,]",timestampTmp,valueTmp);
			strncpy(timestamp,strstr(timestampTmp,":")+2,strlen(strstr(timestampTmp,":"))-3);
			strncpy(value,strstr(valueTmp,":")+1,strlen(strstr(valueTmp,":"))-1);
		}	
		else
		{
			printf("HTTP error!\r\n");
			printf("%s\r\n",http_response);
		}
	}
	return 0;    //��ȷ����0
}

/*-------------------------------------------------*/
/*��������Yeelink�ϴ�������������                  */
/*��  ����device_id���豸��                        */
/*��  ����sensors_id����������                     */
/*��  ����value������                              */
/*��  ����timeout�� ��ʱʱ�䣨10ms�ı�����         */
/*����ֵ��ת���������                             */
/*-------------------------------------------------*/
u8 Yeelink_post(const char *device_id, const char *sensors_id, char *value, u16 timeout)
{
	u16 ret,time;
	char* presult;
	char remote_server[] = "api.yeelink.net";
	char str_tmp[128] = {0};
	static char http_request[1024] = {0};	//����Ϊ��̬��������ֹ��ջ���
	static char http_response[1024] = {0};	//����Ϊ��̬��������ֹ��ջ���

	sprintf(str_tmp,"/v1.0/device/%s/sensor/%s/datapoints",device_id,sensors_id);
	sprintf( http_request , "POST %s HTTP/1.1\r\n",str_tmp);
	sprintf( str_tmp , "Host:%s\r\n" , remote_server);
	strcat( http_request , str_tmp);
	sprintf( str_tmp , "U-ApiKey:%s\r\n" , API_KEY);
	strcat( http_request , str_tmp);
	strcat( http_request , "Accept: */*\r\n");
	strcat( http_request , "Content-Length: 15\r\n");
	strcat( http_request , "Content-Type: application/x-www-form-urlencoded\r\n");
	strcat( http_request , "Connection: close\r\n");

    sprintf( str_tmp ,"\r\n%s\r\n" , value);//������ֵ
	strcat( http_request , str_tmp);
	strcat( http_request , "\r\n");

	u3_printf("%s",http_request);   //����Http���ĸ�������
	time=0;                         //��ʱ����������	
	while(time<timeout)             //�ȴ���ʱ���
	{
		if(USART3_RX_STA&0x8000)    //�ȴ����ݵ���
			break;                  //����whileѭ��
		delay_ms(10);               //��ʱ100ms
		time++;                     //��ʱ������+1       
	}
	if(time>=timeout)return 1;      //��ʱ���󣬷���1
	else                            //û�г�ʱ
	{
		ret=USART3_RX_STA&0x7fff;                    //��ȡ������
		memcpy(http_response,USART3_RX_BUF,ret);     //��������
		USART3_RX_STA=0;                             //����3״̬��ʶ����                            
		memset(USART3_RX_BUF,0,USART3_RXBUFF_SIZE);  //��մ���3���ջ�����	
		
		http_response[ret] = '\0';		             //���������
		presult = strstr( (const char *)http_response , (const char *)"200 OK\r\n");//�ж��Ƿ��յ�200 OK
		if( presult != NULL )
		{					
			printf("HTTP OK!\r\n");
		}	
		else
		{
			printf("HTTP error!\r\n");
			printf("%s\r\n",http_response);
		}
	}
	return 0;
}

/*-------------------------------------------------*/
/*����������ȡOnenet�������п��ص�״̬             */
/*��  ����device_id���豸��                        */
/*��  ����dataflow�� ������                        */
/*��  ����value������ֵ                            */
/*��  ����timeout�� ��ʱʱ�䣨10ms�ı�����         */
/*����ֵ��ת���������                             */
/*-------------------------------------------------*/
u8 OneNet_get(const char *device_id,const char *dataflow, u8 *value,u16 timeout)
{
	u16 ret,time;
	char* presult, *temp;
	char *v1;
	char remote_server[] = "api.heclouds.com";
	char str_tmp[128] = {0};
	static char http_request[1024] = {0};	//����Ϊ��̬��������ֹ��ջ���
	static char http_response[1024] = {0};	//����Ϊ��̬��������ֹ��ջ���

	memset(http_request,0,1024); //���
	memset(http_response,0,1024);//���

	sprintf(str_tmp,"/devices/%s/datapoints?datastream_id=%s",device_id,dataflow);
	// ȷ�� HTTP�����ײ�
	// ����POST /v1.0/device/98d19569e0474e9abf6f075b8b5876b9/1/1/datapoints/add HTTP/1.1\r\n
	sprintf( http_request , "GET %s HTTP/1.1\r\n",str_tmp);
	// �������� ���� Host: api.machtalk.net\r\n
	sprintf( str_tmp , "Host:%s\r\n" , remote_server);
	strcat( http_request , str_tmp);

	// �������� ���� APIKey: d8a605daa5f4c8a3ad086151686dce64
	sprintf( str_tmp , "api-key:%s\r\n" , API_KEY);//��Ҫ�滻Ϊ�Լ���APIKey
	strcat( http_request , str_tmp);
	strcat( http_request , "Connection: close\r\n\r\n");	
	
	u3_printf("%s",http_request);   //����Http���ĸ�������
	time=0;                         //��ʱ����������	
	while(time<timeout)             //�ȴ���ʱ���
	{
		if(USART3_RX_STA&0x8000)    //�ȴ����ݵ���
			break;                  //����whileѭ��
		delay_ms(10);               //��ʱ100ms
		time++;                     //��ʱ������+1       
	}
	if(time>=timeout)return 1;      //��ʱ���󣬷���1
	else                            //û�г�ʱ
	{
		ret=USART3_RX_STA&0x7fff;                    //��ȡ������
		memcpy(http_response,USART3_RX_BUF,ret);     //��������
		USART3_RX_STA=0;                             //����3״̬��ʶ����                            
		memset(USART3_RX_BUF,0,USART3_RXBUFF_SIZE);  //��մ���3���ջ�����	
		
		http_response[ret] = '\0';		             //���������
		presult = strstr( (const char *)http_response , (const char *)"200 OK\r\n");//�ж��Ƿ��յ�200 OK
		if( presult != NULL )   //�ҵ�200 OK
		{						
			printf("HTTP OK!\r\n");
			v1 = strstr( (const char *)http_response , (const char *)"\"value\":");
			if(v1 != NULL)
			{	
				temp=v1+8;             //��λ������״̬��ֵ
				if(*temp&0x01)*value=1; //���ش�״̬ 1
				else *value=0;          //���عر�״̬ 0
			} 
		}	
		else
		{
			printf("HTTP error!\r\n");
		}
	}
	return 0;
}

/*-------------------------------------------------*/
/*��������OneNet�ϴ�������������                   */
/*��  ����device_id���豸��                        */
/*��  ����num��Ҫ�ϴ������ݳ���                    */
/*��  ����value������                              */
/*��  ����timeout�� ��ʱʱ�䣨10ms�ı�����         */
/*����ֵ��ת���������                             */
/*-------------------------------------------------*/
u8 OneNet_post(const char *device_id,u8 num,u8 *value,u16 timeout)
{
	u16 ret,time;
	char* presult;
	char remote_server[] = "api.heclouds.com";
	char str_tmp[128] = {0};
	static char http_request[1024] = {0};	//����Ϊ��̬��������ֹ��ջ���
	static char http_response[1024] = {0};	//����Ϊ��̬��������ֹ��ջ���

	memset(http_request,0,1024);//���
	memset(http_response,0,1024);//���	

	sprintf(str_tmp,"/devices/%s/datapoints?type=3",device_id);
	// ȷ�� HTTP�����ײ�
	// ����POST /v1.0/device/98d19569e0474e9abf6f075b8b5876b9/1/1/datapoints/add HTTP/1.1\r\n
	sprintf( http_request , "POST %s HTTP/1.1\r\n",str_tmp);
	// �������� ���� Host: api.machtalk.net\r\n
	sprintf( str_tmp , "Host:%s\r\n" , remote_server);
	strcat( http_request , str_tmp);

	// �������� ���� APIKey: d8a605daa5f4c8a3ad086151686dce64
	sprintf( str_tmp , "api-key:%s\r\n" , API_KEY);//��Ҫ�滻Ϊ�Լ���APIKey
	strcat( http_request , str_tmp);
	
	sprintf( str_tmp , "Content-Length: %d\r\n" , num);//��Ҫ�滻Ϊ�Լ���APIKey
	strcat( http_request , str_tmp);

	strcat( http_request , "Connection: close\r\n");

    sprintf( str_tmp ,"\r\n%s\r\n" , value);//������ֵ
	strcat( http_request , str_tmp);	
	strcat( http_request , "\r\n");	
	
	u3_printf("%s",http_request);   //����Http���ĸ�������
	time=0;                         //��ʱ����������	
	while(time<timeout)             //�ȴ���ʱ���
	{
		if(USART3_RX_STA&0x8000)    //�ȴ����ݵ���
			break;                  //����whileѭ��
		delay_ms(10);               //��ʱ100ms
		time++;                     //��ʱ������+1       
	}
	if(time>=timeout)return 1;      //��ʱ���󣬷���1
	else                            //û�г�ʱ
	{
		ret=USART3_RX_STA&0x7fff;                    //��ȡ������
		memcpy(http_response,USART3_RX_BUF,ret);     //��������
		USART3_RX_STA=0;                             //����3״̬��ʶ����                            
		memset(USART3_RX_BUF,0,USART3_RXBUFF_SIZE);  //��մ���3���ջ�����	
		
		http_response[ret] = '\0';		             //���������
		presult = strstr( (const char *)http_response , (const char *)"200 OK\r\n");//�ж��Ƿ��յ�200 OK
		if( presult != NULL )   //�ҵ�200 OK
		{					
			printf("HTTP OK!\r\n");
		}	
		else
		{
			printf("HTTP error!\r\n");
			printf("%s\r\n",http_response);
		}
	}
	return 0;
}

/*-------------------------------------------------*/
/*�����������Ӱ������Ʒ�����                       */
/*��  ����device_id���豸��                        */
/*��  ����num��Ҫ�ϴ������ݳ���                    */
/*��  ����value������                              */
/*��  ����timeout�� ��ʱʱ�䣨10ms�ı�����         */
/*����ֵ��0����ȷ  ����������                      */
/*-------------------------------------------------*/
u8 ESP8266_Cloud_Server(void)
{	
	if(ESP8266_SendCmd_OK("AT+CLDSTART",50))return 1;          //��ʱʱ��5s��ʧ�ܷ���1
 	if(ESP8266_SendCmd_OK("AT+LANSTART",50))return 2;          //��ʱʱ��5s��ʧ�ܷ���2	
	return 0;                                                  //�ɹ�����0
}

/*-------------------------------------------------*/
/*����������ȡ�������Ʒ���������������             */
/*��  ����data�����ݻ�����                         */
/*��  ����len�����ݳ���                            */
/*����ֵ��0��������  ������                        */
/*-------------------------------------------------*/
u8 ESP8266_Cloud_GetData(u8 *data, u8 *len)
{
	u8 temp[10]={0};      //��������
	
    if(strstr( (const char *)USART3_RX_BUF , (const char *)"\r\n+CLD"))//������ܵ�+CLD��ʾ������
	{		
		delay_ms(5);
		sscanf((const char *)USART3_RX_BUF ,"%[^,],%[^:]:%s",temp,len,data);//��ȡ��������	
 		USART3_RX_STA=0;                                                    //����3״̬��ʶ����                            
	    memset(USART3_RX_BUF,0,USART3_RXBUFF_SIZE);                         //��մ���3���ջ�����
		return 1;  //�����ݵ���
	}
    else return 0; //�����ݵ���
}

/*-------------------------------------------------*/
/*�����������ſɷ�������������                   */
/*��  ����databuff�����ݻ�����<2048                */
/*��  ����data_len�����ݳ���                       */
/*��  ����timeout�� ��ʱʱ�䣨10ms�ı�����         */
/*����ֵ������ֵ                                   */
/*        0���޴���                                */
/*        1���ȴ��������ݳ�ʱ                      */
/*        2���������ݳ�ʱ                          */
/*-------------------------------------------------*/
u8 ESP8266_Cloud_SendData(u8 *databuff, u16 data_len, u16 timeout)
{
	u16 time=0;
	u16 i;
	
    USART3_RX_STA=0;                                      //����3״̬��ʶ����                            
	memset(USART3_RX_BUF,0,USART3_RXBUFF_SIZE);           //��մ���3���ջ�����          
	u3_printf("AT+CLDSEND=%d\r\n",data_len);              //���ͷ�������ָ��
	
	time=0;                                               //��ʱ����������	
	while(time<timeout)                                   //�ȴ���ʱ���
	{
		if(strstr( (const char *)USART3_RX_BUF , (const char *)">"))//������ܵ�>��ʾָ��ɹ�,׼����������
			break;       //����whileѭ��
		delay_ms(10);    //��ʱ100ms
		time++;          //��ʱ������+1      
	}
	if(time>=timeout)return 1;                //��ʱ���󣬷���1
	else                                      //�޳�ʱ
	{
		USART3_RX_STA=0;                                      //����3״̬��ʶ����                            
	    memset(USART3_RX_BUF,0,USART3_RXBUFF_SIZE);           //��մ���3���ջ����� 	
		for(i=0;i<data_len;i++)u3_printf("%c",databuff[i]);   //��������	
		
		time=0;                                               //��ʱ����������	
		while(time<timeout)                                   //�ȴ���ʱ���
		{
			if(strstr( (const char *)USART3_RX_BUF , (const char *)"SEND OK"))//�������SEND OK����ʾ���ͳɹ�
			{
				USART3_RX_STA=0;                                      //����3״̬��ʶ����                            
				memset(USART3_RX_BUF,0,USART3_RXBUFF_SIZE);           //��մ���3���ջ�����	
				break;                                                //����while
			}
			delay_ms(10);   //��ʱ10ms
			time++;         //��ʱ������+1
	    }
		if(time>=timeout)return 2;                //��ʱ���󣬷���2
		else return 0;	                          //��ȷ����0
	}	
}

/*-------------------------------------------------*/
/*���������ظ���λ��8·���ص�״̬                  */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
//����1------������               ����Ӧled_stalλ0
//����2------�̵���               ����Ӧled_stalλ1
//����3------LED_D8               ����Ӧled_stalλ2
//����4------LED_D9               ����Ӧled_stalλ3
//����5------LED_D10              ����Ӧled_stalλ4
//����6------3ɫLED----��ɫLED    ����Ӧled_stalλ5
//����7------3ɫLED----��ɫLED    ����Ӧled_stalλ6
//����8------3ɫLED----��ɫLED    ����Ӧled_stalλ7
void ESP8266_Switch8Sta(void)
{
	u8 led_stal;
	u8 sta_buff[10]="+SWITCH:";
	
	led_stal=0;
	
	if(BUZZEin)    led_stal|=0x01;     //�������ǿ��ģ�  λ0����1������Ϊ0
	if(!JDQ_IOin)  led_stal|=0x02;     //�̵����ǿ��ģ�  λ1����1������Ϊ0
	if(!LED_D8in)  led_stal|=0x04;     //LED_D8�ǿ��ģ�  λ2����1������Ϊ0
	if(!LED_D9in)  led_stal|=0x08;     //LED_D9�ǿ��ģ�  λ3����1������Ϊ0
	if(!LED_D10in) led_stal|=0x10;     //LED_D10�ǿ��ģ� λ4����1������Ϊ0
	if(!LED_Bin)   led_stal|=0x20;     //��ɫLED�ǿ��ģ� λ5����1������Ϊ0
	if(!LED_Rin)   led_stal|=0x40;     //��ɫLED�ǿ��ģ� λ6����1������Ϊ0
	if(!LED_Gin)   led_stal|=0x80;     //��ɫLED�ǿ��ģ� λ7����1������Ϊ0
	
	sta_buff[8]=led_stal;              //״ֵ̬��¼��������
	sta_buff[9]='\0';                  //���������
	
	u3_printf("%s",sta_buff);          //������λ��
}

/*-------------------------------------------------*/
/*���������ظ���λ���¶�ֵ�ɼ�                     */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void ESP8266_TempSent(void)
{
    u8  temperature;  	    
	u8  humidity; 
	char  temp_buff[8]="+TEMP:";
	
	DHT11_Read_Data(&temperature,&humidity);		         //��ȡ��ʪ��ֵ		
	printf("�¶�=%d��C   ",temperature);        //������ʾ��Ϣ
	sprintf(temp_buff,"+TEMP:%d",(const char *)temperature); //�����ϴ�����
		
	u3_printf("%s",temp_buff);                               //������λ��
}

/*-------------------------------------------------*/
/*���������ظ���λ��ʪ��ֵ�ɼ�                     */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void ESP8266_HumiSent(void)
{
    u8  temperature;  	    
	u8  humidity; 
	char  temp_buff[8]="+HUMI:";
	
	DHT11_Read_Data(&temperature,&humidity);		         //��ȡ��ʪ��ֵ		
	printf("ʪ��=%d %  ",humidity);                          //������ʾ��Ϣ
	sprintf(temp_buff,"+HUMI:%d",(const char *)humidity);    //�����ϴ�����
		
	u3_printf("%s",temp_buff);                               //������λ��
}

/*-------------------------------------------------*/
/*���������ظ���λ����ǿ�Ȳɼ�                     */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void ESP8266_LumiSent(void)
{
	u16 dis_data;
	double temp;
	char  temp_buff[]="+LUMI:";
	
	Start_BH1750();	              //��������ת��
	delay_ms(200);                //��ʱ200ms���ȴ�ת�����
	Read_BH1750();                //��ֵ
	dis_data=BUF[0];  
	dis_data=(dis_data<<8)+BUF[1];//�ϳ����ݣ�����������
	temp=(dis_data)/1.2;
	
	printf("����ǿ��=%-7.2f lx\r\n",temp);   //�������
	sprintf(temp_buff,"+LUMI:%-7.2f",temp);    //�����ϴ�����
		
	u3_printf("%s",temp_buff);                               //������λ��
}
/*-------------------------------------------------*/
/*���������ظ�APP 2·���ص�״̬                  */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
//����1------������               ����Ӧled_stalλ0
//����2------�̵���               ����Ӧled_stalλ1
void ESP8266_Switch2Sta(void)
{
	if(BUZZEin)    u3_printf("+SWITCH1:1");
	else           u3_printf("+SWITCH1:0");
	delay_ms(25);
	
	if(!JDQ_IOin)  u3_printf("+SWITCH2:1");
	else           u3_printf("+SWITCH2:0");	
	delay_ms(25);	
}

/*-------------------------------------------------*/
/*���������ظ�APP���2·����ֵ                     */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void ESP8266_APPdata2Sent(void)
{
    u8     temperature;  	    
	u8     humidity; 
	char   temp_buff[15];
	
	DHT11_Read_Data(&temperature,&humidity);		            //��ȡ��ʪ��ֵ			
	
	sprintf(temp_buff,"+CHANNEL1:%d",(const char *)temperature); //�����ϴ�����
	u3_printf("%s",temp_buff);                                   //������λ��	
	delay_ms(25);
	
	sprintf(temp_buff,"+CHANNEL2:%d",(const char *)humidity);    //�����ϴ�����
	u3_printf("%s",temp_buff);                                   //������λ��	
	delay_ms(25);	
}
/*-------------------------------------------------*/
/*������������APģʽ                               */
/*��  ������                                       */
/*����ֵ��0����ȷ   ����������                     */
/*-------------------------------------------------*/
u8 ESP8266_APmode(void)
{	
	printf("\r\n2����ʱ��׼�������λ\r\n");        //������ʾ��Ϣ
	delay_ms(2000);                                 //��ʱ2s
	ESP8266_Exit_Transfer(10);                      //����Ƿ���͸��״̬����������˳�͸��״̬	
	
	if(ESP8266_SoftReset(50))return 1;                         //�����λ8266����ʱʱ��5s��ʧ�ܾͷ���1
	else printf("ESP8266��λ�ɹ�\r\n");
 	
	if(ESP8266_SendCmd_OK("AT+CWMODE=2",50))return 2;          //����8266ΪAPģʽ����ʱʱ��5s��ʧ�ܷ���2
	else printf("ESP8266����APģʽ�ɹ�\r\n");
 	
	printf("ESP8266׼������AP����\r\n");
	printf("SSID��STM32103+ESP8266\r\n");
	printf("���룺012345678\r\n");
	printf("���ܷ�ʽ��WPA_WPA2_PSK\r\n");
	if(ESP8266_SET_AP(SSID,PASS,WPA_WPA2_PSK,50))return 3;     //����8266��AP����ʱʱ��5s��ʧ�ܷ���3
	else printf("ESP8266����AP�����ɹ�\r\n");
		
	if(ESP8266_SendCmd_OK("AT+CIPMODE=1",50))return 4;         //����͸������ʱʱ��5s��ʧ�ܷ���4
 	else printf("ESP8266����͸���ɹ�\r\n");
	
	if(ESP8266_SendCmd_OK("AT+CIPMUX=0",50))return 5;          //�رն�·���ӣ���ʱʱ��5s��ʧ�ܷ���5
 	else printf("ESP8266�رն�·���ӳɹ�\r\n");
	
	printf("����APģʽIP��ַ��192.168.10.1\r\n");
	if(ESP8266_SendCmd_OK("AT+CIPAP=\"192.168.10.1\"",50))return 6; //����AP��IP��ַ����ʱʱ��5s��ʧ�ܷ���6
 	else printf("ESP8266����APģʽIP�ɹ�\r\n");
	
	return 0;
}
/*-------------------------------------------------*/
/*������������AP����                               */         
/*��  ����ssid��ssid                               */
/*��  ����password������                           */
/*��  ����mode:���ܷ�ʽ                            */
/*��  ����timeout ����ʱʱ��                       */
/*����ֵ��0����ȷ  ����������                      */
/*-------------------------------------------------*/
u8 ESP8266_SET_AP(u8 *ssid, u8 *password, u8 mode, u16 timeout)
{
	u16 time=0;
	
	USART3_RX_STA=0;                                                  //����3״̬��ʶ����                            
	memset(USART3_RX_BUF,0,USART3_RXBUFF_SIZE);                       //��մ���3���ջ�����	  
	u3_printf("AT+CWSAP=\"%s\",\"%s\",4,%d\r\n",ssid,password,mode);  //��������APָ��
	
	time=0;
	while(time<timeout)
	{
		if(strstr( (const char *)USART3_RX_BUF , (const char *)"OK"))//������ܵ�OK��ʾָ��ɹ�
			break;
		delay_ms(100);//��ʱ100ms
		time++;      
	}
	if(time>=timeout)return 1;                //��ʱ���󣬷���1
	else return 0;		                      //�޴�����0
}
