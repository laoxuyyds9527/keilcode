#include "tim_drv.h"
#include "usart_drv.h"

void TIM_Config(uint32_t arr,uint32_t psc)
{
    //开启外设时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);
    
    //初始化定时器
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStruct.TIM_Period = arr;
    TIM_TimeBaseInitStruct.TIM_Prescaler = psc;
    TIM_TimeBaseInit(TIM6,&TIM_TimeBaseInitStruct);
    
    //配置更新中断
    TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE);
    
    //初始化NVIC
    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = TIM6_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_Init(&NVIC_InitStruct);
    
    //使能定时器
    TIM_Cmd(TIM6,ENABLE);
}


void TIM6_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM6,TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM6,TIM_IT_Update);
        
        if(uart_recv_start_flag)
        {
            time_out_count++;
            if(time_out_count >= 5)
            {
                time_out_count = 0;
                uart_recv_end_flag = 1;
                uart_recv_start_flag = 0;
            }            
        }
        
    }
}


