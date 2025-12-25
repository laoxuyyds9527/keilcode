#include "stm32f10x.h"
#include "systick.h"
#include "usart_drv.h"
#include "system.h"
#include "tim.h"
#include "led_drv.h"
void HardWare_Init(void)
{
	//优先级分组
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	//systick
	Systick_Init(72);
	//USART1初始化
	USART1_Init(115200);
	LED_Init();
	
	Tim6_init(5000,7200);//设定500ms，1/72000000
}


int main (void)
{
	
	HardWare_Init();
	PCout(5)=1;
	while(1)
	{
		
	}
}
