/********************************************************************************
* @file    	  lte.c
* @function   LTE模块 AT命令配置
* @brief 	    用3.5.0版本库建的工程模板
* @attention	该例程仅作为参考，如需实际开发，请根据实际需求更改
* @version 	  V1.1
* @company  	深圳市飞思创电子科技有限公司
* @website  	http://www.freestrong.com
* @tel				0755-86528386
* @Author     freestrong
* @date    		2020/11/21
********************************************************************************/
#include "string.h"
#include "stdlib.h"
#include "lte_demo.h"
#include "fat_core.h"
#include "gpio.h"
#include "stdio.h"
#include "uart.h"
#include <stdio.h>
#include "stdarg.h"
#include "string.h"



#define LTE_POWER_ON_WAIT_TIME 2000 										//LTE开机等待时间
#define SIGNALMIN 15                                    //信号质量最低阀值
#define SIGNALMAX 31                                    //信号质量最低阀值
#define SOCKET_BUF_SIZE 128                             //Socket数据缓存大小
char cSocketRecvBuf[SOCKET_BUF_SIZE] = {0};             //socket数据接收缓存
char cSocketSendBuf[SOCKET_BUF_SIZE] = {0};             //socket数据发送缓存
uint8_t ucStateNum = 0;                                 //命令执行顺序标识值
uint8_t retrytimes = 0;                                 //命令重试次数
uint8_t ucErrorTimes = 0;                               //错误次数累计值
uint8_t ucFlightModeTimes = 0;															//进入飞行模式次数

typedef enum
{
    MD_RESET,        				//复位模块
    MD_AT_REQ,       				//AT握手
    MD_WORK_STA_CHK, 				//工作状态检测
    MD_CONNETINIT,   				//连接配置信息初始化
    MD_CONNETED,     				//数据通信
		MD_FLIGHTMODE,			   	//飞行模式
    MD_OK = 0xFE,    				//正常
    MD_ERR = 0xFF,   				//异常
} MD_RUN_STATE;

///**
// * @description:复位LTE模块
// * @param None
// * @return None
// */
//int module_reset(void)
//{
//    switch (ucStateNum)
//    {
//    //拉低PEN引脚
//    case 0:
//        PEN_GPIO_SET_LOW;
//        ucStateNum++;
//        break;
//    //拉高PEN引脚
//    case 1:
//        if (wait_timeout(1000))
//        {
//            PEN_GPIO_SET_HIGH;
//            ucStateNum = 0;
//            return 1;
//        }
//        break;
//    default:
//        break;
//    }
//    return 0;
//}

/**
 * @description:
 * @param str：要检索的字符串
 * @param minval：要匹配信号质量区间最小值
 * @param minval：要匹配信号质量区间最大值
 * @return 0:信号质量不满足正常工作状态, 1:信号质量满足正常工作状态
 */
int match_csq(char *str, int minval, int maxval)
{
    int lpCsqVal = 0;
    char tmp[5] = {0};
    char *p = NULL, *q = NULL;
		
    p = strstr(str, "+CSQ:");
    if (p == NULL)
    {
        return 0;
    }
		
    p = p + 5;
		
    while (*p < 0x30 || *p > 0x39)
    {
        p++;
    }
    q = p;

    while (*p != ',')
    {
        p++;
    }
    memcpy(tmp, q, p - q);
    lpCsqVal = atoi(tmp);
    /* 判断信号质量是否在设置的区间内 */
    if (lpCsqVal >= minval && lpCsqVal <= maxval)
    {
        return 1;
    }
    return 0;
}

/**
 * @description: 检测模块工作状态是否就绪
 * @param None
 * @return 0：检测未完成；MD_OK：模块已就绪；MD_ERR：错误，不满足工作状态
 */
