/*-------------------------------------------------*/
/*         ��γ����STM32F407ZGT6������             */
/*    �Ա���ַ�� http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/
/*-------------------------------------------------*/
/*                ESP8266ͷ�ļ�                    */
/*-------------------------------------------------*/
/*-------------------------------------------------*/

#ifndef __ESP8266_H
#define __ESP8266_H	 

#include "main.h"

#define   BUFF_LEN         1024

#define SSID  "STM32103+ESP8266"    //ssid
#define PASS  "012345678"           //����
#define LOCAL_PORT   5050           //���ض˿ں�
#define WPA_WPA2_PSK    4           //���ܷ�ʽ

extern u8 Esp8266_Buff[BUFF_LEN];      //���ݻ�����
extern u8 IP_Buff[20];                 //���8266��IP
extern u8 Transmission_Sta;            //����״̬��1����������͸��     0��͸��

u8 ESP8266_SmartLink(u16 timeout);                                                       //�������Ӻ���
u8 ESP8266_SendCmd_OK(u8 *cmd, u16 timeout);                                             //����ָ������ڴ��Ļظ���OK
u8 ESP8266_SoftReset(u16 timeout);                                                       //��λ����
u8 ESP8266_GetIP(u8 *ip, u16 timeout);                                                   //�ȴ�����wifi����ȡIP��ַ
void ESP8266_Exit_Transfer(u16 timeout);                                                 //��⣬���͸��ģʽ���˳�͸��
u8 ESP8266_STA_TCPServer(void);                                                          //����STAģʽ��TCP������
u8 ESP8266_Get_LinkSta(void);                                                            //��ȡ����״̬ 
u8 ESP8266_Get_Data(u8 *data, u8 *len, u8 *id);                                          //��ȡ�ͻ������� 
u8 ESP8266_SendData(u8 id, u8 *databuff, u16 data_len, u16 timeout);                     //�������������� 
u16 Char_to_Hex(u8 *char_data, u16 len);                                                 //�ַ�������ת�������֣�0-9999  
u8 ESP8266_Connect_Server(u8 *ip, u16 port,u16 timeout);                                 //����TCP��������������͸��ģʽ 
u8 ESP8266_STA_TCPCLient(u8 *ip, u16 port);                                              //����STAģʽ��TCP������ 
u8 ESP8266_Set_UDPlimit(u8 *ip, u16 port, u16 local_port, u16 timeout);                  //�����޶���UDPԶ������
u8 ESP8266_STA_UDPlimit_Init(u8 *ip, u16 port);                                          //����STAģʽ��UDP�޶�͸�� 
u8 ESP8266_Set_UDPfreedom(u8 *ip, u16 port, u16 local_port, u16 timeout);                //��������ģʽ��UDP
u8 ESP8266_STA_UDPfreedom_Init(u8 *ip, u16 port);                                        //����STAģʽ��UDP����ģʽ
u8 ESP8266_UDPGet_Data(u8 *data, u8 *len);                                               //UDP����ģʽ��ȡ�ͻ�������
u8 ESP8266_UDP_SendData(u8 *databuff, u16 data_len, u16 timeout);                        //UDP��������
u8 Yeelink_get(const char *device_id,const char *sensors_id, char *value, u16 timeout);  //��ȡYeelink�������п��ص�״̬ 
u8 Yeelink_post(const char *device_id, const char *sensors_id, char *value, u16 timeout);//Yeelink�ϴ������������� 
u8 OneNet_get(const char *device_id,const char *dataflow, u8 *value,u16 timeout);        //��ȡOnenet�������п��ص�״̬
u8 OneNet_post(const char *device_id,u8 num,u8 *value,u16 timeout);                      //OneNet�ϴ������������� 
u8 ESP8266_Cloud_Server(void);                                                           //���Ӱ������Ʒ����� 
u8 ESP8266_Cloud_GetData(u8 *data, u8 *len);                                             //��ȡ�������Ʒ���������������
u8 ESP8266_Cloud_SendData(u8 *databuff, u16 data_len, u16 timeout);                      //���ſɷ�������������
void ESP8266_Switch8Sta(void);                                                           //�ظ�8·���ؿ�����λ��8·���ص�״̬
void ESP8266_TempSent(void);                                                             //�ظ���λ���¶�ֵ�ɼ�
void ESP8266_HumiSent(void);                                                             //�ظ���λ��ʪ��ֵ�ɼ�
void ESP8266_LumiSent(void);                                                             //�ظ���λ����ǿ�Ȳɼ�
void ESP8266_Switch2Sta(void);                                                           //�ظ�APP 2·���ص�״̬
void ESP8266_APPdata2Sent(void);                                                         //�ظ�APP���2·����ֵ 
u8 ESP8266_APmode(void);                                                                 //����APģʽ  
u8 ESP8266_SET_AP(u8 *ssid, u8 *password, u8 mode, u16 timeout);                         //����AP����

#endif



