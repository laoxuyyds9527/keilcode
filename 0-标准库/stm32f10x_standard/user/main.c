#include "stm32f10x.h"
#include "systick.h"
#include "usart_drv.h"
#include "system.h"

void HardWare_Init(void)
{
	//优先级分组
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	//systick
	Systick_Init(72);
	//USART1初始化
	USART1_Init(115200);
}


int main (void){
	
	HardWare_Init();
	
	while(1)
	{
		
	}
}
