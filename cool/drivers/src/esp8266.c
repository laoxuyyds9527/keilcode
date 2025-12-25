#include "esp8266.h"	    
#include "systick.h"	    
#include "usart_drv.h"	  


char wifi_mode = 0;     //联网模式 0：SSID和密码写在程序里   1：Smartconfig方式用APP发送
char Connect_flag;      //同服务器连接状态  0：还没有连接服务器  1：连接上服务器了

extern char *ServerIP;         //存放服务器IP或是域名
extern int  ServerPort;        //存放服务器的端口号区

/*
 *brief：初始化WiFi的复位IO                       
 *param：无                                       
 *retval：无                                       
 */
void WiFi_ResetIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;                      //定义一个设置IO端口参数的结构体
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA , ENABLE);   //使能PA端口时钟
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;                 //准备设置PA12
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;         //速率50Mhz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   		  //推免输出方式
	GPIO_Init(GPIOA, &GPIO_InitStructure);            		  //设置PA4
	RESET_IO(1);                                              //复位IO拉高电平
}

/*
 *brief：WiFi发送设置指令                        
 *param：cmd：指令                                
 *param：timeout：超时时间（100ms的倍数）         
 *retval：0：正确   其他：错误                     
 */
char WiFi_SendCmd(char *cmd, int timeout)
{
	WiFi_RxCounter=0;                           //WiFi接收数据量变量清零                        
	memset(WiFi_RX_BUF,0,WiFi_RXBUFF_SIZE);     //清空WiFi接收缓冲区 
	WiFi_printf("%s\r\n",cmd);                  //发送指令
	while(timeout--)							//等待超时时间到0
	{                           
		delay_ms(10);                          //延时100ms
		if(strstr(WiFi_RX_BUF,"OK"))            //如果接收到OK表示指令成功
			break;       						//主动跳出while循环
		printf("%d ",timeout);               //串口输出现在的超时时间
	}
	printf("\r\n");                          //串口输出信息
	if(timeout<=0)
		return 1;                     //如果timeout<=0，说明超时时间到了，也没能收到OK，返回1
	else 
		return 0;		         				//反之，表示正确，说明收到OK，通过break主动跳出while
}

/*
 *brief：WiFi复位                                 
 *param：timeout：超时时间（100ms的倍数）         
 *retval：0：正确   其他：错误                     
 */
char WiFi_Reset(int timeout)
{
	RESET_IO(0);                                    //复位IO拉低电平
	delay_ms(500);                                  //延时500ms
	RESET_IO(1); 
	WiFi_SendCmd("AT+RST",10);
	while(timeout--)								//等待超时时间到0
	{                               
		delay_ms(100);                              //延时100ms
		if(strstr(WiFi_RX_BUF,"ready"))             //如果接收到ready表示复位成功
			break;  
		printf("标记 ");     						    //主动跳出while循环
		printf("%d ",timeout);                   //串口输出现在的超时时间
	}
	printf("\r\n");                              //串口输出信息
	if(timeout<=0)return 1;                         //如果timeout<=0，说明超时时间到了，也没能收到ready，返回1
	else return 0;		         				    //反之，表示正确，说明收到ready，通过break主动跳出while
}

/*
 *brief：WiFi加入路由器指令                       
 *param：timeout：超时时间（1s的倍数）            
 *retval：0：正确   其他：错误                     
 */
char WiFi_JoinAP(int timeout)
{		
	WiFi_RxCounter=0;                               //WiFi接收数据量变量清零                        
	memset(WiFi_RX_BUF,0,WiFi_RXBUFF_SIZE);         //清空WiFi接收缓冲区 
	WiFi_printf("AT+CWJAP=\"%s\",\"%s\"\r\n",SSID,PASS); //发送指令	
	while(timeout--)								
	{                               
		delay_ms(1000);                             //延时1s
		if(strstr(WiFi_RX_BUF,"WIFI GOT IP\r\n\r\nOK")) //如果接收到WIFI GOT IP表示成功
			break;       						    //主动跳出while循环
		printf("%d ",timeout);                  
	}
	printf("\r\n");                              
	if(timeout<=0)return 1;                         //如果timeout<=0，说明超时时间到了，也没能收到WIFI GOT IP，返回1
	return 0;                                       //正确，返回0
}

