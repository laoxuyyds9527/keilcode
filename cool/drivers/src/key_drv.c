#include "key_drv.h"

u8 key_val;

/**
 * @brief 按键初始化
 * @param 无
 * @retval 无
 */
void Key_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	EXTI_InitTypeDef EXTI_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;

	//配置时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);

	//将对应的GPIO口映射到指定的EXTI线路上
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource1);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource4);

	//配置引脚
	//PA0
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD;  //下拉输入
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	//PA1 PA4
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;  //上拉输入
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_4;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	//EXTI线路初始化
	EXTI_InitStruct.EXTI_Line = EXTI_Line0;  //中断线
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;  //中断线使能
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;  //中断模式
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;  //上升沿触发
	EXTI_Init(&EXTI_InitStruct);

	EXTI_InitStruct.EXTI_Line = EXTI_Line1;  //中断线
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;  //中断线使能
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;  //中断模式
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;  //下升沿触发
	EXTI_Init(&EXTI_InitStruct);

	EXTI_InitStruct.EXTI_Line = EXTI_Line4;  //中断线
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;  //中断线使能
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;  //中断模式
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;  //下升沿触发
	EXTI_Init(&EXTI_InitStruct);

	//NVIC初始化
	NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn;  //中断通道
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;  //使能
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStruct);

	NVIC_InitStruct.NVIC_IRQChannel = EXTI1_IRQn;  //中断通道
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;  //使能
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStruct);

	NVIC_InitStruct.NVIC_IRQChannel = EXTI4_IRQn;  //中断通道
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;  //使能
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStruct);
}


/**
 * @brief 按键扫描
 * @param mode:0单次检测模式，1连续检测模式
 * @retval 被按下的按键
 */
u8 Key_Scan(u8 mode)
{
	u8 key_val = KEY_NO;
	static u8 key_flag = 1;  //用来表示按键有没有被按下0表示被按下 1表示没有按下
	if(mode == 1)
		key_flag = 1;
	
	if((PAin(0) || !PAin(1) || !PAin(4)) && key_flag)
	{
		delay_ms(20);
		key_flag = 0;
		if(PAin(0))
		{
			key_val = KEY_1;
		}
		if(!PAin(1))
		{
			key_val = KEY_2;
		}
		if(!PAin(4))
		{
			key_val = KEY_3;
		}
	}
	else if(!PAin(0) && PAin(1) && PAin(4))
	{
		key_flag = 1;
	}
	return key_val;
}


/**
 * @brief 按键1的中断服务函数
 * @param 无
 * @retval 无
 */
void EXTI0_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line0) == SET)
	{
		EXTI_ClearITPendingBit(EXTI_Line0);

		delay_ms(20);
		if(PAin(0))
		{
			key_val = KEY_1;
			fire_off=1;
			printf("KEY_1按下\n");
		}
	}
}

/**
 * @brief 按键2的中断服务函数
 * @param 无
 * @retval 无
 */
void EXTI1_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line1) == SET)
	{
		EXTI_ClearITPendingBit(EXTI_Line1);

		delay_ms(20);
		if(!PAin(1))
		{
			key_val = KEY_2;
			Flame_threshold-=100;
			printf("KEY_2按下\n");
		}
	}
}

/**
 * @brief 按键3的中断服务函数
 * @param 无
 * @retval 无
 */
void EXTI4_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line4) == SET)
	{
		EXTI_ClearITPendingBit(EXTI_Line4);

		delay_ms(20);
		if(!PAin(4))
		{
			key_val = KEY_3;
			Flame_threshold+=100;
			printf("KEY_3按下\n");
		}
	}
}

