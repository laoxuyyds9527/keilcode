/********************************************************************************
* @file    		uart2.c
* @function     串口usart2 通信驱动函数
* @brief 		用3.5.0版本库建的工程模板
*               串口2用于与 fat 核心板通信
*               USART2 : -------   核心板
*			    PA2(TXD) <-------> RXD
*		        PA3(RXD) <-------> TXD
*               GND      <-------> GND
* @attention	该例程仅作为参考，如需实际开发，请根据实际需求更改
* @version 		V1.1
* @company  	深圳市飞思创电子科技有限公司
* @website  	http://www.freestrong.com
* @tel			0755-86528386
* @author       freestrong
* @date    		2020/09/20
********************************************************************************/
#include <string.h>
#include "uart.h"
#include "fat_core.h"

//加入以下代码,支持printf函数,而不需要选择use MicroLIB
#pragma import(__use_no_semihosting)
//定义_sys_exit()以避免使用半主机模式
void _sys_exit(int x)
{
	x = x;
}
//标准库需要的支持函数
struct __FILE
{
	int handle;
};
FILE __stdout;



/**
 * @description: uart5初始化，用于调试信息打印
 * @param None
 * @return None
 */
void UART5_init(u32 baud)
{
	//外设时钟初始化
	RCC_EnableAPB1PeriphClk(RCC_APB1_PERIPH_UART5, ENABLE);
	RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_AFIO | RCC_APB2_PERIPH_GPIOB, ENABLE);

	//GPIO初始化 TX
	GPIO_InitType  GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP ;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.Pin = GPIO_PIN_13;
	GPIO_InitPeripheral(GPIOB, & GPIO_InitStruct);

	//GPIO初始化 RX
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING ;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.Pin = GPIO_PIN_14;
	GPIO_InitPeripheral(GPIOB, & GPIO_InitStruct);


	GPIO_ConfigPinRemap(GPIO_RMP1_UART5,ENABLE);

	//UART5初始化
	USART_InitType USART_InitStruct;
	USART_InitStruct.BaudRate = baud;
	USART_InitStruct.HardwareFlowControl = USART_HFCTRL_NONE;
	USART_InitStruct.Mode =  USART_MODE_RX | USART_MODE_TX;
	USART_InitStruct.Parity = USART_PE_NO;
	USART_InitStruct.StopBits = USART_STPB_1;
	USART_InitStruct.WordLength = USART_WL_8B;
	USART_Init(UART5, &USART_InitStruct);
    
	USART_ClrFlag(UART5, USART_FLAG_TXC);
	USART_ClrFlag(UART5, USART_FLAG_RXDNE);
    USART_Enable(UART5, ENABLE);
	
}

//printf重定向
int fputc(int c,FILE *stream)
{
	USART_SendData(UART5, c);
	while(USART_GetFlagStatus(UART5,USART_FLAG_TXC) != SET); 
	USART_ClrFlag(UART5, USART_FLAG_RXDNE);
	return c;
}

/**
 * @description: UART2初始化，用于跟模组通信
 * @param  None
 * @return None
 */
void uart2_init(void)
{

	GPIO_InitType  GPIO_InitStruct;
	USART_InitType USART_InitStruct;
	NVIC_InitType NVIC_InitStruct;
	RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOB, ENABLE);
	RCC_EnableAPB1PeriphClk(RCC_APB1_PERIPH_USART2, ENABLE);

	GPIO_ConfigPinRemap(GPIO_RMP_SW_JTAG_NO_NJTRST,ENABLE);
	GPIO_ConfigPinRemap(GPIO_RMP3_USART2,ENABLE);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP ;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.Pin = GPIO_PIN_4;
	GPIO_InitPeripheral(GPIOB, & GPIO_InitStruct);

	//GPIO初始化 RX
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING ;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.Pin = GPIO_PIN_5;
	GPIO_InitPeripheral(GPIOB, & GPIO_InitStruct);

	USART_InitStruct.BaudRate = 115200;
	USART_InitStruct.HardwareFlowControl = USART_HFCTRL_NONE;
	USART_InitStruct.Mode =  USART_MODE_RX | USART_MODE_TX;
	USART_InitStruct.Parity = USART_PE_NO;
	USART_InitStruct.StopBits = USART_STPB_1;
	USART_InitStruct.WordLength = USART_WL_8B;
	USART_Init(USART2, &USART_InitStruct);

	NVIC_InitStruct.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;

	NVIC_Init(&NVIC_InitStruct);

	USART_ConfigInt(USART2, USART_INT_RXDNE, ENABLE);
	USART_Enable(USART2, ENABLE);
}

/**
 * @description: UART2发送一个字节函数
 * @param None
 * @return None
 */
void uart2_send_byte(uint8_t data)
{
	USART_SendData(USART2,	data);
	while (USART_GetFlagStatus(USART2, USART_FLAG_TXC) == RESET)
	{
	}
}


void USART2_IRQHandler(void)
{

  if (USART_GetIntStatus(USART2, USART_INT_RXDNE) != RESET) // 接收中断
  {
  	uint8_t ch = USART2->DAT;

		  fat_uart_recv_proc(ch);
		
		
    USART_ClrIntPendingBit(USART2, USART_INT_RXDNE); // 清除标志位
  }
}
