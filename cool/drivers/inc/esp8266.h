#ifndef __ESP8266_H
#define __ESP8266_H

/*头文件*/
#include "usart_drv.h"	       
#include "adc_drv.h"
#include "SGP30.h"
#include "string.h"
#include "gxht3x.h"
#include <stdlib.h>

/*全局变量声明*/
extern char Connect_flag;  //外部变量声明，同服务器连接状态  0：还没有连接服务器  1：连接上服务器了
extern u16	co2_value;
extern int Temp;
extern  uint16_t Humi;


/*宏定义*/
#define RESET_IO(x)    GPIO_WriteBit(GPIOA, GPIO_Pin_12, (BitAction)x)  //PA控制WiFi的复位

#define WiFi_printf       u4_printf           //串口2控制 WiFi
#define WiFi_RxCounter    Uart4_RxCounter    //串口2控制 WiFi
#define WiFi_RX_BUF       Uart4_RxBuff       //串口2控制 WiFi
#define WiFi_RXBUFF_SIZE  UART4_RXBUFF_SIZE  //串口2控制 WiFi

#define SSID   "hoostar2-24G"            //路由器SSID名称
#define PASS   "1234567890"                    //路由器密码

/*函数声明*/
void WiFi_ResetIO_Init(void);
char WiFi_SendCmd(char *cmd, int timeout);
char WiFi_Reset(int timeout);
char WiFi_JoinAP(int timeout);
char WiFi_Connect_Server(int timeout);
char WiFi_Smartconfig(int timeout);
char WiFi_WaitAP(int timeout);
char WiFi_GetIP(int timeout);
char WiFi_Get_LinkSta(void);
char WiFi_Get_Data(char *data, char *len, char *id);
char WiFi_SendData(char id, char *databuff, int data_len, int timeout);
char WiFi_Connect_Server(int timeout);
char WiFi_SET_AP(char *ssid, char *password, char mode, int timeout);
char WiFi_ConnectServer(void);
char wifi_init(void);
char WiFi_ack(void);

#endif


