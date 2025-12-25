#include "stm32f10x.h"
#include "led.h"
#include "usart.h"
void HardWare_Init()
{
	LED_Init();
	Usart_init(9600);
}



int main (void){
	u16 data;
	HardWare_Init();
		
	while(1)
	{
		if(USART_GetFlagStatus(USART1, USART_FLAG_RXNE)==SET)
			{
				data=USART_ReceiveData (USART1);
				
				USART_SendData(USART1,data);
			}
		
		
		
	}
}
