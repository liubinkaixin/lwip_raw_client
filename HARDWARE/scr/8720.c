/*-------------------------------------------------*/
/*         ��������STM32F407ZGT6������             */
/*    �Ա���ַ�� https://shop68304486.taobao.com   */
/*-------------------------------------------------*/
/*-------------------------------------------------*/
/*                  8720Դ�ļ�                     */
/*-------------------------------------------------*/
/*-------------------------------------------------*/

#include "8720.h"
#include "usart1.h"
#include "tcpclient.h" 
#include "main.h"
#include "CriticalSeg.h"
#include "lwip/stats.h"

__8720ip_dev   lan8720_ip;     //8720оƬIP��Ϣ���ƽṹ��
u8  MACaddr[6]={0x00,0x23,0x24,0x01,0x66,0x81};  //MAC��ַ������Ψһ��
u32 LWipTime=0;                //LWip����ʱ��
u8  dhcpstatus=0;              //DHCP״̬

struct netif lan8720_netif;     //����һ��ȫ�ֵ�����ӿڣ�ע����������Ҫ�õ�  
__IO uint32_t TCPTimer = 0;     //TCP��ѯ��ʱ�� 
__IO uint32_t ARPTimer = 0;     //ARP��ѯ��ʱ��
__IO uint32_t CLIENTTimer = 0;  //�ͻ��˷��ͼ�ʱ��

#if LWIP_DHCP                   //�������DHCP
u32 DHCPfineTimer=0;	        //DHCP��ϸ�����ʱ��
u32 DHCPcoarseTimer=0;	        //DHCP�ֲڴ����ʱ��
#endif

/*-------------------------------------------------*/
/*����������ʼ��lwip����ǰ������                   */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void MylwIP_Init(void)
{
  struct ip_addr ipaddr;     //����IP��ַ�ṹ��
  struct ip_addr netmask;    //������������ṹ��
  struct ip_addr gw;         //�������ؽṹ��

  mem_init();                //��ʼ����̬�ڴ��
  memp_init();               //��ʼ���ڴ��
  lwip_init();               //��ʼ��LWIP�ں�
	
#if LWIP_DHCP                //�������DHCP���Զ���ȡIP
  ipaddr.addr = 0;
  netmask.addr = 0;
  gw.addr = 0;                                   
#endif

  ETH_MACAddressConfig(ETH_MAC_Address0, MACaddr);  //����MAC��ַ

  //ע����������   ethernetif_init��������Ҫ�Լ�ʵ�֣���������netif����ֶΣ��Լ���ʼ���ײ�Ӳ����
  netif_add(&lan8720_netif, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, &ethernet_input);

  //��ע����������ע�����������ΪĬ������
  netif_set_default(&lan8720_netif);

#if LWIP_DHCP           //���������DHCP��λ
  dhcp_start(&lan8720_netif);   //����DHCP
#endif

  //������
  netif_set_up(&lan8720_netif);
}

/*-------------------------------------------------*/
/*�������������������ݺ���                         */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void LwIP_Pkt_Handle(void)
{ 
  ethernetif_input(&lan8720_netif); //�����绺�����ж�ȡ���յ������ݰ������䷢�͸�LWIP���� 
}


extern struct tcp_pcb *tcp_client_pcb; 
extern c_srv_connect_t c_srv_ctimout;
extern int get_idle_memp_tab_num(void);
unsigned char buf[4096];
u16 dly_snd = 20;
/*-------------------------------------------------*/
/*��������LWIP������������                       */
/*��  ������                                       */
/*����ֵ����                                       */
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
	
	//ÿ500ms�ͻ��˷���һ������
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
		
		//TCP_Client_Send_Data(tcp_data,sizeof(tcp_data));//�ú���Ϊ��������������ͺ�����
	}
	
	
#if LWIP_TCP
	//ÿ250ms����һ��tcp_tmr()����
  if (LWipTime - TCPTimer >= 250)
  {
    TCPTimer =  LWipTime;
		OS_ENTER_CRITICAL;
    tcp_tmr();
		OS_EXIT_CRITICAL;
  }
