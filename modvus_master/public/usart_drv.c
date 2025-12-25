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
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
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


///**
// * @brief USART1中断服务函数
// * @param 无
// * @retval 无
// */
//void UART5_IRQHandler(void)
//{
//	if(USART_GetITStatus(UART5, USART_IT_RXNE) == SET)
//	{
//		u16 data;
//		
//		//清除中断标志位,确保不会重复进入
//		USART_ClearITPendingBit(UART5, USART_IT_RXNE);

//		//将接收到的数据发回给电脑
//		data = USART_ReceiveData(UART5);

//		USART_SendData(UART5, data);
//	}
//}









#define RX_BUF_SIZE 256
uint8_t Uart_Rx_Buff[RX_BUF_SIZE] = {0};
uint16_t uart_rx_count = 0;
uint8_t time_out_count = 0;
uint8_t uart_recv_start_flag = 0;
uint8_t uart_recv_end_flag = 0;

//串口的中断
void USART4_Init(uint32_t baud)
{
	//开启外设时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO , ENABLE)	;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);

	//GPIO初始化
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init( GPIOC, & GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( GPIOC, & GPIO_InitStruct);
	//初始化UART4
	USART_InitTypeDef  USART_InitStruct;
	USART_InitStruct.USART_BaudRate = baud;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStruct.USART_Parity = USART_Parity_Even;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_9b;
	USART_Init( UART4, &USART_InitStruct);

	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
	//配置NVIC
	NVIC_InitTypeDef  NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = UART4_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;//抢占优先级
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;//响应优先级
	NVIC_Init(& NVIC_InitStruct);

	//清除标志位

	USART_ClearFlag(UART4, USART_FLAG_TC | USART_FLAG_RXNE);
	USART_Cmd(UART4, ENABLE);
}





/**
 * @brief USART4中断服务函数
 * @param 无
 * @retval 无
 */
void UART4_IRQHandler(void)
{
	if(USART_GetITStatus(UART4, USART_IT_RXNE)  == SET)
		{
			USART_ClearITPendingBit(UART4, USART_FLAG_RXNE);
			time_out_count = 0;
			uart_recv_start_flag = 1;
			if(uart_rx_count >= RX_BUF_SIZE) uart_rx_count = 0;
			Uart_Rx_Buff[uart_rx_count ++] = USART_ReceiveData(UART4);
		}
}

void RS485_SendByte(USART_TypeDef* USARTx,uint8_t data)
{
	USART_SendData(USARTx,data);
	while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)!= SET);
	USART_ClearFlag(USARTx, USART_FLAG_TC);
	
}

void RS485_SendBuff(USART_TypeDef* USARTx,uint8_t *buff,uint16_t len)
{
	for(uint16_t i = 0;i<len;i++)
		{
			USART_SendData(USARTx,buff[i]);
			while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)!= SET);
			USART_ClearFlag(USARTx, USART_FLAG_TC);
		}
}


void Modbus_ReadCoils(uint8_t slave_id,uint16_t reg_addr,uint16_t reg_num)
{
	uint8_t buf[8] = {0};
	buf[0] = slave_id;//从机地址
	buf[1] = 0x01;	//功能码
	buf[2] = reg_addr>> 8;
	buf[3] = reg_addr;
	buf[4] = reg_num>>8;
	buf[5] = reg_num;

	uint16_t crc = Get_CRC16(buf, 6);
	buf[6] = crc;
	buf[7] = crc>>8;
	RS485_SendBuff(UART4, buf, 8);
}


void Modbus_HoldingReg(uint8_t slave_id,uint16_t reg_addr,uint16_t reg_num)
{
	uint8_t buf[8] = {0};
	buf[0] = slave_id;//从机地址
	buf[1] = 0x03;	//功能码
	buf[2] = reg_addr>> 8;
	buf[3] = reg_addr;
	buf[4] = reg_num>>8;
	buf[5] = reg_num;

	uint16_t crc = Get_CRC16(buf, 6);
	buf[6] = crc;
	buf[7] = crc>>8;
	RS485_SendBuff(UART4, buf, 8);
}

void MOdbus_WriteHoldingReg(uint8_t slave_id,uint16_t reg_addr,uint16_t reg_num,uint16_t *data)
{
	uint8_t buf[32] = {0};
	uint8_t len = 0;
	uint8_t nbyte = 0;

	buf[len++] = slave_id;
	buf[len++] = 0x10;
	buf[len++] = reg_addr>>8;
	buf[len++] = reg_addr;
	buf[len++] = reg_num>>8;
	buf[len++] = reg_num;

	nbyte = reg_num * 2;
	buf[len++] = nbyte;
	for(uint8_t i=0;i<reg_num;i++)
		{
			buf[len++] = data[i]>>8;
			buf[len++] = data[i];
		}
	uint16_t crc = Get_CRC16(buf,len);
	buf[len++] = crc;
	buf[len++] = crc>>8;

	RS485_SendBuff(UART4,buf,len);
}
