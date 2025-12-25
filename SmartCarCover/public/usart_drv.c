#include "usart_drv.h"


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
