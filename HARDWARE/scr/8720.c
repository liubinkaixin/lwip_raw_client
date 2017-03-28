/*-------------------------------------------------*/
/*         助开发网STM32F407ZGT6开发板             */
/*    淘宝地址： https://shop68304486.taobao.com   */
/*-------------------------------------------------*/
/*-------------------------------------------------*/
/*                  8720源文件                     */
/*-------------------------------------------------*/
/*-------------------------------------------------*/

#include "8720.h"
#include "usart1.h"
#include "tcpclient.h" 
#include "main.h"
#include "CriticalSeg.h"
#include "lwip/stats.h"

__8720ip_dev   lan8720_ip;     //8720芯片IP信息控制结构体
u8  MACaddr[6]={0x00,0x23,0x24,0x01,0x66,0x81};  //MAC地址，具有唯一性
u32 LWipTime=0;                //LWip周期时钟
u8  dhcpstatus=0;              //DHCP状态

struct netif lan8720_netif;     //定义一个全局的网络接口，注册网卡函数要用到  
__IO uint32_t TCPTimer = 0;     //TCP查询计时器 
__IO uint32_t ARPTimer = 0;     //ARP查询计时器
__IO uint32_t CLIENTTimer = 0;  //客户端发送计时器

#if LWIP_DHCP                   //如果开启DHCP
u32 DHCPfineTimer=0;	        //DHCP精细处理计时器
u32 DHCPcoarseTimer=0;	        //DHCP粗糙处理计时器
#endif

/*-------------------------------------------------*/
/*函数名：初始化lwip启动前的事宜                   */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void MylwIP_Init(void)
{
  struct ip_addr ipaddr;     //定义IP地址结构体
  struct ip_addr netmask;    //定义子网掩码结构体
  struct ip_addr gw;         //定义网关结构体

  mem_init();                //初始化动态内存堆
  memp_init();               //初始化内存池
  lwip_init();               //初始化LWIP内核
	
#if LWIP_DHCP                //如果开启DHCP，自动获取IP
  ipaddr.addr = 0;
  netmask.addr = 0;
  gw.addr = 0;                                   
#endif

  ETH_MACAddressConfig(ETH_MAC_Address0, MACaddr);  //配置MAC地址

  //注册网卡函数   ethernetif_init函数，需要自己实现，用于配置netif相关字段，以及初始化底层硬件。
  netif_add(&lan8720_netif, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, &ethernet_input);

  //将注册网卡函数注册的网卡设置为默认网卡
  netif_set_default(&lan8720_netif);

#if LWIP_DHCP           //如果开启了DHCP复位
  dhcp_start(&lan8720_netif);   //开启DHCP
#endif

  //打开网卡
  netif_set_up(&lan8720_netif);
}

/*-------------------------------------------------*/
/*函数名：网卡接收数据函数                         */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void LwIP_Pkt_Handle(void)
{ 
  ethernetif_input(&lan8720_netif); //从网络缓冲区中读取接收到的数据包并将其发送给LWIP处理 
}


