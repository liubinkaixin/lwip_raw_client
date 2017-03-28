/*-------------------------------------------------*/
/*         超纬电子STM32F407ZGT6开发板             */
/*    淘宝地址： http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/
/*-------------------------------------------------*/
/*                ESP8266源文件                    */
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

u8 Esp8266_Buff[BUFF_LEN];      //数据缓冲区
u8 IP_Buff[20];                 //存放8266的IP
u8 Transmission_Sta;            //传输状态，1：指令状态    0：数据状态

/*-------------------------------------------------*/
/*函数名：ESP8266智能连接函数                      */
/*参  数：timeout：超时时间（100ms的倍数）         */
/*返回值：0：正确   其他：错误                     */
/*-------------------------------------------------*/
u8 ESP8266_SmartLink(u16 timeout)
{
	u16  time=0;                                              //超时计数器
	
	if(ESP8266_SoftReset(50))return 1;                        //软件复位8266，超时时间5s，失败就返回1
	else printf("软复位成功,等待10秒后,进入智能连接\r\n");    //串口显示复位成功
	
 	delay_ms(10000);                                          //延时10s
	
	if(ESP8266_SendCmd_OK("AT+CWSMARTSTART=1",50))return 2;   //开启8266智能连接，超时时间5s，失败返回2
	else printf("设置智能连接成功，请使用APP软件配置\r\n");   //串口显示信息
 
	USART3_RX_STA=0;                            //串口3状态标识清零                            
	memset(USART3_RX_BUF,0,USART3_RXBUFF_SIZE); //清空串口3接收缓冲区	
	time=0;                                     //超时计数器清零
	while(time<timeout)                         //等待超时与否
	{
		if(strstr( (const char *)USART3_RX_BUF , (const char *)"SMART SUCCESS"))//如果串口接受到SMART SUCCESS表示SmartLink成功
			break;       //跳出while循环
		delay_ms(100);   //延时100ms
		time++;          //超时计数器+1   
	}	
	if(time>=timeout)return 3;  //SmartLink超时错误，返回3
	else                                      
	{
		return 0;              //正确返回0
	}
}

/*-------------------------------------------------*/
/*函数名：8266发送指令，期待的回答是OK             */
/*参  数：cmd：指令                                */
/*参  数：timeout：超时时间（100ms的倍数）         */
/*返回值：0：正确   其他：错误                     */
/*-------------------------------------------------*/
u8 ESP8266_SendCmd_OK(u8 *cmd, u16 timeout)
{
    u16 time;                                   //超时计数器
	
	USART3_RX_STA=0;                            //串口3状态标识清零                            
	memset(USART3_RX_BUF,0,USART3_RXBUFF_SIZE); //清空串口3接收缓冲区
	u3_printf("%s\r\n",cmd);                    //发送指令
	time=0;                                     //超时计数器清零
	
	while(time<timeout)                         //等待超时与否
	{
		if(strstr( (const char *)USART3_RX_BUF , (const char *)"\r\nOK\r\n"))//如果接受到OK表示指令成功
			break;       //跳出while循环
		delay_ms(100);   //延时100ms
		time++;          //超时计数器+1
	}
	if(time>=timeout)return 1;   //超时错误，返回1
	else return 0;		         //无错，返回0
}

/*-------------------------------------------------*/
/*函数名：ESP8266软件复位                          */
/*参  数：timeout：超时时间（100ms的倍数）         */
/*返回值：0：正确   其他：错误                     */
/*-------------------------------------------------*/
u8 ESP8266_SoftReset(u16 timeout)
{
    u16 time;                                   //超时计数器
	
	USART3_RX_STA=0;                            //串口3状态标识清零                            
	memset(USART3_RX_BUF,0,USART3_RXBUFF_SIZE); //清空串口3接收缓冲区                       
	u3_printf("AT+RST\r\n");                    //发送软件复位指令
	time=0;                                     //超时计数器清零
	
	while(time<timeout)                         //等待超时与否
	{
		if(strstr( (const char *)USART3_RX_BUF , (const char *)"Ai-Thinker Technology"))//如果接受到Ai-Thinker Technology表示复位成功
			break;       //跳出while循环
		delay_ms(100);   //延时100ms
		time++;          //超时计数器+1   
	}
	if(time>=timeout)return 1;       //超时错误，返回1
	else return 0;	                 //无错，返回0
}

/*-------------------------------------------------*/
/*函数名：等待连接wifi，获取IP地址                 */
/*参  数：ip：保存IP的数组                         */
/*参  数：timeout：超时时间（100ms的倍数）         */
/*返回值：0：正确   其他：错误                     */
/*-------------------------------------------------*/
u8 ESP8266_GetIP(u8 *ip, u16 timeout)
{
	u16  time=0;                //超时计数器
	char *presult1,*presult2;
	
	printf("\r\n2秒延时后准备软件复位\r\n");        //串口显示信息
	delay_ms(2000);                                 //延时2s
	ESP8266_Exit_Transfer(10);                      //检测是否在透传状态，如果是则退出透传状态	
	if(ESP8266_SoftReset(50))return 1;              //软件复位8266，超时时间5s，失败就返回1
	else printf("软复位成功,等待连接上wifi\r\n");   //串口显示信息
	
	time=0;                                //超时计数器清零	
	while(time<timeout)                    //等待超时与否
	{
		if(strstr( (const char *)USART3_RX_BUF , (const char *)"WIFI GOT IP"))//等待接收WIFI GOT IP
			break;       //跳出while循环
		delay_ms(100);   //延时100ms
		time++;          //超时计数器+1         
	}
	if(time>=timeout)return 2; //超时错误，没有得到IP返回2
	else                       //连接上了wifi，得到了IP，准备发送查询IP指令      
	{
		printf("连接上wifi，查询模块得到的IP地址\r\n");   //串口显示信息
		USART3_RX_STA=0;                                  //串口3状态标识清零                            
		memset(USART3_RX_BUF,0,USART3_RXBUFF_SIZE);       //清空串口3接收缓冲区 	  
		u3_printf("AT+CIFSR\r\n");                        //发送查询IP指令
		time=0;                                           //超时计数器清零	
		while(time<timeout)                               //等待超时与否
		{
			if(strstr( (const char *)USART3_RX_BUF , (const char *)"\r\nOK\r\n"))//如果接受到OK表示指令成功
				break;       //跳出while循环
			delay_ms(100);   //延时100ms
			time++;          //超时计数器+1     
		}
		if(time>=timeout)return 3;   //查询IP超时错误，返回3
		else                         //正确   
		{
			presult1 = strstr( (const char *)USART3_RX_BUF , (const char *)"\"");
			if( presult1 != NULL )
			{
				presult2 = strstr( (const char *)presult1+1 ,    (const char *)"\"");
				if( presult2 != NULL )
				{
					memcpy(ip,presult1+1,presult2-presult1-1);
					printf("ESP8266的IP地址：%s\r\n",ip);  //串口显示IP地址
					return 0;   //正确返回0
				}
				else return 4;  //未收到预期数据
			}
			else return 5;      //未收到预期数据
		}		
	}		
}

