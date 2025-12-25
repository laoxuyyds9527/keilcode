#include "usart_drv.h"
#include "esp8266.h"

char Uart4_RxCompleted = 0;            //定义一个变量 0：表示接收未完成 1：表示接收完成 
unsigned int Uart4_RxCounter = 0;      //定义一个变量，记录串口2总共接收了多少字节的数据
char Uart4_RxBuff[UART4_RXBUFF_SIZE]; //定义一个数组，用于保存串口2接收到的数据 

/**
 * @brief USART1配置
 * @param baudrate:波特率设置
 * @retval 无
 */
void USART1_Init(u32 baudrate)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
//	NVIC_InitTypeDef NVIC_InitStruct;

	//开启外设时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1 | RCC_APB2Periph_AFIO, ENABLE);

	//GPIO引脚初始化
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	//USART配置
	USART_InitStruct.USART_BaudRate = baudrate;  //波特率,由参数传入
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  //无硬件流控
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;  //全双工模式
	USART_InitStruct.USART_Parity = USART_Parity_No;  //不校验
	USART_InitStruct.USART_StopBits = USART_StopBits_1;  //1位停止位
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;  //8字长
	USART_Init(USART1, &USART_InitStruct);

	//使能指定的USART中断
//	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	//NVIC配置
//	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;  //配置中断通道
//	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;  //使能中断
//	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;  //抢占优先级
//	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;  //响应优先级
//	NVIC_Init(&NVIC_InitStruct);

	//打开串口
	USART_Cmd(USART1, ENABLE);
}



void UART4_MyConfig(u32 baudrate)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	USART_InitTypeDef  USART_InitStruct;
	NVIC_InitTypeDef NVIC_InitStructure;
	/*1、打开时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOC , ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);
	/*2、配置GPIO*/
	/*PA2:Tx*/
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;//复用推挽
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStruct);

	/*PA3:Rx*/
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOC, &GPIO_InitStruct);

	/*初始化串口*/
	USART_InitStruct.USART_BaudRate = baudrate;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_Init(UART4, &USART_InitStruct);
	
	USART_ClearFlag(UART4, USART_FLAG_RXNE);
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);          //开启接收中断
    NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;       //设置串口2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0; //抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//子优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//中断通道使能
	NVIC_Init(&NVIC_InitStructure);	                

	/*使能串口*/
	USART_Cmd(UART4, ENABLE);

	/*清除标志位*/
//	USART_ClearFlag(USART2, USART_FLAG_TC | USART_FLAG_RXNE);
}






















//printf重定向,重写fputc
int fputc(int c,FILE *stream)
{
	USART_SendData(USART1, c);
	
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC) != SET);  //防止发送过快导致顺序错乱
	
	return c;
}


/**
 * @brief USART1中断服务函数
 * @param 无
 * @retval 无
 */
void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
		u16 data;
		
		//清除中断标志位,确保不会重复进入
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);

		//将接收到的数据发回给电脑
		data = USART_ReceiveData(USART1);

		USART_SendData(USART1, data);
	}
}


















void UART4_IRQHandler(void)   
{                      
	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)  //如果USART_IT_RXNE标志置位，表示有数据到了，进入if分支
	{  
		if(Connect_flag==0)							 //如果Connect_flag等于0，当前还没有连接服务器，处于指令配置状态
		{                               
			if(UART4->DR)			//处于指令配置状态时，非零值才保存到缓冲区
			{                                 	
				Uart4_RxBuff[Uart4_RxCounter]=UART4->DR; //保存到缓冲区	
				Uart4_RxCounter++;                        //每接收1个字节的数据，Usart2_RxCounter加1，表示接收的数据总量+1 
			}		
		}
		else			//反之Connect_flag等于1，连接上服务器了	
		{		                                        
			Uart4_RxBuff[Uart4_RxCounter] = UART4->DR;   //把接收到的数据保存到Usart2_RxBuff中				
			if(Uart4_RxCounter == 0)		//如果Usart2_RxCounter等于0，表示是接收的第1个数据，进入if分支	
			{    								
				TIM_Cmd(TIM4,ENABLE); 
			}
			else		//else分支，表示果Usart2_RxCounter不等于0，不是接收的第一个数据
			{                        					
				TIM_SetCounter(TIM4,0);  
			}	
			Uart4_RxCounter++;         				    //每接收1个字节的数据，Usart2_RxCounter加1，表示接收的数据总量+1 
		}		
	}
} 


__align(8) char UART4_TxBuff[UART4_TXBUFF_SIZE];  

void u4_printf(char* fmt,...) 
{  
	unsigned int i,length;
	
	va_list ap;
	memset(UART4_TxBuff,'\0',UART4_TXBUFF_SIZE);
	va_start(ap,fmt);
	vsprintf(UART4_TxBuff,fmt,ap);
	va_end(ap);	
	
	length=strlen((const char*)UART4_TxBuff);		
	while((UART4->SR&0X40)==0);
	for(i = 0;i < length;i++)
	{			
		UART4->DR = UART4_TxBuff[i];
		while((UART4->SR&0X40)==0);	
	}	
}

