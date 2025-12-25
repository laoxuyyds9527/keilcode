/*******************************************************************************
* @file    		tim2.c
* @function     定时器配置
* @brief 		用3.5.0版本库建的工程模板
* @attention	该例程仅作为参考，如需实际开发，请根据实际需求更改
* @version 		V1.1
* @company  	深圳市飞思创电子科技有限公司
* @website  	http://www.freestrong.com
* @tel			0755-86528386
* @Author       freestrong
* @date    		2020/09/20
********************************************************************************/
#include "tim.h"
#include "fat_core.h"
/**
 * @description: TIM2定时器初始化
 * @param ms: 单位毫秒，定时器时间间隔
 * @return None
 */
void tim2_init(int ms)
{
	TIM_TimeBaseInitType  TIM_TimeBaseInitStruct;
	RCC_EnableAPB1PeriphClk(RCC_APB1_PERIPH_TIM2, ENABLE);

	TIM_TimeBaseInitStruct.Period = ms * 1000 - 1;
	TIM_TimeBaseInitStruct.Prescaler = 72-1;
	TIM_TimeBaseInitStruct.ClkDiv = TIM_CLK_DIV1;
	TIM_TimeBaseInitStruct.CntMode = TIM_CNT_MODE_UP;
	TIM_InitTimeBase (TIM2,&TIM_TimeBaseInitStruct);

	NVIC_InitType NVIC_InitStruct;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);

	TIM_ConfigInt(TIM2,TIM_INT_UPDATE, ENABLE);
	TIM_Enable(TIM2, ENABLE);
}




void TIM2_IRQHandler(void)
{
    // 检查更新中断标志位
    if (TIM_GetIntStatus(TIM2, TIM_INT_UPDATE) != RESET)
    {
        // 示例：中断处理逻辑（可替换为你的代码）
        fat_tim_proc();

        // 清除中断挂起位（核心）
        TIM_ClrIntPendingBit(TIM2, TIM_INT_UPDATE);
    }
}