/*-------------------------------------------------*/
/*函数名：检测，如果透传模式则退出透传             */
/*参  数：timeout：超时时间（100ms的倍数）         */
/*返回值：无                                       */
/*-------------------------------------------------*/
void ESP8266_Exit_Transfer(u16 timeout)
{
	u16 time=0;                                       //超时计数器
	
	USART3_RX_STA=0;                                  //串口3状态标识清零                            
	memset(USART3_RX_BUF,0,USART3_RXBUFF_SIZE);       //清空串口3接收缓冲区 
	u3_printf("AT\r\n");                              //发送AT测试指令
	
	time=0;                                           //超时计数器清零	
    while(time<timeout)                               //等待超时与否
	{
		if(strstr( (const char *)USART3_RX_BUF , (const char *)"\r\nOK\r\n"))//如果收到OK，表示处于正常模式
			break;       //跳出while循环
		delay_ms(100);   //延时100ms
		time++;          //超时计数器+1   
	}
	if(time>=timeout)    //超时，表示处于透传状态
	{
		printf("ESP8266处于透传状态，准备退出透传\r\n");   //串口显示信息
		u3_printf("+++");                                  //发送退出透传指令包	
		delay_ms(1000);                                    //延时1s
	}
}

/*-------------------------------------------------*/
/*函数名：设置STA模式下TCP服务器                   */
/*参  数：无                                       */
/*返回值：0：正确   其他：错误                     */
/*-------------------------------------------------*/
u8 ESP8266_STA_TCPServer(void)
{
	if(ESP8266_SendCmd_OK("AT+CWMODE=1",50))return 1;                    //设置8266为STA模式，超时时间5s，失败返回1
	else printf("ESP8266设置STA模式成功\r\n");
 		
	if(ESP8266_SendCmd_OK("AT+CIPMODE=0",50))return 2;                   //关闭透传，超时时间5s，失败返回2
 	else printf("ESP8266关闭透传成功\r\n");
	
	if(ESP8266_SendCmd_OK("AT+CIPMUX=1",50))return 3;                    //开启多路连接，超时时间5s，失败返回3
 	else printf("ESP8266开启多路连接成功\r\n");
	
	if(ESP8266_SendCmd_OK("AT+CIPSERVER=1,5050",50))return 4;            //开启TCP服务器，端口5050，超时时间5s，失败返回4	
	 
	return 0;                                                            //成功设置STA_TCP服务器返回0
}

/*-------------------------------------------------*/
/*函数名：获取连接状态                             */
/*参  数：无                                       */
/*返回值：连接状态                                 */
/*        0：无状态                                */
/*        1：有客户端接入                          */
/*        2：有客户端断开                          */
/*-------------------------------------------------*/
u8 ESP8266_Get_LinkSta(void)
{
	u8 id_temp[10]={0};    //缓冲区，存放ID
	u8 sta_temp[10]={0};   //缓冲区，存放状态
	
	if(strstr( (const char *)USART3_RX_BUF , (const char *)"CONNECT"))//如果接受到CONNECT表示有客户端连接
	{
		sscanf((const char *)USART3_RX_BUF,"%[^,],%[^,]",id_temp,sta_temp);
		printf("有客户端接入，ID=%s\r\n",id_temp);        //串口显示信息
		USART3_RX_STA=0;                                  //串口3状态标识清零                            
		memset(USART3_RX_BUF,0,USART3_RXBUFF_SIZE);       //清空串口3接收缓冲区	
		return 1;                                         //有客户端接入
	}
	if(strstr( (const char *)USART3_RX_BUF , (const char *)"CLOSED"))//如果接受到CLOSED表示有链接断开
	{
		sscanf((const char *)USART3_RX_BUF,"%[^,],%[^,]",id_temp,sta_temp);
		printf("有客户端断开，ID=%s\r\n",id_temp);        //串口显示信息
		USART3_RX_STA=0;                                  //串口3状态标识清零                            
		memset(USART3_RX_BUF,0,USART3_RXBUFF_SIZE);       //清空串口3接收缓冲区	
		return 2;                                         //有客户端断开
	}
    else return 0; //无状态改变	
}
/*-------------------------------------------------*/
/*函数名：获取客户端数据                           */
/*        两组回车换行符\r\n\r\n作为数据的结束符   */
/*参  数：data：数据缓冲区                         */
/*参  数：len： 数据量                             */
/*参  数：id：  发来数据的客户端的连接ID           */
/*返回值：数据状态                                 */
/*        0：无数据                                */
/*        1：有数据                                */
/*-------------------------------------------------*/
u8 ESP8266_Get_Data(u8 *data, u8 *len, u8 *id)
{
	u8 temp[10]={0};      //缓冲区，
	char *presult;

	if(strstr( (const char *)USART3_RX_BUF , (const char *)"\r\n\r\n"))     //两个连着的回车换行作为数据的结束符
	{
		sscanf((const char *)USART3_RX_BUF,"%[^,],%[^,],%[^:]",temp,id,len);//截取各段数据，主要是id和数据长度	
		presult = strstr( (const char *)USART3_RX_BUF , (const char *)":"); //查找冒号。冒号后的是数据
		if( presult != NULL )                             //找到冒号
			sprintf((char *)data,"%s",(presult+1));       //冒号后的数据，复制到data
		USART3_RX_STA=0;                                  //串口3状态标识清零                            
		memset(USART3_RX_BUF,0,USART3_RXBUFF_SIZE);       //清空串口3接收缓冲区	
		return 1;                                         //有数据到来
	}
    else return 0;                                        //无数据到来
}

