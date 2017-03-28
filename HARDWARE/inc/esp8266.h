/*-------------------------------------------------*/
/*         超纬电子STM32F407ZGT6开发板             */
/*    淘宝地址： http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/
/*-------------------------------------------------*/
/*                ESP8266头文件                    */
/*-------------------------------------------------*/
/*-------------------------------------------------*/

#ifndef __ESP8266_H
#define __ESP8266_H	 

#include "main.h"

#define   BUFF_LEN         1024

#define SSID  "STM32103+ESP8266"    //ssid
#define PASS  "012345678"           //密码
#define LOCAL_PORT   5050           //本地端口号
#define WPA_WPA2_PSK    4           //加密方式

extern u8 Esp8266_Buff[BUFF_LEN];      //数据缓冲区
extern u8 IP_Buff[20];                 //存放8266的IP
extern u8 Transmission_Sta;            //传输状态，1：正常，非透传     0：透传

u8 ESP8266_SmartLink(u16 timeout);                                                       //智能连接函数
u8 ESP8266_SendCmd_OK(u8 *cmd, u16 timeout);                                             //发送指令函数，期待的回复是OK
u8 ESP8266_SoftReset(u16 timeout);                                                       //软复位函数
u8 ESP8266_GetIP(u8 *ip, u16 timeout);                                                   //等待连接wifi，获取IP地址
void ESP8266_Exit_Transfer(u16 timeout);                                                 //检测，如果透传模式则退出透传
u8 ESP8266_STA_TCPServer(void);                                                          //设置STA模式下TCP服务器
u8 ESP8266_Get_LinkSta(void);                                                            //获取连接状态 
u8 ESP8266_Get_Data(u8 *data, u8 *len, u8 *id);                                          //获取客户端数据 
u8 ESP8266_SendData(u8 id, u8 *databuff, u16 data_len, u16 timeout);                     //服务器发送数据 
u16 Char_to_Hex(u8 *char_data, u16 len);                                                 //字符型数字转换成数字，0-9999  
u8 ESP8266_Connect_Server(u8 *ip, u16 port,u16 timeout);                                 //连接TCP服务器，并进入透传模式 
u8 ESP8266_STA_TCPCLient(u8 *ip, u16 port);                                              //设置STA模式下TCP服务器 
u8 ESP8266_Set_UDPlimit(u8 *ip, u16 port, u16 local_port, u16 timeout);                  //设置限定的UDP远程主机
u8 ESP8266_STA_UDPlimit_Init(u8 *ip, u16 port);                                          //设置STA模式下UDP限定透传 
u8 ESP8266_Set_UDPfreedom(u8 *ip, u16 port, u16 local_port, u16 timeout);                //设置自由模式的UDP
u8 ESP8266_STA_UDPfreedom_Init(u8 *ip, u16 port);                                        //设置STA模式下UDP自由模式
u8 ESP8266_UDPGet_Data(u8 *data, u8 *len);                                               //UDP自由模式获取客户端数据
u8 ESP8266_UDP_SendData(u8 *databuff, u16 data_len, u16 timeout);                        //UDP发送数据
u8 Yeelink_get(const char *device_id,const char *sensors_id, char *value, u16 timeout);  //获取Yeelink服务器中开关的状态 
u8 Yeelink_post(const char *device_id, const char *sensors_id, char *value, u16 timeout);//Yeelink上传传感器的数据 
u8 OneNet_get(const char *device_id,const char *dataflow, u8 *value,u16 timeout);        //获取Onenet服务器中开关的状态
u8 OneNet_post(const char *device_id,u8 num,u8 *value,u16 timeout);                      //OneNet上传传感器的数据 
u8 ESP8266_Cloud_Server(void);                                                           //连接安可信云服务器 
u8 ESP8266_Cloud_GetData(u8 *data, u8 *len);                                             //获取安可信云服务器发来的数据
u8 ESP8266_Cloud_SendData(u8 *databuff, u16 data_len, u16 timeout);                      //向安信可服务器发送数据
void ESP8266_Switch8Sta(void);                                                           //回复8路开关控制上位机8路开关的状态
void ESP8266_TempSent(void);                                                             //回复上位机温度值采集
void ESP8266_HumiSent(void);                                                             //回复上位机湿度值采集
void ESP8266_LumiSent(void);                                                             //回复上位机光强度采集
void ESP8266_Switch2Sta(void);                                                           //回复APP 2路开关的状态
void ESP8266_APPdata2Sent(void);                                                         //回复APP软件2路数据值 
u8 ESP8266_APmode(void);                                                                 //设置AP模式  
u8 ESP8266_SET_AP(u8 *ssid, u8 *password, u8 mode, u16 timeout);                         //设置AP参数

#endif



