#include "n32wb452.h"
#include "stdio.h"
#include "led.h"
#include "usart.h"
#include "systick.h"
#include "gpio.h"
#include "uart.h"
#include "tim.h"
#include "fat_core.h"
#include "lte_demo.h"




void headware_init(void)
{
	Systick_Init(144);
	led_init();	
	pen_gpio_init();
	UART5_init(115200);
	uart2_init();
	
	tim2_init(FAT_TIMER_VAL);
	
	reg_fat_uart_send_byte(uart2_send_byte);

}


int main(void)
{  
	
	headware_init();
	
  	while (1)
    {	
        module_MQTT();
    
	}
}