/*-------------------------------------------------*/
/*函数名：服务器发送数据                           */
/*参  数：databuff：数据缓冲区<2048                */
/*参  数：data_len：数据长度                       */
/*参  数：id：      客户端的连接ID                 */
/*参  数：timeout： 超时时间（10ms的倍数）         */
/*返回值：错误值                                   */
/*        0：无错误                                */
/*        1：等待发送数据超时                      */
/*        2：连接断开了                            */
/*        3：发送数据超时                          */
/*-------------------------------------------------*/
u8 ESP8266_SendData(u8 id, u8 *databuff, u16 data_len, u16 timeout)
{
	u16 time=0;        //超时计数器
	u16 i;
	
    USART3_RX_STA=0;                                  //串口3状态标识清零                            
	memset(USART3_RX_BUF,0,USART3_RXBUFF_SIZE);       //清空串口3接收缓冲区		        
	u3_printf("AT+CIPSEND=%d,%d\r\n",id,data_len);    //发送发送数据指令
	
	time=0;                                           //超时计数器清零	
    while(time<timeout)                               //等待超时与否
	{
		if(strstr( (const char *)USART3_RX_BUF , (const char *)">"))//如果接受到>表示指令成功,准备发送数据
			break;       //跳出while循环
		delay_ms(10);    //延时10ms
		time++;          //超时计数器+1     
	}
	if(time>=timeout)return 1;     //超时错误，返回1
	else                           //没超时，正确       
	{
		USART3_RX_STA=0;                                     //串口3状态标识清零                            
		memset(USART3_RX_BUF,0,USART3_RXBUFF_SIZE);          //清空串口3接收缓冲区		
		for(i=0;i<data_len;i++)u3_printf("%c",databuff[i]);  //发送数据	
		
		time=0;                                              //超时计数器清零	
		while(time<timeout)                                  //等待超时与否
		{
			if(strstr( (const char *)USART3_RX_BUF , (const char *)"SEND OK"))//如果接受SEND OK，表示发送成功
			{ 
				USART3_RX_STA=0;                                              //串口3状态标识清零                            
				memset(USART3_RX_BUF,0,USART3_RXBUFF_SIZE);                   //清空串口3接收缓冲区		
				break;                                                        //跳出while循环
			} 
			if(strstr( (const char *)USART3_RX_BUF , (const char *)"link is not valid"))//如果接受link is not valid，表示连接断开
			{
				USART3_RX_STA=0;                                              //串口3状态标识清零                            
				memset(USART3_RX_BUF,0,USART3_RXBUFF_SIZE);                   //清空串口3接收缓冲区		
				return 2;                                                     //返回2
			}
			delay_ms(10);              //延时10ms
			time++;                    //超时计数器+1 
	    }
		if(time>=timeout)return 3;     //超时错误，返回3
		else return 0;	               //正确，返回0
	}	
}

/*-------------------------------------------------*/
/*函数名：连接TCP服务器，并进入透传模式            */
/*参  数：ip：服务器IP地址                         */
/*参  数：port：服务器端口号                       */
/*参  数：timeout： 超时时间（100ms的倍数）        */
/*返回值：0：正确  其他：错误                      */
/*-------------------------------------------------*/
u8 ESP8266_Connect_Server(u8 *ip, u16 port,u16 timeout)
{
	u16 time=0;
	
	USART3_RX_STA=0;                                       //串口3状态标识清零                            
	memset(USART3_RX_BUF,0,USART3_RXBUFF_SIZE);            //清空串口3接收缓冲区	  
	u3_printf("AT+CIPSTART=\"TCP\",\"%s\",%d\r\n",ip,port);//发送连接服务器指令
	
	time=0;                                              //超时计数器清零	
	while(time<timeout)                                  //等待超时与否
	{
		if(strstr( (const char *)USART3_RX_BUF , (const char *)"CONNECT"))//如果接受到CONNECT表示连接成功
			break;                                                        //跳出while循环
		if(strstr( (const char *)USART3_RX_BUF , (const char *)"CLOSED")) //如果接受到CLOSED表示服务器未开启
			return 1;                                                     //服务器未开启返回1
		if(strstr( (const char *)USART3_RX_BUF , (const char *)"ALREADY CONNECTED"))//如果接受到ALREADY CONNECTED已经建立连接
			return 2;                                                               //已经建立连接返回2
		delay_ms(100);  //延时100ms
		time++;         //超时计数器+1    
	}
	if(time>=timeout)return 3;                //超时错误，返回3
	else                                      //连接成功，准备进入透传
	{
		printf("ESP8266连接服务器成功，准备进入透传\r\n");    //串口显示信息
		USART3_RX_STA=0;                                      //串口3状态标识清零                            
  	    memset(USART3_RX_BUF,0,USART3_RXBUFF_SIZE);           //清空串口3接收缓冲区	  
		u3_printf("AT+CIPSEND\r\n");                          //发送进入透传指令
		
		time=0;                                               //超时计数器清零	
		while(time<timeout)                                   //等待超时与否
		{
			if(strstr( (const char *)USART3_RX_BUF , (const char *)"\r\nOK\r\n\r\n>"))//如果成立表示进入透传成功
				break;       //跳出while循环
			delay_ms(100);   //延时100ms
			time++;          //超时计数器+1     
		}
		if(time>=timeout)return 4;                //透传超时错误，返回4
		else return 0;	                          //成功返回0		
	}
}

/*-------------------------------------------------*/
/*函数名：设置STA模式下TCP服务器                   */
/*参  数：ip：服务器IP地址                         */
/*参  数：port：服务器端口号                       */
/*返回值：0：正确  其他：错误                      */
/*-------------------------------------------------*/
u8 ESP8266_STA_TCPCLient(u8 *ip, u16 port)
{
	u8 res;
	 	
	if(ESP8266_SendCmd_OK("AT+CWMODE=1",50))return 1;          //设置8266为STA模式，超时时间5s，失败返回1
	else printf("ESP8266设置STA模式成功\r\n");                 //串口显示信息
 		
	if(ESP8266_SendCmd_OK("AT+CIPMODE=1",50))return 2;         //开启透传，超时时间5s，失败返回2
 	else printf("ESP8266开启透传成功\r\n");                    //串口显示信息
	
	if(ESP8266_SendCmd_OK("AT+CIPMUX=0",50))return 3;          //关闭多路连接，超时时间5s，失败返回3
 	else printf("ESP8266关闭多路连接成功\r\n");                //串口显示信息
	
	printf("ESP8266连接服务器中... ...\r\n");                  //串口显示信息
	res=ESP8266_Connect_Server(ip,port,50);                    //连接TCP服务器,并进入透传，超时时间5s，失败返回7	
	switch(res)
	{
		case 0 : break;        //0正确      
		case 1 : return 4;     //服务器未开启  返回4
		case 2 : return 5;     //已经连接      返回5
		case 3 : return 6;     //连接超时启    返回6
		case 4 : return 7;     //设置透传失败  返回7
	}
	
	return 0;                  //返回0
}