/*
 *brief：WiFi_Smartconfig                         
 *param：timeout：超时时间（1s的倍数）            
 *retval：0：正确   其他：错误                     
 */
char WiFi_Smartconfig(int timeout)
{	
	WiFi_RxCounter=0;                           //WiFi接收数据量变量清零                        
	memset(WiFi_RX_BUF,0,WiFi_RXBUFF_SIZE);     //清空WiFi接收缓冲区     
	while(timeout--)
	{                           
		delay_ms(1000);                         //延时1s
		if(strstr(WiFi_RX_BUF,"connected"))     //如果串口接受到connected表示成功
			break;                              //跳出while循环  
		printf("%d ",timeout);               //串口输出现在的超时时间  
	}	
	printf("\r\n");                          
	if(timeout<=0)return 1;                     //超时错误，返回1
	return 0;                                   //正确返回0
}

/*
 *brief：等待加入路由器                           
 *param：timeout：超时时间（1s的倍数）            
 *retval：0：正确   其他：错误                     
 */
char WiFi_WaitAP(int timeout)
{		
	while(timeout--)
	{                               
		delay_ms(1000);                             //延时1s
		if(strstr(WiFi_RX_BUF,"WIFI GOT IP"))       //如果接收到WIFI GOT IP表示成功
			break;       						    //主动跳出while循环
		printf("%d ",timeout);                   //串口输出现在的超时时间
	}
	printf("\r\n");                             
	if(timeout<=0)return 1;                         //如果timeout<=0，说明超时时间到了，也没能收到WIFI GOT IP，返回1
	return 0;                                       //正确，返回0
}

/*
 *brief：等待连接wifi，获取IP地址                 
 *param：ip：保存IP的数组                         
 *param：timeout：超时时间（100ms的倍数）         
 *retval：0：正确   其他：错误                     
 */
char WiFi_GetIP(int timeout)
{
	char *presult1,*presult2;
	char ip[50];
	
	WiFi_RxCounter=0;                               //WiFi接收数据量变量清零                        
	memset(WiFi_RX_BUF,0,WiFi_RXBUFF_SIZE);         //清空WiFi接收缓冲区 
	WiFi_printf("AT+CIFSR\r\n");                    //发送指令	
	while(timeout--)
	{                               
		delay_ms(100);                              //延时100ms
		if(strstr(WiFi_RX_BUF,"OK"))                //如果接收到OK表示成功
			break;       						    //主动跳出while循环
		printf("%d ",timeout);                   //串口输出现在的超时时间
	}
	printf("\r\n");                              
	if(timeout<=0)return 1;                         //如果timeout<=0，说明超时时间到了，也没能收到OK，返回1
	else
	{
		presult1 = strstr(WiFi_RX_BUF,"\"");
		if( presult1 != NULL ){
			presult2 = strstr(presult1+1,"\"");
			if( presult2 != NULL )
			{
				memcpy(ip,presult1+1,presult2-presult1-1);
				printf("ESP8266的IP地址：%s\r\n",ip);     //串口显示IP地址
				return 0;    //正确返回0
			}
			else return 2;  //未收到预期数据
		}
		else return 3;      //未收到预期数据	
	}
}

/*
 *brief：获取连接状态                             
 *param：无                                       
 *retval：连接状态                                 
 *        0：无状态                                
 *        1：有客户端接入                          
 *        2：有客户端断开                          
 */
