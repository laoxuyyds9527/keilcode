#include "stm32f10x.h"
#include "led.h"
#include "usart.h"
void HardWare_Init()
{
	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_2);
	LED_Init();
	Usart_init(9600);
}



int main (void)
	{

	HardWare_Init();
	while(1)
	{
		u16 data;
		printf("A");
		USART1_IRQHander();
		
	}
}
