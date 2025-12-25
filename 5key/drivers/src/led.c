#include "led.h"


//led的初始化
void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	//
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC,ENABLE);

	//
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	//
	GPIO_SetBits(GPIOB, GPIO_Pin_2);
	GPIO_SetBits(GPIOC, GPIO_Pin_5);


}



void LED_Ctrl(u8 led_choose, u8 led_state)
{
	switch(led_choose)
	{
		case LED_B:
			if(led_state == LED_ON)
				GPIO_ResetBits(GPIOB, GPIO_Pin_2);
			else if(led_state == LED_OFF)
				GPIO_SetBits(GPIOB, GPIO_Pin_2);
			break;
		case LED_R:
			if(led_state == LED_ON)
				GPIO_ResetBits(GPIOC, GPIO_Pin_5);
			else if(led_state == LED_OFF)
				GPIO_SetBits(GPIOC, GPIO_Pin_5);
			break;
	}
}