char WiFi_Get_LinkSta(void)
{
	char id_temp[10]={0};    //缓冲区，存放ID
	char sta_temp[10]={0};   //缓冲区，存放状态
	
	if(strstr(WiFi_RX_BUF,"CONNECT"))				//如果接受到CONNECT表示有客户端连接
	{                 	
		sscanf(WiFi_RX_BUF,"%[^,],%[^,]",id_temp,sta_temp);
		printf("有客户端接入，ID=%s\r\n",id_temp);  
		WiFi_RxCounter=0;                              //WiFi接收数据量变量清零                        
		memset(WiFi_RX_BUF,0,WiFi_RXBUFF_SIZE);        //清空WiFi接收缓冲区     
		return 1;                                      //有客户端接入
	}
	else if(strstr(WiFi_RX_BUF,"CLOSED"))				//如果接受到CLOSED表示有链接断开
	{            	
		sscanf(WiFi_RX_BUF,"%[^,],%[^,]",id_temp,sta_temp);
		printf("有客户端断开，ID=%s\r\n",id_temp);        
		WiFi_RxCounter=0;                                    //WiFi接收数据量变量清零                        
		memset(WiFi_RX_BUF,0,WiFi_RXBUFF_SIZE);              //清空WiFi接收缓冲区     
		return 2;                                            //有客户端断开
	}
	else return 0;                                          //无状态改变	
}
/*
 *brief：获取客户端数据                           
 *        两组回车换行符\r\n\r\n作为数据的结束符   
 *param：data：数据缓冲区                         
 *param：len： 数据量                             
 *param：id：  发来数据的客户端的连接ID           
 *retval：数据状态                                 
 *        0：无数据                                
 *        1：有数据                                
 */
char WiFi_Get_Data(char *data, char *len, char *id)
{
	char temp[10]={0};      //缓冲区
	char *presult;

	if(strstr(WiFi_RX_BUF,"\r\n\r\n"))						//两个连着的回车换行作为数据的结束符
	{                     
		sscanf(WiFi_RX_BUF,"%[^,],%[^,],%[^:]",temp,id,len);//截取各段数据，主要是id和数据长度	
		presult = strstr(WiFi_RX_BUF,":");                  //查找冒号。冒号后的是数据
		if( presult != NULL )                               //找到冒号
			sprintf((char *)data,"%s",(presult+1));         //冒号后的数据，复制到data
		WiFi_RxCounter=0;                                   //WiFi接收数据量变量清零                        
		memset(WiFi_RX_BUF,0,WiFi_RXBUFF_SIZE);             //清空WiFi接收缓冲区    
		return 1;                                           //有数据到来
	} 
	else return 0;                                        //无数据到来
}
/*
 *brief：服务器发送数据                           
 *param：databuff：数据缓冲区<2048                
 *param：data_len：数据长度                       
 *param：id：      客户端的连接ID                 
 *param：timeout： 超时时间（10ms的倍数）         
 *retval：错误值                                   
 *        0：无错误                                
 *        1：等待发送数据超时                      
 *        2：连接断开了                           
 *        3：发送数据超时                          
 */
char WiFi_SendData(char id, char *databuff, int data_len, int timeout)
{    
	int i;
	
	WiFi_RxCounter=0;                                 //WiFi接收数据量变量清零                        
	memset(WiFi_RX_BUF,0,WiFi_RXBUFF_SIZE);           //清空WiFi接收缓冲区 
	WiFi_printf("AT+CIPSEND=%d,%d\r\n",id,data_len);  //发送指令	
    while(timeout--)
	{                                 	
		delay_ms(10);                                 //延时10ms
		if(strstr(WiFi_RX_BUF,">"))                   //如果接收到>表示成功
			break;       						      //主动跳出while循环
		printf("%d ",timeout);                     //串口输出现在的超时时间
	}
	if(timeout<=0)return 1;                                   //超时错误，返回1
	else													//没超时，正确
	{                                                            	
		WiFi_RxCounter=0;                                     //WiFi接收数据量变量清零                        
		memset(WiFi_RX_BUF,0,WiFi_RXBUFF_SIZE);               //清空WiFi接收缓冲区 	
		for(i=0;i<data_len;i++)WiFi_printf("%c",databuff[i]); //发送数据	
		while(timeout--)
		{                                     	
			delay_ms(10);                                     
			if(strstr(WiFi_RX_BUF,"SEND OK"))
			{                //如果接受SEND OK，表示发送成功			 
				WiFi_RxCounter=0;                                 //WiFi接收数据量变量清零                        
				memset(WiFi_RX_BUF,0,WiFi_RXBUFF_SIZE);           //清空WiFi接收缓冲区 			
				break;                                        //跳出while循环
			} 
			if(strstr(WiFi_RX_BUF,"link is not valid"))			//如果接受link is not valid，表示连接断开
			{      			
				WiFi_RxCounter=0;                             //WiFi接收数据量变量清零                        
				memset(WiFi_RX_BUF,0,WiFi_RXBUFF_SIZE);       //清空WiFi接收缓冲区 			
				return 2;                                     //返回2
			}
	    }
		if(timeout<=0)return 3;      //超时错误，返回3
		else return 0;	            //正确，返回0
	}	
}

