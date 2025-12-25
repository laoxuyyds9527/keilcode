#include "usart.h"

void UART5_init(u16 buad)
{
	GPIO_InitType  GPIO_InitStruct;
	USART_InitType  USART_InitStruct;
	//开启外设时钟
	RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_AFIO|RCC_APB2_PERIPH_GPIOB,ENABLE);
	RCC_EnableAPB1PeriphClk (RCC_APB1_PERIPH_UART5, ENABLE);
	//初始化tx
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStruct.Pin=GPIO_PIN_13;
	GPIO_InitPeripheral (GPIOB, &GPIO_InitStruct);
	//初始化rx
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStruct.Pin=GPIO_PIN_14;
	GPIO_InitPeripheral (GPIOB, &GPIO_InitStruct);

	//重映射引脚
	GPIO_ConfigPinRemap(GPIO_RMP1_UART5,ENABLE);

	//配置串口
	USART_InitStruct.BaudRate=buad;
	USART_InitStruct.HardwareFlowControl=USART_HFCTRL_NONE;
	USART_InitStruct.Mode= USART_MODE_RX| USART_MODE_TX;  
	USART_InitStruct.Parity=USART_PE_NO;
	USART_InitStruct.StopBits=USART_STPB_1;
	USART_InitStruct.WordLength=USART_WL_8B;
	USART_Init (UART5,&USART_InitStruct);

	//打开串口
	USART_Enable (UART5,ENABLE);
}




void UART5_tx(uint16_t Data)
{
	if(USART_GetFlagStatus (UART5,USART_FLAG_TXDE)==SET)
		{	
			USART_ClrFlag (UART5,USART_FLAG_TXDE);
			USART_SendData (UART5,Data);
		}


}
u16 UART5_rx(uint16_t data)
{
	if(USART_GetFlagStatus (UART5,USART_FLAG_RXDNE)==SET)
		{
			USART_ClrFlag (UART5,USART_FLAG_RXDNE);
			data=USART_ReceiveData (UART5);
		}
	return data;

}







//printf重定向,重写fputc
int fputc(int c,FILE *stream)
{
	USART_SendData(UART5, c);
	
	while(USART_GetFlagStatus(UART5,USART_FLAG_TXC) != SET);  //防止发送过快导致顺序错乱
	
	return c;
}













