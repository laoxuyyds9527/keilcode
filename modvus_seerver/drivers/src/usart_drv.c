#include "usart_drv.h"

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
	USART_InitStruct.USART_Parity = USART_Parity_No;//不校验
	USART_InitStruct.USART_StopBits = USART_StopBits_1;//停止位
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;//字长设置
	USART_Init(USART1, &USART_InitStruct);

	//清空一些标志位
	USART_ClearFlag(USART1, USART_FLAG_TC | USART_FLAG_RXNE);

	//外设使能
	USART_Cmd(USART1, ENABLE);
}

//printf的重定向
int fputc(int ch,FILE *p)
{
	USART_SendData(USART1,(u8)ch);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC) != SET);
	
	return ch;
}


void USART4_Init(uint32_t baud)
{
    //开启外设时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOC,ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);
    
    //初始化GPIO
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Pin = RS485_TX_PIN;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(RS485_TX_PORT,&GPIO_InitStruct);
    
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStruct.GPIO_Pin = RS485_RX_PIN;
    GPIO_Init(RS485_RX_PORT,&GPIO_InitStruct);
    
    //初始化UART4
    USART_InitTypeDef USART_InitStruct;
    USART_InitStruct.USART_BaudRate = baud;
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStruct.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
    USART_InitStruct.USART_Parity = USART_Parity_Even;
    USART_InitStruct.USART_StopBits = USART_StopBits_1;
    USART_InitStruct.USART_WordLength = USART_WordLength_9b;
    USART_Init(RS485_USART,&USART_InitStruct);
    
    //使能接收中断
    USART_ITConfig(RS485_USART,USART_IT_RXNE,ENABLE);
    
    //配置NVIC
    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = UART4_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_Init(&NVIC_InitStruct);
    
    //清除标志位
    USART_ClearFlag(RS485_USART,USART_FLAG_TC|USART_FLAG_RXNE);
    
    //使能串口
    USART_Cmd(RS485_USART,ENABLE);    
        
}


void USART2_Init(uint32_t baud)
{
    //开启外设时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOG,ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
    
    //初始化GPIO
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Pin = RS485_TX_PIN;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(RS485_TX_PORT,&GPIO_InitStruct);
    
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStruct.GPIO_Pin = RS485_RX_PIN;
    GPIO_Init(RS485_RX_PORT,&GPIO_InitStruct);
    
#if !RS485_USE_UART4    
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Pin = RS485_RX_EN_PIN;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(RS485_RX_EN_PORT,&GPIO_InitStruct);
#endif

    //初始化UART4
    USART_InitTypeDef USART_InitStruct;
    USART_InitStruct.USART_BaudRate = baud;
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStruct.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
    USART_InitStruct.USART_Parity = USART_Parity_Even;
    USART_InitStruct.USART_StopBits = USART_StopBits_1;
    USART_InitStruct.USART_WordLength = USART_WordLength_9b;
    USART_Init(RS485_USART,&USART_InitStruct);
    
    //使能接收中断
    USART_ITConfig(RS485_USART,USART_IT_RXNE,ENABLE);
    
    //配置NVIC
    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_Init(&NVIC_InitStruct);
    
    //清除标志位
    USART_ClearFlag(RS485_USART,USART_FLAG_TC|USART_FLAG_RXNE);
    
    //使能串口
    USART_Cmd(RS485_USART,ENABLE); 

    //默认设置为接收模式
    RS485_RX_EN;
        
}


void RS485_SendByte(USART_TypeDef* USARTx,uint8_t data)
{
    //设置为发送模式
    RS485_TX_EN;
    
    USART_SendData(USARTx,data);
    while(USART_GetFlagStatus(USARTx,USART_FLAG_TC)!=SET);
    USART_ClearFlag(USARTx,USART_FLAG_TC);
    
    //设置为接收模式
    RS485_RX_EN;
}


void RS485_SendBuff(USART_TypeDef* USARTx,uint8_t *buff,uint16_t len)
{
    //设置为发送模式
    RS485_TX_EN;
    
    for(uint16_t i=0;i<len;i++)
    {
        USART_SendData(USARTx,buff[i]);
        while(USART_GetFlagStatus(USARTx,USART_FLAG_TC)!=SET);
        USART_ClearFlag(USARTx,USART_FLAG_TC);
    }
    
    //设置为接收模式
    RS485_RX_EN;
}


uint8_t Uart_Rx_Buff[RX_BUF_SIZE] = {0};
uint16_t uart_rx_count = 0;

uint8_t uart_recv_start_flag = 0;
uint8_t uart_recv_end_flag = 0;
uint8_t time_out_count = 0;

void UART4_IRQHandler(void)
{
    if(USART_GetITStatus(UART4,USART_IT_RXNE)!=RESET)
    {
        USART_ClearITPendingBit(UART4,USART_IT_RXNE);
       
        if(uart_recv_start_flag == 0)
            uart_recv_start_flag =1;//开始接收数据
        time_out_count = 0; //清零超时计数
        
        if(uart_recv_start_flag && !uart_recv_end_flag)
        {
            if(uart_rx_count >= RX_BUF_SIZE) uart_rx_count = 0;
            Uart_Rx_Buff[uart_rx_count++] = USART_ReceiveData(UART4);
        }
        
        
    }
}

void USART2_IRQHandler(void)
{
    if(USART_GetITStatus(USART2,USART_IT_RXNE)!=RESET)
    {
        USART_ClearITPendingBit(USART2,USART_IT_RXNE);
        RS485_SendByte(USART2,USART_ReceiveData(USART2));
    }
}