/*
 *brief：连接TCP服务器，并进入透传模式           
 *param：timeout： 超时时间（100ms的倍数）        
 *retval：0：正确  其他：错误                      
 */
char WiFi_Connect_Server(int timeout)
{	
	WiFi_RxCounter=0;                               //WiFi接收数据量变量清零                        
	memset(WiFi_RX_BUF,0,WiFi_RXBUFF_SIZE);         //清空WiFi接收缓冲区   
	WiFi_printf("AT+CIPSTART=\"TCP\",\"%s\",%d\r\n",ServerIP,ServerPort);//发送连接服务器指令
	while(timeout--)
	{                               
		delay_ms(100);                              //延时100ms	
		if(strstr(WiFi_RX_BUF ,"CONNECT"))          //如果接受到CONNECT表示连接成功
			break;                                  //跳出while循环
		if(strstr(WiFi_RX_BUF ,"CLOSED"))           //如果接受到CLOSED表示服务器未开启
			return 1;                               //服务器未开启返回1
		if(strstr(WiFi_RX_BUF ,"ALREADY CONNECTED"))//如果接受到ALREADY CONNECTED已经建立连接
			return 2;                               //已经建立连接返回2
		printf("%d ",timeout);                   //串口输出现在的超时时间  
	}
	printf("\r\n");                        
	if(timeout<=0)return 3;                   //超时错误，返回3
	else                                      //连接成功，准备进入透传
	{
		printf("准备进入透传\r\n");                  
		WiFi_RxCounter=0;                               //WiFi接收数据量变量清零                        
		memset(WiFi_RX_BUF,0,WiFi_RXBUFF_SIZE);         //清空WiFi接收缓冲区     
		WiFi_printf("AT+CIPSEND\r\n");                  //发送进入透传指令
		while(timeout--)
		{                               
			delay_ms(100);                              //延时100ms	
			if(strstr(WiFi_RX_BUF,"\r\nOK\r\n\r\n>"))   //如果成立表示进入透传成功
				break;                          //跳出while循环
			printf("%d ",timeout);           //串口输出现在的超时时间  
		}
		if(timeout<=0)return 4;                 //透传超时错误，返回4	
	}
	return 0;	                                //成功返回0	
}
/*
 *brief：设置AP                                           
 *param：ssid：ssid                               
 *param：password：密码                           
 *param：mode:加密方式                            
 *param：timeout ：超时时间                       
 *retval：0；正确  其他：错误                      
 */
char WiFi_SET_AP(char *ssid, char *password, char mode, int timeout)
{
	WiFi_RxCounter=0;                                                   //WiFi接收数据量变量清零                        
	memset(WiFi_RX_BUF,0,WiFi_RXBUFF_SIZE);                             //清空WiFi接收缓冲区 
	WiFi_printf("AT+CWSAP=\"%s\",\"%s\",4,%d\r\n",ssid,password,mode);  //发送设置AP指令
	while(timeout--)
	{                           
		delay_ms(100);                          //延时100ms
		if(strstr(WiFi_RX_BUF,"OK"))            //如果接收到OK表示指令成功
			break;       						//主动跳出while循环
		printf("%d ",timeout);               //串口输出现在的超时时间
	}
	printf("\r\n");                          
	if(timeout<=0)return 1;                     //如果timeout<=0，说明超时时间到了，也没能收到OK，返回1
	else return 0;		         				//反之，表示正确，说明收到OK，通过break主动跳出while
}
/*
 *brief：连接服务器                               
 *param：无                                       
 *retval：0：正确   其他：错误                     
 */