extern struct tcp_pcb *tcp_client_pcb; 
extern c_srv_connect_t c_srv_ctimout;
extern int get_idle_memp_tab_num(void);
unsigned char buf[4096];
u16 dly_snd = 20;
/*-------------------------------------------------*/
/*函数名：LWIP周期性任务函数                       */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void lwip_periodic_handle(void)
{	
	
	int i;
	static unsigned int cnt = 0;
	static u32 last_time = 0;
	static u32 snd_cnt = 0;
	
	
	
	int num;
	
	
	if(LWipTime - last_time >= 10)
	{
		last_time = LWipTime;
		if((tcp_client_pcb != NULL) && (c_srv_ctimout.connect))
		{
			if(dly_snd == 0)
			{
				OS_ENTER_CRITICAL;
				if(tcp_client_pcb->snd_buf < 8*1024)
				{
					dly_snd = 1;
				}
				else if(ERR_OK == tcp_write(tcp_client_pcb,buf,sizeof(buf),1))
				{
					cnt ++;
					//if(0 == (cnt % 10))
					{
						data_led_cpl();
					}
					//num = get_idle_memp_tab_num();
					//printf("test snd ok:%08d,snd_buf left:%02d k byte,memp_tab left:%03d\r\n",cnt,tcp_client_pcb->snd_buf/1024,num);
					printf("test snd ok:%08d,snd_buf left:%02d k byte\r\n",cnt,tcp_client_pcb->snd_buf/1024);
					if(num < 10)
					{
						//__breakpoint(0);
					}
				}
				else
				{
					dly_snd = 20;
					printf("test snd err%d\r\n",cnt);
				}
				OS_EXIT_CRITICAL;
				snd_cnt ++;
				if(0 == (snd_cnt % 40))
				{
					//dly_snd = 20;
				}
			}
			if(dly_snd > 0)
			{
				dly_snd --;
			}
		}
	}
	
	//每500ms客户端发送一次数据
	if ((LWipTime - CLIENTTimer >= 500))
	{
		
		CLIENTTimer =  LWipTime;
		
		for(i = 0;i < sizeof(buf);i ++)
		{
			buf[i] = i%255;
		}
		OS_ENTER_CRITICAL;
		tcp_connect_check();
		OS_EXIT_CRITICAL;
		connect_led(c_srv_ctimout.connect);
		
		//TCP_Client_Send_Data(tcp_data,sizeof(tcp_data));//该函数为主动向服务器发送函数，
	}
	
	
#if LWIP_TCP
	//每250ms调用一次tcp_tmr()函数
  if (LWipTime - TCPTimer >= 250)
  {
    TCPTimer =  LWipTime;
		OS_ENTER_CRITICAL;
    tcp_tmr();
		OS_EXIT_CRITICAL;
  }
#endif
  //ARP每5s周期性调用一次
  if ((LWipTime - ARPTimer) >= ARP_TMR_INTERVAL)
  {
    ARPTimer =  LWipTime;
		OS_ENTER_CRITICAL;
    etharp_tmr();
		OS_EXIT_CRITICAL;
  }

#if LWIP_DHCP //如果使用DHCP的话
  //每500ms调用一次dhcp_fine_tmr()
  if (LWipTime - DHCPfineTimer >= DHCP_FINE_TIMER_MSECS)
  {
    DHCPfineTimer =  LWipTime;
		OS_ENTER_CRITICAL;
    dhcp_fine_tmr();
	if ((dhcpstatus != 1)&&(dhcpstatus != 0XFF))
    { 
		lwip_dhcp_process_handle();  //DHCP处理
    }
		OS_EXIT_CRITICAL;
  }

  //每60s执行一次DHCP粗糙处理
  if (LWipTime - DHCPcoarseTimer >= DHCP_COARSE_TIMER_MSECS)
  {
    DHCPcoarseTimer =  LWipTime;
		OS_ENTER_CRITICAL;
    dhcp_coarse_tmr();
		OS_EXIT_CRITICAL;
  }  
#endif
}

/*-------------------------------------------------*/
/*函数名：DHCP处理任务                             */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/

#if LWIP_DHCP//如果使能了DHCP

void lwip_dhcp_process_handle(void)
{
	u32 ip=0;       //IP
	u32 netmask=0;  //子网掩码
	u32 gw=0;       //网关掩码

	ip=lan8720_netif.ip_addr.addr;		//读取新IP地址
	netmask=lan8720_netif.netmask.addr;//读取子网掩码
	gw=lan8720_netif.gw.addr;			//读取默认网关 
	
	if(ip!=0)			//正确获取到IP地址的时候
	{
		dhcpstatus=1;	//DHCP成功
		printf("网卡名称\"%s\"的MAC地址为:................%d.%d.%d.%d.%d.%d\r\n",lan8720_netif.hostname,lan8720_netif.hwaddr[0],lan8720_netif.hwaddr[1],lan8720_netif.hwaddr[2],lan8720_netif.hwaddr[3],lan8720_netif.hwaddr[4],lan8720_netif.hwaddr[5]);
		//解析出通过DHCP获取到的IP地址
		lan8720_ip.ip[3]=(uint8_t)(ip>>24); 
		lan8720_ip.ip[2]=(uint8_t)(ip>>16);
		lan8720_ip.ip[1]=(uint8_t)(ip>>8);
		lan8720_ip.ip[0]=(uint8_t)(ip);
		printf("通过DHCP获取到IP地址..............%d.%d.%d.%d\r\n",lan8720_ip.ip[0],lan8720_ip.ip[1],lan8720_ip.ip[2],lan8720_ip.ip[3]);
		//解析通过DHCP获取到的子网掩码地址
		lan8720_ip.netmask[3]=(uint8_t)(netmask>>24);
		lan8720_ip.netmask[2]=(uint8_t)(netmask>>16);
		lan8720_ip.netmask[1]=(uint8_t)(netmask>>8);
		lan8720_ip.netmask[0]=(uint8_t)(netmask);
		printf("通过DHCP获取到子网掩码............%d.%d.%d.%d\r\n",lan8720_ip.netmask[0],lan8720_ip.netmask[1],lan8720_ip.netmask[2],lan8720_ip.netmask[3]);
		//解析出通过DHCP获取到的默认网关
		lan8720_ip.gateway[3]=(uint8_t)(gw>>24);
		lan8720_ip.gateway[2]=(uint8_t)(gw>>16);
		lan8720_ip.gateway[1]=(uint8_t)(gw>>8);
		lan8720_ip.gateway[0]=(uint8_t)(gw);
		printf("通过DHCP获取到的默认网关..........%d.%d.%d.%d\r\n",lan8720_ip.gateway[0],lan8720_ip.gateway[1],lan8720_ip.gateway[2],lan8720_ip.gateway[3]);
	}
	else if(lan8720_netif.dhcp->tries>LWIP_MAX_DHCP_TRIES) //通过DHCP服务获取IP地址失败,且超过最大尝试次数
	{
		dhcpstatus=0XFF;     //DHCP超时失败
		IP4_ADDR(&(lan8720_netif.ip_addr),lan8720_ip.ip[0],lan8720_ip.ip[1],lan8720_ip.ip[2],lan8720_ip.ip[3]);
		IP4_ADDR(&(lan8720_netif.netmask),lan8720_ip.netmask[0],lan8720_ip.netmask[1],lan8720_ip.netmask[2],lan8720_ip.netmask[3]);
		IP4_ADDR(&(lan8720_netif.gw),lan8720_ip.gateway[0],lan8720_ip.gateway[1],lan8720_ip.gateway[2],lan8720_ip.gateway[3]);
		printf("DHCP服务超时\r\n");
	}
}
#endif 

