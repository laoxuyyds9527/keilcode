#include "tim.h"






//设置基本定时
void Tim6_init(u16 per ,u16 psc)
{
	NVIC_InitTypeDef  NVIC_InitStruct;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStruct;
	//配置外部时钟源
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
	//配置tim6定时器
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;//指定时钟时钟分频，1
	TIM_TimeBaseInitStruct.TIM_CounterMode= TIM_CounterMode_Up;//计数器模式向上技术
	TIM_TimeBaseInitStruct.TIM_Period= per;//预分频系数
	TIM_TimeBaseInitStruct.TIM_Prescaler= psc;//重装载值
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter=0;//旨在高级定时器tim1和tim8有效
	TIM_TimeBaseInit(TIM6,&TIM_TimeBaseInitStruct);
	//配置中断源
	TIM_ITConfig(TIM6,TIM_IT_Update, ENABLE);
	//配置NVIC
	NVIC_InitStruct.NVIC_IRQChannel=TIM6_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=1;//抢占优先级
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=1;//响应优先级
	NVIC_Init(&NVIC_InitStruct);
	
	//开启定时器
	TIM_Cmd(TIM6, ENABLE);
}



//tim6的终端服务函数
void TIM6_IRQHandler(void)//不知道在哪里看这个函数的定义
{
	if(TIM_GetFlagStatus(TIM6, TIM_IT_Update)==SET)
		{
			TIM_ClearFlag(TIM6, TIM_IT_Update);
			PBout(2)=!PBout(2);
			PCout(5)=!PCout(5);

		}

}