char WiFi_ConnectServer(void)
{	
	
		char vlua_cmd[512] = {0}; 
		// 正确写法：所有内部双引号用\"转义，删除多余的\,，指令参数完整包裹在字符串内
		snprintf(vlua_cmd, sizeof(vlua_cmd),"AT+MQTTPUB=0,\"$sys/g7sR65eiO1/coolbox/thing/property/post\",\"{\\\"id\\\":\\\"123\\\"\\,\\\"params\\\":{\\\"Humi\\\":{\\\"value\\\":%d}}}\",0,0",Humi);
		if(WiFi_SendCmd(vlua_cmd,500))			 //设置DHCP模式，总计5s超时时间
		{               
			printf("湿度数据上报失败，准备重启\r\n");    
			return 2;                                      //返回2
		}
		else printf("湿度数据上报成功\r\n");
		
	 
		// 正确写法：所有内部双引号用\"转义，删除多余的\,，指令参数完整包裹在字符串内
	//	SHT30_Read_Humiture(&Temp,&Humi);
		memset(vlua_cmd, 0, 512);
		snprintf(vlua_cmd,sizeof(vlua_cmd),"AT+MQTTPUB=0,\"$sys/g7sR65eiO1/coolbox/thing/property/post\",\"{\\\"id\\\":\\\"123\\\"\\,\\\"params\\\":{\\\"Temp\\\":{\\\"value\\\":%d}}}\",0,0",Temp);
		if(WiFi_SendCmd(vlua_cmd,500))			 //设置DHCP模式，总计5s超时时间
		{               
			printf("温度度数据上报失败，准备重启\r\n");    
			return 2;                                      //返回2
		}
		else printf("温度数据上报成功\r\n");
		
	  
		// 正确写法：所有内部双引号用\"转义，删除多余的\,，指令参数完整包裹在字符串内
		Sgp30_Get_Co2_Value(&co2_value);
		// 缓冲区足够容纳指令
		memset(vlua_cmd, 0, 512);
		snprintf(vlua_cmd, sizeof(vlua_cmd),"AT+MQTTPUB=0,\"$sys/g7sR65eiO1/coolbox/thing/property/post\",\"{\\\"id\\\":\\\"123\\\"\\,\\\"params\\\":{\\\"co2_value\\\":{\\\"value\\\":%d}}}\",0,0",co2_value);
		if(WiFi_SendCmd(vlua_cmd,500))			 //设置DHCP模式，总计5s超时时间
		{               
			printf("二氧化碳数据上报失败，准备重启\r\n");    
			return 2;                                      //返回2
		}
		else printf("二氧化碳数据上报成功\r\n");
		
		memset(WiFi_RX_BUF,0,WiFi_RXBUFF_SIZE);
		WiFi_RxCounter = 0;

	
	
                   
	return 0;                                          //正确返回0	

}




