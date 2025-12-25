#include "usart_drv.h"

/*
 * @brief:串口1的初始化
 * @param:baudrate波特率大小
 * @retval:无
 */
void Usart_MyConfig(u32 baudrate)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef  USART_InitStruct;
	//外设时钟使能
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO | RCC_APB2Periph_USART1,ENABLE);

	//GPIO配置
	//PA9:Tx
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;//复用推挽输出
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	//PA10:Rx
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	//配置串口
	USART_InitStruct.USART_BaudRate = baudrate;//波特率
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//硬件流控
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//输入输出模式
	USART_InitStruct.USART_Parity = USART_Parity_No;//偶校验
	USART_InitStruct.USART_StopBits = USART_StopBits_1;//停止位
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;//字长设置
	USART_Init(USART1, &USART_InitStruct);

	//清空一些标志位
	USART_ClearFlag(USART1, USART_FLAG_TC | USART_FLAG_RXNE);

	//外设使能
	USART_Cmd(USART1, ENABLE);
}


void USART2_Config(u32 baudrate)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef  USART_InitStruct;
	//外设时钟使能
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO,ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	//GPIO配置
	//PA9:Tx
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;//复用推挽输出
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	//PA10:Rx
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	//配置串口
	USART_InitStruct.USART_BaudRate = baudrate;//波特率
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//硬件流控
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//输入输出模式
	USART_InitStruct.USART_Parity = USART_Parity_No;//偶校验
	USART_InitStruct.USART_StopBits = USART_StopBits_1;//停止位
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;//字长设置
	USART_Init(USART2, &USART_InitStruct);

	//清空一些标志位
	USART_ClearFlag(USART2, USART_FLAG_TC | USART_FLAG_RXNE);

	//外设使能
	USART_Cmd(USART2, ENABLE);
}



//printf的重定向
int fputc(int ch,FILE *p)
{
	USART_SendData(USART1,(u8)ch);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC) != SET);
	
	return ch;
}


__align(8) char Usart1_TxBuff[USART1_TXBUFF_SIZE];  

void u1_printf(char* fmt,...) 
{  
	unsigned int i,length;
	
	va_list ap;
	memset(Usart1_TxBuff,'\0',USART1_TXBUFF_SIZE);
	va_start(ap,fmt);
	vsprintf((char *)Usart1_TxBuff,fmt,ap);
	va_end(ap);	
	
	length=strlen((const char*)Usart1_TxBuff);		
	while((USART1->SR&0X40)==0);
	for(i = 0;i < length;i++)
	{			
		USART1->DR = Usart1_TxBuff[i];
		while((USART1->SR&0X40)==0);	
//		while(USART_GetFlagStatus(USART1,USART_FLAG_TC) == RESET);
//		USART_SendData(USART1,(uint8_t)Usart1_TxBuff[i]);
	}	
}
