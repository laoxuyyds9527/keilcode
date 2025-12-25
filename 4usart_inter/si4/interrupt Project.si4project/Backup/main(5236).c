#include "stm32f10x.h"
#include "led.h"
#include "usart.h"
void HardWare_Init()
{
	LED_Init();
	Usart_init(9600);
}



int main (void){

	HardWare_Init();
	while(1)
	{
		USART_IRQHander();
	}
}