int module_is_ready(void)
{
    switch (ucStateNum)
    {
    //关闭AT命令回显
    case 0x00:
        if (fat_send_wait_cmdres_blocking("ATE0\r\n", 1000))
        {
						//收到OK
            if (fat_cmdres_keyword_matching("OK"))
            {
                ucErrorTimes = 0;
                ucStateNum++;
            }
            else
            {
								//发送10次得不到正确应答
                if (ucErrorTimes++ > 10)
                {
                    ucStateNum = MD_ERR;
                }
            }
        }
        break;
    //读卡
    case 0x01:
        if (fat_send_wait_cmdres_blocking("AT+CPIN?\r\n", 1000))
        {
						//收到+CPIN: READY
            if (fat_cmdres_keyword_matching("+CPIN: READY"))
            {
                ucErrorTimes = 0;
                ucStateNum++;
            }
            else
            {     
								//发送10次得不到正确应答
                if (ucErrorTimes++ > 10)
                {
                    ucStateNum = MD_ERR;
                }
            }
        }
        break;
    //查询信号质量
    case 0x02:
        if (fat_send_wait_cmdres_blocking("AT+CSQ\r\n", 3000))
        {
						//收到OK
            if (fat_cmdres_keyword_matching("OK"))
            {
							//收到的是99（射频信号未初始化）
							if (fat_cmdres_keyword_matching("+CSQ: 99,99"))
								{
										//发送30次得不到正确应答
										if (ucErrorTimes++ > 30)
										{
												ucStateNum = MD_ERR;
										}
								}
              else
                {    
										//信号值在SIGNALMIN~SIGNALMAX这个区间
                    if (match_csq(cFatUartRecvBuf, SIGNALMIN, SIGNALMAX))
                    {
                        ucErrorTimes = 0;
												ucStateNum++;
                    }
										else 
										{
												ucStateNum = MD_ERR;
										}
                }
            }
						//没收到应答
            else
            {
								//发送30次不应答
                if (ucErrorTimes++ > 30)
                {
                    ucStateNum = MD_ERR;
                }
            }
        }
        break;
			//查看当前GPRS附着状态
    case 0x03:
        if (fat_send_wait_cmdres_blocking("AT+CGATT?\r\n", 1000))
        {
						//收到+CGATT: 1
            if (fat_cmdres_keyword_matching("+CGATT: 1"))
            {
                ucErrorTimes = 0;
								ucStateNum = MD_OK;
            }
            else
            {   
								//发送30次得不到正确应答
                if (ucErrorTimes++ > 30)
                {
                    ucStateNum = MD_ERR;
                }
            }
        }
        break;
		//错误跳至飞行模式
		case MD_ERR:
				ucStateNum = 0;
				return MD_ERR;
    //完成
    case MD_OK:
				ucStateNum = 0;
        return MD_OK;
    default:
        break;
    }
    return 0;
}

/**
 * @description: 连接和相关配置初始化
 * @param None
 * @return 0：检测未完成；MD_OK：模块已就绪；MD_ERR：错误，不满足工作状态
 */
int module_connet_parm_init(void)
{
    switch (ucStateNum)
    {
		//配置接收模式:从服务器接收的 MQTT 消息不以 URC 的形式上报
    case 0x00:
				//AT+QMTCFG="recv/mode",<client_idx>[,<msg_recv_mode>[,<msg_len_enable>]]
        if (fat_send_wait_cmdres_blocking("AT+QMTCFG=\"version\",0,4\r\n", 1000))
        {
						//收到OK
            if (fat_cmdres_keyword_matching("OK"))
            {
                ucErrorTimes = 0;
                ucStateNum++;
            }
            else
            {
								//发送10次得不到正确应答
                if (ucErrorTimes++ > 10)
                {
                    ucStateNum = MD_ERR;
                }
            }
        }
        break;
    //打开MQTT客户端网络
    case 0x01:
        if (fat_send_wait_cmdres_blocking("AT+QMTOPEN=0,\"mqtts.heclouds.com\",1883\r\n", 1000))
        {
						//收到+QMTOPEN: 0,0
            if (fat_cmdres_keyword_matching("+QMTOPEN: 0,0"))
            {
                ucErrorTimes = 0;
                ucStateNum++;
            }
            else
            {
								//发送10次得不到正确应答
                if (ucErrorTimes++ > 10)
                {
                    ucStateNum = MD_ERR;
                }
            }
        }
        break;
    //将MQTT客户端连接服务器
    case 0x02:
        if (fat_send_wait_cmdres_blocking("AT+QMTCONN=0,gpsguide,O1rPy548ZP,version=2018-10-31&res=products%2FO1rPy548ZP%2Fdevices%2Fgpsguide&et=2052914641&method=md5&sign=w%2B3CL1kxR21g6MlQ9zL8Sw%3D%3D \r\n", 2000))
        {
						//收到+QMTCONN: 0,0,0
            if (fat_cmdres_keyword_matching("+QMTCONN: 0,0,0"))
            {
                ucErrorTimes = 0;
                ucStateNum++;
            }
            else
            {
								//发送10次得不到正确应答
                if (ucErrorTimes++ > 10)
                {
                    ucStateNum = MD_ERR;
                }
            }
        }
        break;
		//订阅主题
    case 0x03:
        if (fat_send_wait_cmdres_blocking("AT+QMTSUB=0,1,\"$sys/O1rPy548ZP/gpsguide/dp/post/json/+\",0\r\n", 1000))
        {
						//收到+QMTSUB: 0,1,0,0
            if (fat_cmdres_keyword_matching("+QMTSUB: 0,1,0,0"))
            {
                ucErrorTimes = 0;
                ucStateNum = MD_OK;
            }
            else
            {
								//发送5次得不到正确应答
                if (ucErrorTimes++ > 5)
                {
                    ucStateNum = MD_ERR;
                }
            }
        }
        break;
    //完成
    case MD_OK:
				ucStateNum = 0;
        return MD_OK;
    //错误跳至飞行模式
    case MD_ERR:
				ucStateNum = 0;
        return MD_ERR;
    default:
        break;
    }
    return 0;
}