#endif
  //ARPÿ5s�����Ե���һ��
  if ((LWipTime - ARPTimer) >= ARP_TMR_INTERVAL)
  {
    ARPTimer =  LWipTime;
		OS_ENTER_CRITICAL;
    etharp_tmr();
		OS_EXIT_CRITICAL;
  }

#if LWIP_DHCP //���ʹ��DHCP�Ļ�
  //ÿ500ms����һ��dhcp_fine_tmr()
  if (LWipTime - DHCPfineTimer >= DHCP_FINE_TIMER_MSECS)
  {
    DHCPfineTimer =  LWipTime;
		OS_ENTER_CRITICAL;
    dhcp_fine_tmr();
	if ((dhcpstatus != 1)&&(dhcpstatus != 0XFF))
    { 
		lwip_dhcp_process_handle();  //DHCP����
    }
		OS_EXIT_CRITICAL;
  }

  //ÿ60sִ��һ��DHCP�ֲڴ���
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
/*��������DHCP��������                             */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/

#if LWIP_DHCP//���ʹ����DHCP

void lwip_dhcp_process_handle(void)
{
	u32 ip=0;       //IP
	u32 netmask=0;  //��������
	u32 gw=0;       //��������

	ip=lan8720_netif.ip_addr.addr;		//��ȡ��IP��ַ
	netmask=lan8720_netif.netmask.addr;//��ȡ��������
	gw=lan8720_netif.gw.addr;			//��ȡĬ������ 
	
	if(ip!=0)			//��ȷ��ȡ��IP��ַ��ʱ��
	{
		dhcpstatus=1;	//DHCP�ɹ�
		printf("��������\"%s\"��MAC��ַΪ:................%d.%d.%d.%d.%d.%d\r\n",lan8720_netif.hostname,lan8720_netif.hwaddr[0],lan8720_netif.hwaddr[1],lan8720_netif.hwaddr[2],lan8720_netif.hwaddr[3],lan8720_netif.hwaddr[4],lan8720_netif.hwaddr[5]);
		//������ͨ��DHCP��ȡ����IP��ַ
		lan8720_ip.ip[3]=(uint8_t)(ip>>24); 
		lan8720_ip.ip[2]=(uint8_t)(ip>>16);
		lan8720_ip.ip[1]=(uint8_t)(ip>>8);
		lan8720_ip.ip[0]=(uint8_t)(ip);
		printf("ͨ��DHCP��ȡ��IP��ַ..............%d.%d.%d.%d\r\n",lan8720_ip.ip[0],lan8720_ip.ip[1],lan8720_ip.ip[2],lan8720_ip.ip[3]);
		//����ͨ��DHCP��ȡ�������������ַ
		lan8720_ip.netmask[3]=(uint8_t)(netmask>>24);
		lan8720_ip.netmask[2]=(uint8_t)(netmask>>16);
		lan8720_ip.netmask[1]=(uint8_t)(netmask>>8);
		lan8720_ip.netmask[0]=(uint8_t)(netmask);
		printf("ͨ��DHCP��ȡ����������............%d.%d.%d.%d\r\n",lan8720_ip.netmask[0],lan8720_ip.netmask[1],lan8720_ip.netmask[2],lan8720_ip.netmask[3]);
		//������ͨ��DHCP��ȡ����Ĭ������
		lan8720_ip.gateway[3]=(uint8_t)(gw>>24);
		lan8720_ip.gateway[2]=(uint8_t)(gw>>16);
		lan8720_ip.gateway[1]=(uint8_t)(gw>>8);
		lan8720_ip.gateway[0]=(uint8_t)(gw);
		printf("ͨ��DHCP��ȡ����Ĭ������..........%d.%d.%d.%d\r\n",lan8720_ip.gateway[0],lan8720_ip.gateway[1],lan8720_ip.gateway[2],lan8720_ip.gateway[3]);
	}
	else if(lan8720_netif.dhcp->tries>LWIP_MAX_DHCP_TRIES) //ͨ��DHCP�����ȡIP��ַʧ��,�ҳ�������Դ���
	{
		dhcpstatus=0XFF;     //DHCP��ʱʧ��
		IP4_ADDR(&(lan8720_netif.ip_addr),lan8720_ip.ip[0],lan8720_ip.ip[1],lan8720_ip.ip[2],lan8720_ip.ip[3]);
		IP4_ADDR(&(lan8720_netif.netmask),lan8720_ip.netmask[0],lan8720_ip.netmask[1],lan8720_ip.netmask[2],lan8720_ip.netmask[3]);
		IP4_ADDR(&(lan8720_netif.gw),lan8720_ip.gateway[0],lan8720_ip.gateway[1],lan8720_ip.gateway[2],lan8720_ip.gateway[3]);
		printf("DHCP����ʱ\r\n");
	}
}
#endif 

