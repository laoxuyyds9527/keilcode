#include "system.h"
#include "usart_drv.h"	

char rxdatabufer;
u16 point1 = 0;

_SaveData Save_Data;

	
char USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART_RX_STA=0;       //接收状态标记	  
  
void uart_init(u32 bound)
{
    //GPIO端口设置
    GPIO_InitType  GPIO_InitStruct;
	USART_InitType USART_InitStruct;
	NVIC_InitType NVIC_InitStruct;
	 
	RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_USART1|RCC_APB2_PERIPH_GPIOB, ENABLE);	//使能USART1，GPIOA时钟
     //USART1_TX   PA.9
    GPIO_InitStruct.Pin = GPIO_PIN_6; //PA.9
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
    GPIO_InitPeripheral(GPIOB, & GPIO_InitStruct);
   
    //USART1_RX	  PA.10
	GPIO_InitStruct.Pin = GPIO_PIN_7;
//     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_InitPeripheral(GPIOB, & GPIO_InitStruct);  

   //Usart1 NVIC 配置

    NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=2 ;//抢占优先级3
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;		//子优先级3
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStruct);	//根据指定的参数初始化VIC寄存器
  
   //USART 初始化设置

	USART_InitStruct.BaudRate = bound;//一般设置为9600;
	USART_InitStruct.WordLength = USART_WL_8B;//字长为8位数据格式
	USART_InitStruct.StopBits =  USART_STPB_1;//一个停止位
	USART_InitStruct.Parity = USART_PE_NO;//无奇偶校验位
	USART_InitStruct.HardwareFlowControl = USART_HFCTRL_NONE;//无硬件数据流控制
	USART_InitStruct.Mode = USART_MODE_RX | USART_MODE_TX;	//收发模式
    USART_Init(USART1, &USART_InitStruct); //初始化串口
	
	
	//功能重映射
	GPIO_ConfigPinRemap(GPIO_RMP_USART1,ENABLE);
    USART_ConfigInt(USART1, USART_INT_RXDNE, ENABLE);//开启中断
    USART_Enable(USART1, ENABLE);                    //使能串口 
	CLR_Buf();//清空缓存
}

void USART1_IRQHandler(void)                	//串口1中断服务程序
{
	u8 Res;	
	if(USART_GetIntStatus(USART1, USART_INT_RXDNE) != RESET) 
	{
		
		Res =USART_ReceiveData(USART1);//(USART1->DR);	//读取接收到的数据
//		printf("%c\r\n",Res);
		if(Res == '$')
		{
			point1 = 0;	
		}
		 USART_RX_BUF[point1++] = Res;

		if(USART_RX_BUF[0] == '$' && USART_RX_BUF[4] == 'M' && USART_RX_BUF[5] == 'C')			//确定是否收到"GPRMC/GNRMC"这一帧数据
		{
			if(Res == '\n')									   
			{
//				printf("USART_RX_BUF:%s\r\n",USART_RX_BUF);
				memset(Save_Data.GPS_Buffer, 0, GPS_Buffer_Length);      //清空
				memcpy(Save_Data.GPS_Buffer, USART_RX_BUF, point1); 	//保存数据
				Save_Data.isGetData = true;
				point1 = 0;
				memset(USART_RX_BUF, 0, USART_REC_LEN);      //清空				
			}	
					
		}
		
		if(point1 >= USART_REC_LEN)
		{
			point1 = USART_REC_LEN;
		}		 
   } 

}


u8 Hand(char *a)                   // 串口命令识别函数
{ 
    if(strstr(USART_RX_BUF,a)!=NULL)
	    return 1;
	else
		return 0;
}

void CLR_Buf(void)                           // 串口缓存清理
{
	memset(USART_RX_BUF, 0, USART_REC_LEN);      //清空
  point1 = 0;                    
}

void clrStruct()
{
	Save_Data.isGetData = false;
	Save_Data.isParseData = false;
	Save_Data.isUsefull = false;
	memset(Save_Data.GPS_Buffer, 0, GPS_Buffer_Length);      //清空
	memset(Save_Data.UTCTime, 0, UTCTime_Length);
	memset(Save_Data.latitude, 0, latitude_Length);
	memset(Save_Data.N_S, 0, N_S_Length);
	memset(Save_Data.longitude, 0, longitude_Length);
	memset(Save_Data.E_W, 0, E_W_Length);
	
}

	

