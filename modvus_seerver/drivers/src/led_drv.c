#include "led_drv.h"

void LED_Init(void)
{
    //使能外设时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
    
    //GPIO初始化
    GPIO_InitTypeDef GPIO_InitStruct;
    
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;//指定引脚号
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;//指定GPIO的工作模式 推挽输出
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//GPIO 速率
    
    GPIO_Init(GPIOA, &GPIO_InitStruct);
    
    //设置LED的初始状态,关闭
    GPIO_SetBits(GPIOA,GPIO_Pin_8);    
}


void LED_Ctrl(LED_Dev led_num,uint8_t led_state)
{
    switch(led_num)
    {
        case  LED0:
        {
            if(led_state == LED_ON)
            {
                GPIO_ResetBits(GPIOA,GPIO_Pin_8);
            }
            else
            {
                GPIO_SetBits(GPIOA,GPIO_Pin_8);  
            }
            break;
        }
        
        default: break;
    }
}








