#include "n32wb452.h"
#include "led.h"
#include "usart.h"
#include "systick.h"
void headware_init(void)
{
	led_init();
	UART5_init(115200);
	Systick_Init(144);
}


int main (void)
{
	headware_init();
	u16 data=0;
	while(1)
	{
//	led_on();
//	delay_ms(100);
////	printf("xx");
//	led_off();
//	delay_ms(100);
	data=UART5_rx();
	if(data!=0)
	{		
		
	UART5_tx(data);
	}
	}
	 

}
