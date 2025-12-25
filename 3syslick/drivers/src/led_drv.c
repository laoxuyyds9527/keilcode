#include "led_drv.h"

#define PORT_LED	GPIOA	
#define PIN_LED		GPIO_Pin_8
#define RCC_LED		RCC_APB2Periph_GPIOA


/**
 * @brief:LED的初始化
 * @param:无
 * @retval:无
 */
void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	//使能外设时钟
	RCC_APB2PeriphClockCmd(RCC_LED,ENABLE);

	//初始化GPIO
	GPIO_InitStruct.GPIO_Pin = PIN_LED;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(PORT_LED, &GPIO_InitStruct);
	
	//初始化为灭灯
	GPIO_SetBits(PORT_LED, PIN_LED);
	
}

/**
 * @brief:LED的控制
 * @param:led_choose选择控制的灯
 * @param:led_state选择给定灯的状态
 * @retval:无
 */
void LED_Ctrl(u8 led_choose, u8 led_state)
{
	switch(led_choose)
	{
		case LED_1:
		{
			if(led_state == LED_ON)
			{
				GPIO_ResetBits(PORT_LED, PIN_LED);
			}
			else if(led_state == LED_OFF)
			{
				GPIO_SetBits(PORT_LED, PIN_LED);
			}
		}
		break;
	}
}