#define LWIP_USE_STM32F407VET6

/*-------------------------------------------------*/
/*����������̫���ӿ�����                           */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void GPIO_Configuration(void)
{
	uint8_t  time=50;
	uint32_t i;
	GPIO_InitTypeDef GPIO_InitStructure;
#ifdef LWIP_USE_STM32F407VET6

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOD , ENABLE);//ʹ��GPIOʱ�� RMII�ӿ�
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);   //ʹ��SYSCFGʱ��

    SYSCFG_ETH_MediaInterfaceConfig(SYSCFG_ETH_MediaInterface_RMII); //MAC��PHY֮��ʹ��RMII�ӿ�

    /*������������ RMII�ӿ�
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

    //����PA1 PA2 PA7
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_ETH); //���Ÿ��õ�����ӿ���
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_ETH);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_ETH);

    //����PC1,PC4 and PC5
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource1, GPIO_AF_ETH); //���Ÿ��õ�����ӿ���
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource4, GPIO_AF_ETH);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource5, GPIO_AF_ETH);

    //����PB11, PB12 and PB13
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_ETH);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource12, GPIO_AF_ETH);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_ETH);
#else
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);    //ʹ��GPIOAʱ�� RMII�ӿ�
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);    //ʹ��GPIOCʱ�� RMII�ӿ�
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);    //ʹ��GPIODʱ�� RMII�ӿ�
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);    //ʹ��GPIOGʱ�� RMII�ӿ�
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);   //ʹ��SYSCFGʱ��
	
	/*������������ RMII�ӿ�
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
					
	//����PA1 PA2 PA7
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;  
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_ETH); //���Ÿ��õ�����ӿ���
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_ETH);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_ETH);

	//����PC1,PC4 and PC5
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource1, GPIO_AF_ETH); //���Ÿ��õ�����ӿ���
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource4, GPIO_AF_ETH);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource5, GPIO_AF_ETH);
                                
	//����PG11, PG14 and PG13 
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_11 | GPIO_Pin_13 | GPIO_Pin_14;
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource11, GPIO_AF_ETH);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource13, GPIO_AF_ETH);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource14, GPIO_AF_ETH);
#endif

	
	//����PD3Ϊ�������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	//�������
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;  
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOD,GPIO_Pin_3);//����PD3
	while(time--)
	{
		for(i=40000;i>0;i--);        //��ʱһ��
	}
	GPIO_SetBits(GPIOD,GPIO_Pin_3);  //����PD3
}

/*-------------------------------------------------*/
/*����������̫���ж�����                           */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	
	NVIC_InitStructure.NVIC_IRQChannel = ETH_IRQn;  //��̫���ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0X00;  //�жϼĴ�����2������ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0X00;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);    
}