/**
 * @description: 截取socket接收缓存区的Topic
 * @param cmdres：命令响应数据
 * @param recvbuf：socket接收缓存
 * @return 接收Topic的长度
 */
int match_recv_topic(char *cmdres, char *recvbuf)
{
    int length = 0;
    char *p = NULL, *q = NULL;
    p = strstr(cmdres, "+QMTRECV: 0,0,\"");
    if (p == NULL)
    {
        return 0;
    }
    p = p + 15;
    q = p;
    while (*p != 0x2C)
    {
        p++;
    }
    length = p - q - 1;
    memset(recvbuf, 0, SOCKET_BUF_SIZE);
    memcpy(recvbuf, q, length);
    return length;
}

/**
 * @description: 截取socket接收缓存区的Topic的内容
 * @param cmdres：命令响应数据
 * @param recvbuf：socket接收缓存
 * @return 接收数据的长度
 */
int match_recv_data(char *cmdres, char *recvbuf)
{
    int length = 0;
    char *p = NULL, *q = NULL;
		//定位至应答的OK
    p = strstr(cmdres, "OK");
    if (p == NULL)
    {
        return 0;
    }
		//定位至Topic内容后的"
		p = p - 5;
		q = p;
		do
		{
        p--;
    }
    while (*p != 0x22);
    length = q - p - 1;	
    memset(recvbuf, 0, SOCKET_BUF_SIZE);
    memcpy(recvbuf, p + 1, length);
    return length;
}

/**
 * @description: 数据收发部分
 * @return 0：检测未完成；MD_ERR：错误
 */
int module_data(void)
{
    int ret = 0;
    switch (ucStateNum)
    {
    //定长主题发布数据
    case 0x00:
        if (fat_send_wait_cmdres_nonblocking("AT+QMTPUB=0,0,0,0,\"$sys/O1rPy548ZP/gpsguide/dp/post/json\"\r\n", 1000))
        {
						//收到>
            if (fat_cmdres_keyword_matching(">"))
            {
                ucErrorTimes = 0;
                ucStateNum++;
            }
            else
            {
								//发送3次得不到正确应答
                if (ucErrorTimes++ > 3)
                {
                    ucStateNum = MD_ERR;
                }
            }
        }
        break;
		//数据
		                                  
    case 0x01: 
		char str[256] = {0};
		
		snprintf(str, sizeof(str),"{\"id\": 123,\"dp\": {\"location\": [{\"v\":{\"lon\": %.6lf,\"lat\": %.6lf}}]}}",longi,lati);				
		printf("str:%s\r\n",str);
//		snprintf(str, sizeof(str),"{\"id\": 123,\"dp\": {\"latitude\": [{\"v\": %.6lf}],\"longitude\": [{\"v\": %.6lf}]}}",lati,longi);		
		fat_send_wait_cmdres_nonblocking(str, 5000);
        USART_SendData(USART2,0x1A);
		fat_delay(100);
		//收到OK
		if (fat_cmdres_keyword_matching("+QMTPUB: 0,0,0"))
		{
			ucErrorTimes = 0;
			ucStateNum=0x00;
		}
		else
		{
							//发送3次得不到正确应答
			if (ucErrorTimes++ > 5)
			{
				ucStateNum = MD_ERR;
			}
		}
        
        break;
    
		//错误跳至飞行模式
    case MD_ERR:
				ucStateNum = 0;
        return MD_ERR;
    default:
        break;
    }
    return 0;
}