/*-------------------------------------------------*/
/*函数名：设置限定的UDP远程主机                    */
/*参  数：ip：UDP远程主机 IP地址                   */
/*参  数：port：UDP远程主机 端口号                 */
/*参  数：local_port:本地主机 端口号               */
/*参  数：timeout： 超时时间（100ms的倍数）        */
/*返回值：0：正确  其他：错误                      */
/*-------------------------------------------------*/
u8 ESP8266_Set_UDPlimit(u8 *ip, u16 port, u16 local_port, u16 timeout)
{
	u16 time=0;
	
	USART3_RX_STA=0;                                                       //串口3状态标识清零                            
	memset(USART3_RX_BUF,0,USART3_RXBUFF_SIZE);                            //清空串口3接收缓冲区 
	u3_printf("AT+CIPSTART=\"UDP\",\"%s\",%d,%d,0\r\n",ip,port,local_port);//发送UDP设置命令
	
	time=0;                                              //超时计数器清零	
	while(time<timeout)                                  //等待超时与否
	{
		if(strstr( (const char *)USART3_RX_BUF , (const char *)"OK"))//如果接受到OK表示成功
			break;       //跳出while循环
		delay_ms(100);   //延时100ms
		time++;          //超时计数器+1       
	}
	if(time>=timeout)return 1;                //超时错误，返回1
	else                                      //没超时，准备进入透传
	{
		printf("ESP8266设置UDP成功，准备进入透传\r\n");       //串口显示信息
		USART3_RX_STA=0;                                      //串口3状态标识清零                            
	    memset(USART3_RX_BUF,0,USART3_RXBUFF_SIZE);           //清空串口3接收缓冲区	  		
		u3_printf("AT+CIPSEND\r\n");                          //发送进入透传指令
		
		time=0;                                              //超时计数器清零	
		while(time<timeout)                                  //等待超时与否
		{
			if(strstr( (const char *)USART3_RX_BUF , (const char *)"\r\nOK\r\n\r\n>"))//如果成立表示进入透传成功
				break;       //跳出while循环
			delay_ms(100);   //延时100ms
			time++;          //超时计数器+1       
		}
		if(time>=timeout)return 2;                //透传超时错误，返回2
		else return 0;	                          //成功返回0		
	}
}

/*-------------------------------------------------*/
/*函数名：设置STA模式下UDP限定透传                 */
/*参  数：ip：UDP远程主机 IP地址                   */
/*参  数：port：UDP远程主机 端口号                 */
/*返回值：0：正确  其他：错误                      */
/*-------------------------------------------------*/
u8 ESP8266_STA_UDPlimit_Init(u8 *ip, u16 port)
{
	u8 res;
	
	if(ESP8266_SendCmd_OK("AT+CWMODE=1",50))return 1;          //设置8266为STA模式，超时时间5s，失败返回1
	else printf("ESP8266设置STA模式成功\r\n");                 //串口显示信息
	
	if(ESP8266_SendCmd_OK("AT+CIPMODE=1",50))return 2;         //开启透传，超时时间5s，失败返回1
 	else printf("ESP8266开启透传成功\r\n");                    //串口显示信息
	
	if(ESP8266_SendCmd_OK("AT+CIPMUX=0",50))return 3;          //关闭多路连接，超时时间5s，失败返回3
 	else printf("ESP8266关闭多路连接成功\r\n");                //串口显示信息
	
	printf("ESP8266设置UDP中... ...\r\n");                     //串口显示信息
	res=ESP8266_Set_UDPlimit(ip,port,5050,500);                //设置UDP,并进入透传，超时时间5s
	switch(res)
	{
		case 0 : break;       //正确
		case 1 : return 4;    //设置UDP失败， 返回4
		case 2 : return 5;    //进入透传失败，返回5
	}
	
	return 0;                 //正确,返回0
}

/*-------------------------------------------------*/
/*函数名：设置自由模式的UDP                        */
/*参  数：ip：UDP远程主机 IP地址                   */
/*参  数：port：UDP远程主机 端口号                 */
/*参  数：local_port:本地主机 端口号               */
/*参  数：timeout： 超时时间（100ms的倍数）        */
/*返回值：0：正确  其他：错误                      */
/*-------------------------------------------------*/
u8 ESP8266_Set_UDPfreedom(u8 *ip, u16 port, u16 local_port, u16 timeout)
{
	u16 time=0;
	
	USART3_RX_STA=0;                                                       //串口3状态标识清零                            
	memset(USART3_RX_BUF,0,USART3_RXBUFF_SIZE);                            //清空串口3接收缓冲区  
	u3_printf("AT+CIPSTART=\"UDP\",\"%s\",%d,%d,2\r\n",ip,port,local_port);//发送UDP设置命令
	
	time=0;                                              //超时计数器清零	
	while(time<timeout)                                  //等待超时与否
	{
		if(strstr( (const char *)USART3_RX_BUF , (const char *)"OK"))//如果接受到OK表示成功
			break;       //跳出while循环
		delay_ms(100);   //延时100ms
		time++;          //超时计数器+1      
	}
	if(time>=timeout)return 1;                //超时错误，返回1
    else return 0;	                          //成功返回0		
}

/*-------------------------------------------------*/
/*函数名：设置STA模式下UDP自由模式                 */
/*参  数：ip：UDP远程主机 IP地址                   */
/*参  数：port：UDP远程主机 端口号                 */
/*返回值：0：正确  其他：错误                      */
/*-------------------------------------------------*/
u8 ESP8266_STA_UDPfreedom_Init(u8 *ip, u16 port)
{		
	if(ESP8266_SendCmd_OK("AT+CWMODE=1",50))return 1;          //设置8266为STA模式，超时时间5s，失败返回1
	else printf("ESP8266设置STA模式成功\r\n");                 //串口显示信息
 		
	if(ESP8266_SendCmd_OK("AT+CIPMODE=0",50))return 2;         //关闭透传，超时时间5s，失败返回1
 	else printf("ESP8266关闭透传成功\r\n");                    //串口显示信息
	
	if(ESP8266_SendCmd_OK("AT+CIPMUX=0",50))return 3;          //关闭多路连接，超时时间5s，失败返回2
 	else printf("ESP8266关闭多路连接成功\r\n");                //串口显示信息
	 
	printf("ESP8266设置UDP中... ...\r\n");                     //串口显示信息
	if(ESP8266_Set_UDPfreedom(ip,port,5050,500))return 4;      //设置UDP,并进入透传，超时时间5s，失败返回7	
		
	return 0;                                                  //正确返回0
}

