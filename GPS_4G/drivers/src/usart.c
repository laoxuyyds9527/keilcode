#include "usart.h"

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
