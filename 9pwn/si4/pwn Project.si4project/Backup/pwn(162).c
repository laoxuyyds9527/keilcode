#include "pwn.h"



void pwn_init(u16 per,u16 pcs)
{


	GPIO_InitTypeDef  GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef  TIM_OCInitStruct;

	
	//开启时钟
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOA|RCC_APB2Periph_TIM1, ENABLE);
	//配置GPIO引脚gpa8
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_6;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	//开启高级定时器1的初始化
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period=per;
	TIM_TimeBaseInitStruct.TIM_Prescaler=pcs;
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter=0;
	TIM_TimeBaseStructInit(TIM1,&TIM_TimeBaseInitStruct);

	//输出的配置，因为是高级定时器所以是oc1专用的函数...........还是不清楚这部分的
	//缺少的填默认值
	
	
	TIM_OCInitStruct.TIM_OCIdleState=TIM_OCIdleState_Set;
	TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM1;//
	//TIM_OCInitStruct.TIM_OCNIdleState
	//TIM_OCInitStruct.TIM_OCNPolarity都是互补输出才需要填
	TIM_OCInitStruct.TIM_OCPolarity=TIM_OCPolarity_High;//有效电平
	//TIM_OCInitStruct.TIM_OutputNState
	TIM_OCInitStruct.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_Pulse=0;
	TIM_OC1Init(TIM1,&TIM_OCInitStruct);
	
	//开启定时的主输出的使能
	TIM_CtrlPWMOutputs(TIM1,ENABLE);

	//开启自动重装值arr的影子寄存器，也急速预装载值寄存器
	TIM_ARRPreloadConfig(TIM1,ENABLE);

	//开启捕获比较寄存的影子寄存器
	TIM_OC1PreloadConfig(TIM1,TIM_OCPreload_Enable);

	//开启计时器
	TIM_Cmd(TIM1, ENABLE);

}
