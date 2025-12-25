#include "stm32f10x.h"
#include "led_drv.h"
#include "rcc_drv.h"




void HardWare_Init(void)
{
	//RCC初始化
	RCC_MyConfig(RCC_PLLSource_HSE_Div1,RCC_PLLMul_9);
	//LED初始化
	LED_Init();
}

void delay(void)
{
	for(int i = 0;i < 0x8FFFF;)
	{
		i++;
	}
}

int main (void){
	
	HardWare_Init();

	while(1)
	{
		LED_Ctrl(LED_B,LED_ON);
		delay();
		LED_Ctrl(LED_B,LED_OFF);
		delay();
		LED_Ctrl(LED_R,LED_ON);
		delay();
		LED_Ctrl(LED_R,LED_OFF);
		delay();
	}
}
