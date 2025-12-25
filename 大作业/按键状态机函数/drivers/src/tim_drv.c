#include "tim_drv.h"


/**
 * @brief TIM6的初始化
 * @param per:自动重装载值
 * @param psc:预分频系数
 * @retval 无
 */
void TIM6_Init(u16 per,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;

	//开启外设时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);

	//初始化TIM6定时器
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;  //时钟分割,不分割
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;  //向上计数模式,基本定时器只有这种模式
	TIM_TimeBaseInitStruct.TIM_Period = per;  //自动重装载值
	TIM_TimeBaseInitStruct.TIM_Prescaler = psc;  //预分频器系数
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;  //仅对于高级定时器TIM1和TIM8有效
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseInitStruct);

	//配置中断源
	TIM_ClearITPendingBit(TIM6,TIM_IT_Update);
	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE);

	//NVIC配置
	NVIC_InitStruct.NVIC_IRQChannel = TIM6_IRQn;  //中断通道
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;  //抢占优先级
  	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;  //响应优先级
	NVIC_Init(&NVIC_InitStruct);
	
	//开启定时器
	TIM_Cmd(TIM6, ENABLE);
}

/**
 * @brief TIM6中断服务函数
 * @param 无
 * @retval 无
 */
void TIM6_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM6,TIM_IT_Update) == SET)
	{
		
		
		if(++key_shake_cnt >= 0xff)
			{
				key_shake_cnt = 0xff;
			}
		if(++key_press_cnt >= 0xff)
			{
				key_press_cnt = 0xff;
			} 
		if(++double_wait_cnt >= 0xff)
			{
				double_wait_cnt = 0xff;
			}
		TIM_ClearITPendingBit(TIM6,TIM_IT_Update);
	}
}

