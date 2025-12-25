#include "key.h"


//按键的初始化
void key_init(void)
{
	NVIC_InitTypeDef  NVIC_InitStruct;
	EXTI_InitTypeDef  EXTI_InitStruct;
	GPIO_InitTypeDef GPIO_InitStruct;
	//开启外设时钟重映射需要开复用
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
	//指定的gpio映射用作外部中断线路，给引脚配上一个中断器
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource1);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource4);
	
	//配置按键GPIOA0
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPD;//下拉输入
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_0; 
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	//配置按键GPIOA1,4
	GPIO_InitStruct.GPIO_Mode= GPIO_Mode_IPU;//上拉输入
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_1; 
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;

	GPIO_InitStruct.GPIO_Mode= GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_4; 
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);


	//etxi中断线路的初始化
	EXTI_InitStruct.EXTI_Line=EXTI_Line0;//中断线
	EXTI_InitStruct.EXTI_LineCmd=ENABLE;//使能
	EXTI_InitStruct.EXTI_Mode=EXTI_Mode_Interrupt;//由时间触发还是中断触发
	EXTI_InitStruct.EXTI_Trigger=EXTI_Trigger_Rising;//上升沿触发


	EXTI_InitStruct.EXTI_Line=EXTI_Line1;//中断线
	EXTI_InitStruct.EXTI_LineCmd=ENABLE;//使能
	EXTI_InitStruct.EXTI_Mode=EXTI_Mode_Interrupt;//由时间触发还是中断触发
	EXTI_InitStruct.EXTI_Trigger=EXTI_Trigger_Falling;//下降沿沿触发

	
	EXTI_InitStruct.EXTI_Line=EXTI_Line4;//中断线
	EXTI_InitStruct.EXTI_LineCmd=ENABLE;//使能
	EXTI_InitStruct.EXTI_Mode=EXTI_Mode_Interrupt;//由时间触发还是中断触发
	EXTI_InitStruct.EXTI_Trigger=EXTI_Trigger_Falling;//下降沿沿触发
	EXTI_Init(&EXTI_InitStruct);


	//NVIC初始化
	NVIC_InitStruct.NVIC_IRQChannel=EXTI0_IRQn;//中断通道
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;//使能
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=1;

	NVIC_InitStruct.NVIC_IRQChannel=EXTI1_IRQn;//中断通道
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;//使能
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=1;

	NVIC_InitStruct.NVIC_IRQChannel=EXTI4_IRQn;//中断通道
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;//使能
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=1;

	
	NVIC_Init(&NVIC_InitStruct);
	

}




//中断服务函数
void key_inter()
{





}