/*-------------------------------------------------*/
/*����������̫������                               */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void Ethernet_Configuration(void)
{ 
	ETH_InitTypeDef ETH_InitStructure; 
	
	SYSCFG_ETH_MediaInterfaceConfig(SYSCFG_ETH_MediaInterface_RMII); //MAC��PHY֮��ʹ��RMII�ӿ�
	
	//ʹ����̫��MAC�Լ�MAC���պͷ���ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_ETH_MAC | RCC_AHB1Periph_ETH_MAC_Tx |RCC_AHB1Periph_ETH_MAC_Rx, ENABLE);
                        
	ETH_DeInit();  								//AHB����������̫��
	ETH_SoftwareReset();  						//�����������
	while (ETH_GetSoftwareResetStatus() == SET);//�ȴ��������������� 
	ETH_StructInit(&ETH_InitStructure); 	 	//��ʼ������ΪĬ��ֵ  

	///����MAC�������� 
	ETH_InitStructure.ETH_AutoNegotiation = ETH_AutoNegotiation_Enable;   			//������������Ӧ����
	ETH_InitStructure.ETH_LoopbackMode = ETH_LoopbackMode_Disable;					//�رշ���
	ETH_InitStructure.ETH_RetryTransmission = ETH_RetryTransmission_Disable; 		//�ر��ش�����
	ETH_InitStructure.ETH_AutomaticPadCRCStrip = ETH_AutomaticPadCRCStrip_Disable; 	//�ر��Զ�ȥ��PDA/CRC���� 
	ETH_InitStructure.ETH_ReceiveAll = ETH_ReceiveAll_Disable;						//�رս������е�֡
	ETH_InitStructure.ETH_BroadcastFramesReception = ETH_BroadcastFramesReception_Enable;//����������й㲥֡
	ETH_InitStructure.ETH_PromiscuousMode = ETH_PromiscuousMode_Disable;			//�رջ��ģʽ�ĵ�ַ����  
	ETH_InitStructure.ETH_MulticastFramesFilter = ETH_MulticastFramesFilter_Perfect;//�����鲥��ַʹ��������ַ����   
	ETH_InitStructure.ETH_UnicastFramesFilter = ETH_UnicastFramesFilter_Perfect;	//�Ե�����ַʹ��������ַ���� 
#if  !CHECKSUM_GEN_ICMP    //�ж��Ƿ���Ӳ��У��,�ر����У��
	ETH_InitStructure.ETH_ChecksumOffload = ETH_ChecksumOffload_Enable; 			//����ipv4��TCP/UDP/ICMP��֡У���ж��   
#endif
	//������ʹ��֡У���ж�ع��ܵ�ʱ��һ��Ҫʹ�ܴ洢ת��ģʽ,�洢ת��ģʽ��Ҫ��֤����֡�洢��FIFO��,
	//����MAC�ܲ���/ʶ���֡У��ֵ,����У����ȷ��ʱ��DMA�Ϳ��Դ���֡,����Ͷ�������֡
	ETH_InitStructure.ETH_DropTCPIPChecksumErrorFrame = ETH_DropTCPIPChecksumErrorFrame_Enable; //��������TCP/IP����֡
	ETH_InitStructure.ETH_ReceiveStoreForward = ETH_ReceiveStoreForward_Enable;     //�����������ݵĴ洢ת��ģʽ    
	ETH_InitStructure.ETH_TransmitStoreForward = ETH_TransmitStoreForward_Enable;   //�����������ݵĴ洢ת��ģʽ  

	ETH_InitStructure.ETH_ForwardErrorFrames = ETH_ForwardErrorFrames_Disable;     	//��ֹת������֡  
	ETH_InitStructure.ETH_ForwardUndersizedGoodFrames = ETH_ForwardUndersizedGoodFrames_Disable;	//��ת����С�ĺ�֡ 
	ETH_InitStructure.ETH_SecondFrameOperate = ETH_SecondFrameOperate_Enable;  		//�򿪴���ڶ�֡����
	ETH_InitStructure.ETH_AddressAlignedBeats = ETH_AddressAlignedBeats_Enable;  	//����DMA����ĵ�ַ���빦��
	ETH_InitStructure.ETH_FixedBurst = ETH_FixedBurst_Enable;            			//�����̶�ͻ������    
	ETH_InitStructure.ETH_RxDMABurstLength = ETH_RxDMABurstLength_32Beat;     		//DMA���͵����ͻ������Ϊ32������   
	ETH_InitStructure.ETH_TxDMABurstLength = ETH_TxDMABurstLength_32Beat;			//DMA���յ����ͻ������Ϊ32������
	ETH_InitStructure.ETH_DMAArbitration = ETH_DMAArbitration_RoundRobin_RxTx_2_1;
	ETH_Init(&ETH_InitStructure,PHY_ADDRESS);		//����ETH
	
	ETH_DMAITConfig(ETH_DMA_IT_NIS|ETH_DMA_IT_R,ENABLE);  	//ʹ����̫�������ж�
	
}