/*-------------------------------------------------*/
/*函数名：UDP自由模式获取客户端数据                */
/*参  数：data：数据缓冲区                         */
/*参  数：len： 数据大小                           */
/*返回值：0：无数据  1：有数据                     */
/*-------------------------------------------------*/
u8 ESP8266_UDPGet_Data(u8 *data, u8 *len)
{
	u8 temp[10]={0};      //缓冲区，
	char *presult;

	if(strstr( (const char *)USART3_RX_BUF , (const char *)"\r\n\r\n"))//两个连着的回车换行作为数据的结束符
	{		
		sscanf((const char *)USART3_RX_BUF,"%[^,],%[^:]:",temp,len);//截取各段数据	
		presult = strstr( (const char *)USART3_RX_BUF , (const char *)":"); //查找冒号。冒号后的是数据
		if( presult != NULL )                             //找到冒号
			sprintf((char *)data,"%s",(presult+1));       //冒号后的数据，复制到data
		USART3_RX_STA=0;                                  //串口3状态标识清零                            
		memset(USART3_RX_BUF,0,USART3_RXBUFF_SIZE);       //清空串口3接收缓冲区	
		return 1;                                         //有数据到来
	} 
    else return 0; //无数据到来
}

/*-------------------------------------------------*/
/*函数名：UDP发送数据                              */
/*参  数：databuff：数据缓冲区<2048                */
/*参  数：data_len：数据长度                       */
/*参  数：timeout： 超时时间（10ms的倍数）         */
/*返回值：错误值                                   */
/*        0：无错误                                */
/*        1：等待发送数据超时                      */
/*        2：发送数据超时                          */
/*-------------------------------------------------*/
u8 ESP8266_UDP_SendData(u8 *databuff, u16 data_len, u16 timeout)
{
	u16 time=0;
	u16 i;
	
    USART3_RX_STA=0;                                      //串口3状态标识清零                            
	memset(USART3_RX_BUF,0,USART3_RXBUFF_SIZE);           //清空串口3接收缓冲区          
	u3_printf("AT+CIPSEND=%d\r\n",data_len);              //发送发送数据指令
	
	time=0;                                               //超时计数器清零	
	while(time<timeout)                                   //等待超时与否
	{
		if(strstr( (const char *)USART3_RX_BUF , (const char *)">"))//如果接受到>表示指令成功,准备发送数据
			break;       //跳出while循环
		delay_ms(10);    //延时100ms
		time++;          //超时计数器+1      
	}
	if(time>=timeout)return 1;                //超时错误，返回1
	else                                      //无超时
	{
		USART3_RX_STA=0;                                      //串口3状态标识清零                            
	    memset(USART3_RX_BUF,0,USART3_RXBUFF_SIZE);           //清空串口3接收缓冲区 	
		for(i=0;i<data_len;i++)u3_printf("%c",databuff[i]);   //发送数据	
		
		time=0;                                               //超时计数器清零	
		while(time<timeout)                                   //等待超时与否
		{
			if(strstr( (const char *)USART3_RX_BUF , (const char *)"SEND OK"))//如果接受SEND OK，表示发送成功
			{
				USART3_RX_STA=0;                                      //串口3状态标识清零                            
				memset(USART3_RX_BUF,0,USART3_RXBUFF_SIZE);           //清空串口3接收缓冲区	
				break;                                                //跳出while
			}
			delay_ms(10);   //延时10ms
			time++;         //超时计数器+1
	    }
		if(time>=timeout)return 2;                //超时错误，返回2
		else return 0;	                          //正确返回0
	}	
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
/*函数名：获取Yeelink服务器中开关的状态            */
/*参  数：device_id：设备号                        */
/*参  数：sensors_id：传感器号                     */
/*参  数：value：开关值                            */
/*参  数：timeout： 超时时间（10ms的倍数）         */
/*返回值：转换后的数字                             */
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
	static char http_request[1024] = {0};	//声明为静态变量，防止堆栈溢出
	static char http_response[1024] = {0};	//声明为静态变量，防止堆栈溢出

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
	
	u3_printf("%s",http_request);   //发送Http报文给服务器
	time=0;                         //超时计数器清零	
	while(time<timeout)             //等待超时与否
	{
		if(USART3_RX_STA&0x8000)    //等待数据到来
			break;                  //跳出while循环
		delay_ms(10);               //延时100ms
		time++;                     //超时计数器+1       
	}
	if(time>=timeout)return 1;      //超时错误，返回1
	else                            //没有超时
	{
		ret=USART3_RX_STA&0x7fff;                    //获取数据量
		memcpy(http_response,USART3_RX_BUF,ret);     //拷贝数据
		USART3_RX_STA=0;                             //串口3状态标识清零                            
		memset(USART3_RX_BUF,0,USART3_RXBUFF_SIZE);  //清空串口3接收缓冲区	
		
		http_response[ret] = '\0';		             //加入结束符
		presult = strstr( (const char *)http_response , (const char *)"200 OK\r\n");//判断是否收到200 OK
		if( presult != NULL )   //找到200 OK
		{			
			static char strTmp[1024]={0};//声明为静态变量，防止堆栈溢出		
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
	return 0;    //正确返回0
}

/*-------------------------------------------------*/
/*函数名：Yeelink上传传感器的数据                  */
/*参  数：device_id：设备号                        */
/*参  数：sensors_id：传感器号                     */
/*参  数：value：数据                              */
/*参  数：timeout： 超时时间（10ms的倍数）         */
/*返回值：转换后的数字                             */
/*-------------------------------------------------*/
u8 Yeelink_post(const char *device_id, const char *sensors_id, char *value, u16 timeout)
{
	u16 ret,time;
	char* presult;
	char remote_server[] = "api.yeelink.net";
	char str_tmp[128] = {0};
	static char http_request[1024] = {0};	//声明为静态变量，防止堆栈溢出
	static char http_response[1024] = {0};	//声明为静态变量，防止堆栈溢出

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

    sprintf( str_tmp ,"\r\n%s\r\n" , value);//加入数值
	strcat( http_request , str_tmp);
	strcat( http_request , "\r\n");

	u3_printf("%s",http_request);   //发送Http报文给服务器
	time=0;                         //超时计数器清零	
	while(time<timeout)             //等待超时与否
	{
		if(USART3_RX_STA&0x8000)    //等待数据到来
			break;                  //跳出while循环
		delay_ms(10);               //延时100ms
		time++;                     //超时计数器+1       
	}
	if(time>=timeout)return 1;      //超时错误，返回1
	else                            //没有超时
	{
		ret=USART3_RX_STA&0x7fff;                    //获取数据量
		memcpy(http_response,USART3_RX_BUF,ret);     //拷贝数据
		USART3_RX_STA=0;                             //串口3状态标识清零                            
		memset(USART3_RX_BUF,0,USART3_RXBUFF_SIZE);  //清空串口3接收缓冲区	
		
		http_response[ret] = '\0';		             //加入结束符
		presult = strstr( (const char *)http_response , (const char *)"200 OK\r\n");//判断是否收到200 OK
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
/*函数名：获取Onenet服务器中开关的状态             */
/*参  数：device_id：设备号                        */
/*参  数：dataflow： 数据流                        */
/*参  数：value：开关值                            */
/*参  数：timeout： 超时时间（10ms的倍数）         */
/*返回值：转换后的数字                             */
/*-------------------------------------------------*/
u8 OneNet_get(const char *device_id,const char *dataflow, u8 *value,u16 timeout)
{
	u16 ret,time;
	char* presult, *temp;
	char *v1;
	char remote_server[] = "api.heclouds.com";
	char str_tmp[128] = {0};
	static char http_request[1024] = {0};	//声明为静态变量，防止堆栈溢出
	static char http_response[1024] = {0};	//声明为静态变量，防止堆栈溢出

	memset(http_request,0,1024); //清空
	memset(http_response,0,1024);//清空

	sprintf(str_tmp,"/devices/%s/datapoints?datastream_id=%s",device_id,dataflow);
	// 确定 HTTP请求首部
	// 例如POST /v1.0/device/98d19569e0474e9abf6f075b8b5876b9/1/1/datapoints/add HTTP/1.1\r\n
	sprintf( http_request , "GET %s HTTP/1.1\r\n",str_tmp);
	// 增加属性 例如 Host: api.machtalk.net\r\n
	sprintf( str_tmp , "Host:%s\r\n" , remote_server);
	strcat( http_request , str_tmp);

	// 增加密码 例如 APIKey: d8a605daa5f4c8a3ad086151686dce64
	sprintf( str_tmp , "api-key:%s\r\n" , API_KEY);//需要替换为自己的APIKey
	strcat( http_request , str_tmp);
	strcat( http_request , "Connection: close\r\n\r\n");	
	
	u3_printf("%s",http_request);   //发送Http报文给服务器
	time=0;                         //超时计数器清零	
	while(time<timeout)             //等待超时与否
	{
		if(USART3_RX_STA&0x8000)    //等待数据到来
			break;                  //跳出while循环
		delay_ms(10);               //延时100ms
		time++;                     //超时计数器+1       
	}
	if(time>=timeout)return 1;      //超时错误，返回1
	else                            //没有超时
	{
		ret=USART3_RX_STA&0x7fff;                    //获取数据量
		memcpy(http_response,USART3_RX_BUF,ret);     //拷贝数据
		USART3_RX_STA=0;                             //串口3状态标识清零                            
		memset(USART3_RX_BUF,0,USART3_RXBUFF_SIZE);  //清空串口3接收缓冲区	
		
		http_response[ret] = '\0';		             //加入结束符
		presult = strstr( (const char *)http_response , (const char *)"200 OK\r\n");//判断是否收到200 OK
		if( presult != NULL )   //找到200 OK
		{						
			printf("HTTP OK!\r\n");
			v1 = strstr( (const char *)http_response , (const char *)"\"value\":");
			if(v1 != NULL)
			{	
				temp=v1+8;             //定位到开关状态的值
				if(*temp&0x01)*value=1; //开关打开状态 1
				else *value=0;          //开关关闭状态 0
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
/*函数名：OneNet上传传感器的数据                   */
/*参  数：device_id：设备号                        */
/*参  数：num：要上传的数据长度                    */
/*参  数：value：数据                              */
/*参  数：timeout： 超时时间（10ms的倍数）         */
/*返回值：转换后的数字                             */
/*-------------------------------------------------*/
u8 OneNet_post(const char *device_id,u8 num,u8 *value,u16 timeout)
{
	u16 ret,time;
	char* presult;
	char remote_server[] = "api.heclouds.com";
	char str_tmp[128] = {0};
	static char http_request[1024] = {0};	//声明为静态变量，防止堆栈溢出
	static char http_response[1024] = {0};	//声明为静态变量，防止堆栈溢出

	memset(http_request,0,1024);//清空
	memset(http_response,0,1024);//清空	

	sprintf(str_tmp,"/devices/%s/datapoints?type=3",device_id);
	// 确定 HTTP请求首部
	// 例如POST /v1.0/device/98d19569e0474e9abf6f075b8b5876b9/1/1/datapoints/add HTTP/1.1\r\n
	sprintf( http_request , "POST %s HTTP/1.1\r\n",str_tmp);
	// 增加属性 例如 Host: api.machtalk.net\r\n
	sprintf( str_tmp , "Host:%s\r\n" , remote_server);
	strcat( http_request , str_tmp);

	// 增加密码 例如 APIKey: d8a605daa5f4c8a3ad086151686dce64
	sprintf( str_tmp , "api-key:%s\r\n" , API_KEY);//需要替换为自己的APIKey
	strcat( http_request , str_tmp);
	
	sprintf( str_tmp , "Content-Length: %d\r\n" , num);//需要替换为自己的APIKey
	strcat( http_request , str_tmp);

	strcat( http_request , "Connection: close\r\n");

    sprintf( str_tmp ,"\r\n%s\r\n" , value);//加入数值
	strcat( http_request , str_tmp);	
	strcat( http_request , "\r\n");	
	
	u3_printf("%s",http_request);   //发送Http报文给服务器
	time=0;                         //超时计数器清零	
	while(time<timeout)             //等待超时与否
	{
		if(USART3_RX_STA&0x8000)    //等待数据到来
			break;                  //跳出while循环
		delay_ms(10);               //延时100ms
		time++;                     //超时计数器+1       
	}
	if(time>=timeout)return 1;      //超时错误，返回1
	else                            //没有超时
	{
		ret=USART3_RX_STA&0x7fff;                    //获取数据量
		memcpy(http_response,USART3_RX_BUF,ret);     //拷贝数据
		USART3_RX_STA=0;                             //串口3状态标识清零                            
		memset(USART3_RX_BUF,0,USART3_RXBUFF_SIZE);  //清空串口3接收缓冲区	
		
		http_response[ret] = '\0';		             //加入结束符
		presult = strstr( (const char *)http_response , (const char *)"200 OK\r\n");//判断是否收到200 OK
		if( presult != NULL )   //找到200 OK
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
/*函数名：连接安可信云服务器                       */
/*参  数：device_id：设备号                        */
/*参  数：num：要上传的数据长度                    */
/*参  数：value：数据                              */
/*参  数：timeout： 超时时间（10ms的倍数）         */
/*返回值：0：正确  其他：错误                      */
/*-------------------------------------------------*/
u8 ESP8266_Cloud_Server(void)
{	
	if(ESP8266_SendCmd_OK("AT+CLDSTART",50))return 1;          //超时时间5s，失败返回1
 	if(ESP8266_SendCmd_OK("AT+LANSTART",50))return 2;          //超时时间5s，失败返回2	
	return 0;                                                  //成功返回0
}

/*-------------------------------------------------*/
/*函数名：获取安可信云服务器发来的数据             */
/*参  数：data：数据缓冲区                         */
/*参  数：len：数据长度                            */
/*返回值：0：无数据  有数据                        */
/*-------------------------------------------------*/
u8 ESP8266_Cloud_GetData(u8 *data, u8 *len)
{
	u8 temp[10]={0};      //缓冲区，
	
    if(strstr( (const char *)USART3_RX_BUF , (const char *)"\r\n+CLD"))//如果接受到+CLD表示有数据
	{		
		delay_ms(5);
		sscanf((const char *)USART3_RX_BUF ,"%[^,],%[^:]:%s",temp,len,data);//截取各段数据	
 		USART3_RX_STA=0;                                                    //串口3状态标识清零                            
	    memset(USART3_RX_BUF,0,USART3_RXBUFF_SIZE);                         //清空串口3接收缓冲区
		return 1;  //有数据到来
	}
    else return 0; //无数据到来
}

/*-------------------------------------------------*/
/*函数名：向安信可服务器发送数据                   */
/*参  数：databuff：数据缓冲区<2048                */
/*参  数：data_len：数据长度                       */
/*参  数：timeout： 超时时间（10ms的倍数）         */
/*返回值：错误值                                   */
/*        0：无错误                                */
/*        1：等待发送数据超时                      */
/*        2：发送数据超时                          */
/*-------------------------------------------------*/
u8 ESP8266_Cloud_SendData(u8 *databuff, u16 data_len, u16 timeout)
{
	u16 time=0;
	u16 i;
	
    USART3_RX_STA=0;                                      //串口3状态标识清零                            
	memset(USART3_RX_BUF,0,USART3_RXBUFF_SIZE);           //清空串口3接收缓冲区          
	u3_printf("AT+CLDSEND=%d\r\n",data_len);              //发送发送数据指令
	
	time=0;                                               //超时计数器清零	
	while(time<timeout)                                   //等待超时与否
	{
		if(strstr( (const char *)USART3_RX_BUF , (const char *)">"))//如果接受到>表示指令成功,准备发送数据
			break;       //跳出while循环
		delay_ms(10);    //延时100ms
		time++;          //超时计数器+1      
	}
	if(time>=timeout)return 1;                //超时错误，返回1
	else                                      //无超时
	{
		USART3_RX_STA=0;                                      //串口3状态标识清零                            
	    memset(USART3_RX_BUF,0,USART3_RXBUFF_SIZE);           //清空串口3接收缓冲区 	
		for(i=0;i<data_len;i++)u3_printf("%c",databuff[i]);   //发送数据	
		
		time=0;                                               //超时计数器清零	
		while(time<timeout)                                   //等待超时与否
		{
			if(strstr( (const char *)USART3_RX_BUF , (const char *)"SEND OK"))//如果接受SEND OK，表示发送成功
			{
				USART3_RX_STA=0;                                      //串口3状态标识清零                            
				memset(USART3_RX_BUF,0,USART3_RXBUFF_SIZE);           //清空串口3接收缓冲区	
				break;                                                //跳出while
			}
			delay_ms(10);   //延时10ms
			time++;         //超时计数器+1
	    }
		if(time>=timeout)return 2;                //超时错误，返回2
		else return 0;	                          //正确返回0
	}	
}

/*-------------------------------------------------*/
/*函数名：回复上位机8路开关的状态                  */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
//开关1------蜂鸣器               ：对应led_stal位0
//开关2------继电器               ：对应led_stal位1
//开关3------LED_D8               ：对应led_stal位2
//开关4------LED_D9               ：对应led_stal位3
//开关5------LED_D10              ：对应led_stal位4
//开关6------3色LED----蓝色LED    ：对应led_stal位5
//开关7------3色LED----红色LED    ：对应led_stal位6
//开关8------3色LED----绿色LED    ：对应led_stal位7
void ESP8266_Switch8Sta(void)
{
	u8 led_stal;
	u8 sta_buff[10]="+SWITCH:";
	
	led_stal=0;
	
	if(BUZZEin)    led_stal|=0x01;     //蜂鸣器是开的，  位0：置1，否则为0
	if(!JDQ_IOin)  led_stal|=0x02;     //继电器是开的，  位1：置1，否则为0
	if(!LED_D8in)  led_stal|=0x04;     //LED_D8是开的，  位2：置1，否则为0
	if(!LED_D9in)  led_stal|=0x08;     //LED_D9是开的，  位3：置1，否则为0
	if(!LED_D10in) led_stal|=0x10;     //LED_D10是开的， 位4：置1，否则为0
	if(!LED_Bin)   led_stal|=0x20;     //蓝色LED是开的， 位5：置1，否则为0
	if(!LED_Rin)   led_stal|=0x40;     //红色LED是开的， 位6：置1，否则为0
	if(!LED_Gin)   led_stal|=0x80;     //绿色LED是开的， 位7：置1，否则为0
	
	sta_buff[8]=led_stal;              //状态值记录到缓冲区
	sta_buff[9]='\0';                  //加入结束符
	
	u3_printf("%s",sta_buff);          //发给上位机
}

/*-------------------------------------------------*/
/*函数名：回复上位机温度值采集                     */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void ESP8266_TempSent(void)
{
    u8  temperature;  	    
	u8  humidity; 
	char  temp_buff[8]="+TEMP:";
	
	DHT11_Read_Data(&temperature,&humidity);		         //读取温湿度值		
	printf("温度=%d°C   ",temperature);        //串口显示信息
	sprintf(temp_buff,"+TEMP:%d",(const char *)temperature); //构建上传数据
		
	u3_printf("%s",temp_buff);                               //发给上位机
}

/*-------------------------------------------------*/
/*函数名：回复上位机湿度值采集                     */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void ESP8266_HumiSent(void)
{
    u8  temperature;  	    
	u8  humidity; 
	char  temp_buff[8]="+HUMI:";
	
	DHT11_Read_Data(&temperature,&humidity);		         //读取温湿度值		
	printf("湿度=%d %  ",humidity);                          //串口显示信息
	sprintf(temp_buff,"+HUMI:%d",(const char *)humidity);    //构建上传数据
		
	u3_printf("%s",temp_buff);                               //发给上位机
}

/*-------------------------------------------------*/
/*函数名：回复上位机光强度采集                     */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void ESP8266_LumiSent(void)
{
	u16 dis_data;
	double temp;
	char  temp_buff[]="+LUMI:";
	
	Start_BH1750();	              //开启数据转换
	delay_ms(200);                //延时200ms，等待转换完毕
	Read_BH1750();                //读值
	dis_data=BUF[0];  
	dis_data=(dis_data<<8)+BUF[1];//合成数据，即光照数据
	temp=(dis_data)/1.2;
	
	printf("光照强度=%-7.2f lx\r\n",temp);   //串口输出
	sprintf(temp_buff,"+LUMI:%-7.2f",temp);    //构建上传数据
		
	u3_printf("%s",temp_buff);                               //发给上位机
}
/*-------------------------------------------------*/
/*函数名：回复APP 2路开关的状态                  */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
//开关1------蜂鸣器               ：对应led_stal位0
//开关2------继电器               ：对应led_stal位1
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
/*函数名：回复APP软件2路数据值                     */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void ESP8266_APPdata2Sent(void)
{
    u8     temperature;  	    
	u8     humidity; 
	char   temp_buff[15];
	
	DHT11_Read_Data(&temperature,&humidity);		            //读取温湿度值			
	
	sprintf(temp_buff,"+CHANNEL1:%d",(const char *)temperature); //构建上传数据
	u3_printf("%s",temp_buff);                                   //发给上位机	
	delay_ms(25);
	
	sprintf(temp_buff,"+CHANNEL2:%d",(const char *)humidity);    //构建上传数据
	u3_printf("%s",temp_buff);                                   //发给上位机	
	delay_ms(25);	
}
/*-------------------------------------------------*/
/*函数名：设置AP模式                               */
/*参  数：无                                       */
/*返回值：0：正确   其他：错误                     */
/*-------------------------------------------------*/
u8 ESP8266_APmode(void)
{	
	printf("\r\n2秒延时后准备软件复位\r\n");        //串口显示信息
	delay_ms(2000);                                 //延时2s
	ESP8266_Exit_Transfer(10);                      //检测是否在透传状态，如果是则退出透传状态	
	
	if(ESP8266_SoftReset(50))return 1;                         //软件复位8266，超时时间5s，失败就返回1
	else printf("ESP8266软复位成功\r\n");
 	
	if(ESP8266_SendCmd_OK("AT+CWMODE=2",50))return 2;          //设置8266为AP模式，超时时间5s，失败返回2
	else printf("ESP8266设置AP模式成功\r\n");
 	
	printf("ESP8266准备设置AP参数\r\n");
	printf("SSID：STM32103+ESP8266\r\n");
	printf("密码：012345678\r\n");
	printf("加密方式：WPA_WPA2_PSK\r\n");
	if(ESP8266_SET_AP(SSID,PASS,WPA_WPA2_PSK,50))return 3;     //设置8266的AP，超时时间5s，失败返回3
	else printf("ESP8266设置AP参数成功\r\n");
		
	if(ESP8266_SendCmd_OK("AT+CIPMODE=1",50))return 4;         //开启透传，超时时间5s，失败返回4
 	else printf("ESP8266开启透传成功\r\n");
	
	if(ESP8266_SendCmd_OK("AT+CIPMUX=0",50))return 5;          //关闭多路连接，超时时间5s，失败返回5
 	else printf("ESP8266关闭多路连接成功\r\n");
	
	printf("设置AP模式IP地址：192.168.10.1\r\n");
	if(ESP8266_SendCmd_OK("AT+CIPAP=\"192.168.10.1\"",50))return 6; //设置AP的IP地址，超时时间5s，失败返回6
 	else printf("ESP8266设置AP模式IP成功\r\n");
	
	return 0;
}
/*-------------------------------------------------*/
/*函数名：设置AP参数                               */         
/*参  数：ssid：ssid                               */
/*参  数：password：密码                           */
/*参  数：mode:加密方式                            */
/*参  数：timeout ：超时时间                       */
/*返回值：0；正确  其他：错误                      */
/*-------------------------------------------------*/
u8 ESP8266_SET_AP(u8 *ssid, u8 *password, u8 mode, u16 timeout)
{
	u16 time=0;
	
	USART3_RX_STA=0;                                                  //串口3状态标识清零                            
	memset(USART3_RX_BUF,0,USART3_RXBUFF_SIZE);                       //清空串口3接收缓冲区	  
	u3_printf("AT+CWSAP=\"%s\",\"%s\",4,%d\r\n",ssid,password,mode);  //发送设置AP指令
	
	time=0;
	while(time<timeout)
	{
		if(strstr( (const char *)USART3_RX_BUF , (const char *)"OK"))//如果接受到OK表示指令成功
			break;
		delay_ms(100);//延时100ms
		time++;      
	}
	if(time>=timeout)return 1;                //超时错误，返回1
	else return 0;		                      //无错，返回0
}