/**
 * @description: 飞行模式处理函数
 * @param None
 * @return 0：检测未完成；MD_WORK_STA_CHK：重新开启网络跳至模块状态检测；MD_ERR：错误
 */
int module_flightmode()
{
		switch (ucStateNum)
    {
			case 0x00:
				ucFlightModeTimes++;
				ucStateNum++;
				FAT_DBGPRT_INFO("进入飞行模式次数：%d\r\n",ucFlightModeTimes);
				break;
			case 0x01:
				if (ucFlightModeTimes == 2)
				{
						FAT_DBGPRT_INFO("第二次进入飞行模式，复位模块\r\n");
						ucStateNum = MD_ERR;
				}
				else
					ucStateNum++;
				break;
			case 0x02:
        if (fat_send_wait_cmdres_blocking("AT+CFUN=0\r\n", 2000))
        {
						//收到OK
            if (fat_cmdres_keyword_matching("OK"))
            {
								ucErrorTimes = 0;
                ucStateNum++;
            }
						else
            {
								//发送5次得不到正确应答，跳至MD_ERR
                if (ucErrorTimes++ > 5)
                {
                    ucStateNum = MD_ERR;
                }
            }
        }
        break;
			case 0x03:
        if (wait_timeout(5000))
						ucStateNum++;
        break;
			case 0x04:
        if (fat_send_wait_cmdres_blocking("AT+CFUN=1\r\n", 2000))
        {
						//收到OK,状态更为MD_WORK_STA_CHK，跳至模块状态检测
            if (fat_cmdres_keyword_matching("OK"))
            {
								FAT_DBGPRT_INFO("再开启功能\r\n");
								ucStateNum = 0;
								return MD_WORK_STA_CHK;
            }
						else
            {
								//发送5次得不到正确应答，跳至MD_ERR
                if (ucErrorTimes++ > 5)
                {
                    ucStateNum = MD_ERR;
                }
            }
        }
        break;
			case MD_ERR:
				ucStateNum = 0;
				return MD_ERR;
			default:
        break;
    }
    return 0;	
}

/**
 * @description: LTE模块MQTT协议
 * @param None
 * @return None
 */
void module_MQTT(void)
{
    static int state = MD_RESET;
    int ret = 0;
    switch (state)
    {
    //复位模块
    case MD_RESET:
        if (fat_send_wait_cmdres_blocking("AT+RST\r\n", 5000))
        {
//			 if (fat_cmdres_keyword_matching("OK"))
					state = MD_AT_REQ;
//			 else
//			 {
//				 printf("no");
//			 }
        }
        break;
    //AT握手
    case MD_AT_REQ:
        if (fat_send_wait_cmdres_blocking("AT\r\n", 5000))//5s
        {
            if (fat_cmdres_keyword_matching("OK"))
            {
				printf("标记");
                ucErrorTimes = 0;
                state = MD_WORK_STA_CHK;
            }
            else
            {
                if (ucErrorTimes++ > 10)
                {
                    state = MD_ERR;
                }
            }
        }
        break;
    //模块状态检测
    case MD_WORK_STA_CHK:
		
        ret = module_is_ready();
        if (ret == MD_OK)
        {
            state = MD_CONNETINIT;
        }
				else if (ret == MD_ERR)
        {
            state = MD_FLIGHTMODE;
        }
        break;
    //连接参数初始化
    case MD_CONNETINIT:
        ret = module_connet_parm_init();
        if (ret == MD_OK)
        {
            state = MD_CONNETED;
        }
        else if (ret == MD_ERR)
        {
            state = MD_FLIGHTMODE;
        }
        break;
    //数据通信处理
    case MD_CONNETED:
        if (module_data() == MD_ERR)
        {
            state = MD_FLIGHTMODE;
        }
        break;
		//飞行模式处理
		case MD_FLIGHTMODE:
				ret = module_flightmode();
				if(ret == MD_WORK_STA_CHK)
        {
            state = MD_WORK_STA_CHK;
        }
				else if(ret == MD_ERR)
        {
						ucFlightModeTimes = 0;
            state = MD_ERR;
        }
				break;
    //错误
    case MD_ERR:
	 		  ucErrorTimes = 0;
				state = MD_RESET;
        break;
    default:
        break;
    }
}

