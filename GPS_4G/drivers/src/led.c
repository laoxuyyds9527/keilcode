#include "led.h"



void led_init(void)
{
	GPIO_InitType  GPIO_InitStruct;
	//开启外设时钟
	RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOA,ENABLE);
	//初始化引脚
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStruct.Pin=GPIO_PIN_8;
	GPIO_InitPeripheral(GPIOA,&GPIO_InitStruct);
}



void led_on(void)
{

	GPIO_SetBits(GPIOA, GPIO_PIN_8);

}



void led_off(void)
{

	GPIO_ResetBits(GPIOA, GPIO_PIN_8);

}