char wifi_init(void)
{
		
	printf("准备复位模块\r\n");                     
	if(WiFi_Reset(50))							//复位，总计5s超时时间
	{                                
		printf("复位失败，准备重启\r\n");           
		return 1;                                      //返回1
	}
	else printf("复位成功\r\n");
	
	printf("握手测试\r\n");                     
	if(WiFi_SendCmd("AT",50))							//复位，总计5s超时时间
	{                                
		printf("握手失败\r\n");           
		return 1;                                      //返回1
	}
	else printf("握手成功\r\n");     
	
	printf("准备设置STA模式\r\n");  
	
	if(WiFi_SendCmd("AT+CWMODE=1",50))				 //设置STA模式，总计5s超时时间
	{               
		printf("设置STA模式失败，准备重启\r\n");    
		return 2;                                      //返回2
	}
	else printf("设置STA模式成功\r\n");
	
	printf("开启DHCP模式\r\n");  
	
	if(WiFi_SendCmd("AT+CWDHCP=1,1",50))				 //设置DHCP模式，总计5s超时时间
	{               
		printf("设置DHCP模式失败，准备重启\r\n");    
		return 2;                                      //返回2
	}
	else printf("设置DHCP模式成功\r\n");
	
	printf("连接wifi热点\r\n");  
	
	if(WiFi_SendCmd("AT+CWJAP=\"hoostar2-24G\",\"1234567890\"",1000))				 //设置DHCP模式，总计5s超时时间
	{               
		printf("连接wifi热点失败，准备重启\r\n");    
		return 2;                                      //返回2
	}
	else printf("连接wifi热点成功\r\n");
	
	printf("配置MQTT客户端\r\n");  
	if(WiFi_SendCmd("AT+MQTTUSERCFG=0,1,\"coolbox\",\"g7sR65eiO1\",\"version=2018-10-31&res=products%2Fg7sR65eiO1%2Fdevices%2Fcoolbox&et=2052914641&method=md5&sign=Zrn0VMCFClKTQaNMWZWbtw%3D%3D\",0,0,\"\"",50))				 //设置DHCP模式，总计5s超时时间
	{               
		printf("配置MQTT客户端，准备重启\r\n");    
		return 2;                                      //返回2
	}
	else printf("配置MQTT客户端成功\r\n");
	
	printf("连接到MQTT服务器\r\n");  
	if(WiFi_SendCmd("AT+MQTTCONN=0,\"mqtts.heclouds.com\",1883,1",1000))				 //设置DHCP模式，总计5s超时时间
	{               
		printf("连接MQTT服务器失败，准备重启\r\n");    
		return 2;                                      //返回2
	}
	else printf("连接MQTT服务器成功\r\n");
	
//	printf("订阅数据点上报结果通知消息\r\n");  
//	if(WiFi_SendCmd("AT+MQTTSUB=0,\"$sys/g7sR65eiO1/coolbox/thing/property/post/reply\",1",50))				 //设置DHCP模式，总计5s超时时间
//	{               
//		printf("订阅数据失败，准备重启\r\n");    
//		return 2;                                      //返回2
//	}
//	else printf("订阅数据成功\r\n");
	
	
	printf("订阅下行数据通知消息\r\n");  
	if(WiFi_SendCmd("AT+MQTTSUB=0,\"$sys/g7sR65eiO1/coolbox/thing/property/set\",1",50))				 //设置DHCP模式，总计5s超时时间
	{               
		printf("订阅数据失败，准备重启\r\n");    
		return 2;                                      //返回2
	}
	else printf("订阅数据成功\r\n");
	
	
	return 0;  
	
	
}










//接受平台数据

char WiFi_ack(void)
{

	if(strstr(WiFi_RX_BUF,"+MQTTSUBRECV"))
	{
		char str[256] = {0};
		memcpy(str,WiFi_RX_BUF,256);
		printf("下发指令:%s\r\n",str);
		int id = -1;
		int temp_threshold = -1;
			// 1. 定位"id":"的起始位置
		char *id_start = strstr(str, "\"id\":\"");
		if (id_start != NULL)
		{
			// 移动指针到id数值的起始位置（跳过"id":"）
			id_start += strlen("\"id\":\"");
			// 将字符串转换为整数（atoi会自动跳过空格，直到遇到非数字字符停止）
			id = atoi(id_start);
		}
		else
		{
			printf("未找到id字段\n");
		}

		// -------------------------- 步骤2：提取温度阈值（5） --------------------------
		// 1. 定位"Temp_threshold":的起始位置
		char *temp_start = strstr(str, "\"Temp_threshold\":");
		if (temp_start != NULL)
		{
			// 移动指针到阈值数值的起始位置（跳过"Temp_threshold":）
			temp_start += strlen("\"Temp_threshold\":");
			// 将字符串转换为整数
			temp_threshold = atoi(temp_start);
		}
		else
		{
			printf("未找到Temp_threshold字段\n");
		}

		// 打印提取结果（调试用）
		printf("提取的id：%d\n", id);
		printf("提取的温度阈值：%d\n", temp_threshold);
		memset(str, 0, 256);
		snprintf(str, sizeof(str),"AT+MQTTPUB=0,0,\"$sys/g7sR65eiO1/coolbox/thing/property/set_reply\",\"{\"id\":\"%d\",\"code\":200,\"msg\":\"succ\"}\",0,0",id);
		printf("响应指令:%s\r\n",str);
		if(WiFi_SendCmd(str,50))				 //设置DHCP模式，总计5s超时时间
		{   
			u4_printf(Uart4_RxBuff);            
			printf("响应失败，准备重启\r\n");    
			return 2;                                      //返回2
		}
		else
		printf("响应成功\r\n");
	}
	else
		printf("没有下发\r\n");


	return 0;  

}