#define LWIP_USE_STM32F407VET6

/*-------------------------------------------------*/
/*函数名：以太网接口配置                           */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void GPIO_Configuration(void)
{
	uint8_t  time=50;
	uint32_t i;
	GPIO_InitTypeDef GPIO_InitStructure;
#ifdef LWIP_USE_STM32F407VET6

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOD , ENABLE);//使能GPIO时钟 RMII接口
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);   //使能SYSCFG时钟

    SYSCFG_ETH_MediaInterfaceConfig(SYSCFG_ETH_MediaInterface_RMII); //MAC和PHY之间使用RMII接口

    /*网络引脚设置 RMII接口
      ETH_MDIO -------------------------> PA2
      ETH_MDC --------------------------> PC1
      ETH_RMII_REF_CLK------------------> PA1
      ETH_RMII_CRS_DV ------------------> PA7
      ETH_RMII_RXD0 --------------------> PC4
      ETH_RMII_RXD1 --------------------> PC5

      ETH_RMII_TX_EN -------------------> PB11
      ETH_RMII_TXD0 --------------------> PB12
      ETH_RMII_TXD1 --------------------> PB13

      ETH_RESET-------------------------> PD3*/

    //配置PA1 PA2 PA7
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_ETH); //引脚复用到网络接口上
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_ETH);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_ETH);

    //配置PC1,PC4 and PC5
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource1, GPIO_AF_ETH); //引脚复用到网络接口上
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource4, GPIO_AF_ETH);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource5, GPIO_AF_ETH);

    //配置PB11, PB12 and PB13
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_ETH);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource12, GPIO_AF_ETH);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_ETH);
#else
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);    //使能GPIOA时钟 RMII接口
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);    //使能GPIOC时钟 RMII接口
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);    //使能GPIOD时钟 RMII接口
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);    //使能GPIOG时钟 RMII接口
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);   //使能SYSCFG时钟
	
	/*网络引脚设置 RMII接口
	ETH_MDIO -------------------------> PA2
	ETH_MDC --------------------------> PC1
	ETH_RMII_REF_CLK------------------> PA1
	ETH_RMII_CRS_DV ------------------> PA7
	ETH_RMII_RXD0 --------------------> PC4
	ETH_RMII_RXD1 --------------------> PC5
	ETH_RMII_TX_EN -------------------> PG11
	ETH_RMII_TXD0 --------------------> PG13
	ETH_RMII_TXD1 --------------------> PG14
	ETH_RESET-------------------------> PD3*/
					
	//配置PA1 PA2 PA7
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;  
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_ETH); //引脚复用到网络接口上
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_ETH);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_ETH);

	//配置PC1,PC4 and PC5
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource1, GPIO_AF_ETH); //引脚复用到网络接口上
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource4, GPIO_AF_ETH);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource5, GPIO_AF_ETH);
                                
	//配置PG11, PG14 and PG13 
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_11 | GPIO_Pin_13 | GPIO_Pin_14;
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource11, GPIO_AF_ETH);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource13, GPIO_AF_ETH);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource14, GPIO_AF_ETH);
#endif

	
	//配置PD3为推完输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	//推完输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;  
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOD,GPIO_Pin_3);//拉低PD3
	while(time--)
	{
		for(i=40000;i>0;i--);        //延时一会
	}
	GPIO_SetBits(GPIOD,GPIO_Pin_3);  //拉高PD3
}

/*-------------------------------------------------*/
/*函数名：以太网中断配置                           */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	
	NVIC_InitStructure.NVIC_IRQChannel = ETH_IRQn;  //以太网中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0X00;  //中断寄存器组2最高优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0X00;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);    
}

