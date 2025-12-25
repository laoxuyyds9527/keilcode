#include "usart.h"


//@brief usart1的配置
//@param
//@retval 没有


void Usart_init(u32 baudrate)
{

	NVIC_InitTypeDef NVIC_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	GPIO_InitTypeDef  GPIO_InitStruct;
	
	//1开启外部时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_USART1|RCC_APB2Periph_AFIO, ENABLE);
	//2GPIO的初始化
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF_PP; 
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU; 
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	//3usart的配置
	USART_InitStruct.USART_BaudRate=baudrate;
	USART_InitStruct.USART_WordLength=USART_WordLength_8b;
	USART_InitStruct.USART_StopBits=USART_StopBits_1;
	USART_InitStruct.USART_Parity=USART_Parity_No;
	USART_InitStruct.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
	USART_Init(USART1,&USART_InitStruct);

	//配置usart的中断

	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);


	//NVIC的配置
	NVIC_InitStruct.NVIC_IRQChannel=USART1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=0;
	NVIC_Init(&NVIC_InitStruct);
	
	
	
	
	
	//4打开串口
	USART_Cmd(USART1, ENABLE);
	

}



//对printf重定向，重写发putc
	int fputc(int c,FILE*stream)
		{
			USART_SendData(USART1, c);
			while(USART_GetFlagStatus(USART1, USART_FLAG_TC)!=SET);//防止发送过快导致覆盖了前面的数据
			return c;

		}
		
		
//usart中断服务
	void USART1_IRQHander(void)
		{
		if(USART_GetITStatus(USART1,USART_IT_RXNE )!=SET)
			{
				u16 data;
				
				USART_ClearITPendingBit(USART1,USART_IT_RXNE);
				data=USART_ReceiveData(USART1);	

				USART_SendData(USART1,data);
			}


		}


		