#include "stm32f10x.h"
#include "led_drv.h"

void HardWare_Init(void)
{
	/* LED≥ı ºªØ */
	LED_Init();
}

int main (void){
	
	HardWare_Init();

	while(1)
	{
		LED_Ctrl(LED_1,LED_ON);
	}
}