/*-------------------------------------------------*/
/*函数名：以太网配置                               */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void Ethernet_Configuration(void)
{ 
	ETH_InitTypeDef ETH_InitStructure; 
	
	SYSCFG_ETH_MediaInterfaceConfig(SYSCFG_ETH_MediaInterface_RMII); //MAC和PHY之间使用RMII接口
	
	//使能以太网MAC以及MAC接收和发送时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_ETH_MAC | RCC_AHB1Periph_ETH_MAC_Tx |RCC_AHB1Periph_ETH_MAC_Rx, ENABLE);
                        
	ETH_DeInit();  								//AHB总线重启以太网
	ETH_SoftwareReset();  						//软件重启网络
	while (ETH_GetSoftwareResetStatus() == SET);//等待软件重启网络完成 
	ETH_StructInit(&ETH_InitStructure); 	 	//初始化网络为默认值  

	///网络MAC参数设置 
	ETH_InitStructure.ETH_AutoNegotiation = ETH_AutoNegotiation_Enable;   			//开启网络自适应功能
	ETH_InitStructure.ETH_LoopbackMode = ETH_LoopbackMode_Disable;					//关闭反馈
	ETH_InitStructure.ETH_RetryTransmission = ETH_RetryTransmission_Disable; 		//关闭重传功能
	ETH_InitStructure.ETH_AutomaticPadCRCStrip = ETH_AutomaticPadCRCStrip_Disable; 	//关闭自动去除PDA/CRC功能 
	ETH_InitStructure.ETH_ReceiveAll = ETH_ReceiveAll_Disable;						//关闭接收所有的帧
	ETH_InitStructure.ETH_BroadcastFramesReception = ETH_BroadcastFramesReception_Enable;//允许接收所有广播帧
	ETH_InitStructure.ETH_PromiscuousMode = ETH_PromiscuousMode_Disable;			//关闭混合模式的地址过滤  
	ETH_InitStructure.ETH_MulticastFramesFilter = ETH_MulticastFramesFilter_Perfect;//对于组播地址使用完美地址过滤   
	ETH_InitStructure.ETH_UnicastFramesFilter = ETH_UnicastFramesFilter_Perfect;	//对单播地址使用完美地址过滤 
#if  !CHECKSUM_GEN_ICMP    //判断是否开启硬件校验,关闭软件校验
	ETH_InitStructure.ETH_ChecksumOffload = ETH_ChecksumOffload_Enable; 			//开启ipv4和TCP/UDP/ICMP的帧校验和卸载   
#endif
	//当我们使用帧校验和卸载功能的时候，一定要使能存储转发模式,存储转发模式中要保证整个帧存储在FIFO中,
	//这样MAC能插入/识别出帧校验值,当真校验正确的时候DMA就可以处理帧,否则就丢弃掉该帧
	ETH_InitStructure.ETH_DropTCPIPChecksumErrorFrame = ETH_DropTCPIPChecksumErrorFrame_Enable; //开启丢弃TCP/IP错误帧
	ETH_InitStructure.ETH_ReceiveStoreForward = ETH_ReceiveStoreForward_Enable;     //开启接收数据的存储转发模式    
	ETH_InitStructure.ETH_TransmitStoreForward = ETH_TransmitStoreForward_Enable;   //开启发送数据的存储转发模式  

	ETH_InitStructure.ETH_ForwardErrorFrames = ETH_ForwardErrorFrames_Disable;     	//禁止转发错误帧  
	ETH_InitStructure.ETH_ForwardUndersizedGoodFrames = ETH_ForwardUndersizedGoodFrames_Disable;	//不转发过小的好帧 
	ETH_InitStructure.ETH_SecondFrameOperate = ETH_SecondFrameOperate_Enable;  		//打开处理第二帧功能
	ETH_InitStructure.ETH_AddressAlignedBeats = ETH_AddressAlignedBeats_Enable;  	//开启DMA传输的地址对齐功能
	ETH_InitStructure.ETH_FixedBurst = ETH_FixedBurst_Enable;            			//开启固定突发功能    
	ETH_InitStructure.ETH_RxDMABurstLength = ETH_RxDMABurstLength_32Beat;     		//DMA发送的最大突发长度为32个节拍   
	ETH_InitStructure.ETH_TxDMABurstLength = ETH_TxDMABurstLength_32Beat;			//DMA接收的最大突发长度为32个节拍
	ETH_InitStructure.ETH_DMAArbitration = ETH_DMAArbitration_RoundRobin_RxTx_2_1;
	ETH_Init(&ETH_InitStructure,PHY_ADDRESS);		//配置ETH
	
	ETH_DMAITConfig(ETH_DMA_IT_NIS|ETH_DMA_IT_R,ENABLE);  	//使能以太网接收中断
	
}
