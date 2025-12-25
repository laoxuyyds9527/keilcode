#include "stm32f10x.h"
#include "led.h"
#include "usart.h"
void HardWare_Init()
{
	LED_Init();
	Usart_init(9600);
	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_2);
	void key ();
}



int main (void)
{
	u8 ky;
	HardWare_Init();
	while(1)
	{
		u8 keyscan();
		if(ky=kye_no)
		{
			printf("没有按");
		}
		if(ky=kye_1)
		{
			printf("按1");
		}
		if(ky=kye_2)
		{
			printf("按2");
		}
		if(ky=kye_3)
		{
			printf("按3");
		}
	}
	return 0;
}
