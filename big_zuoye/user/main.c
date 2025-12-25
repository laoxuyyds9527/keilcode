#include "stm32f10x.h"
#include "led_drv.h"
#include "systick.h"
#include "usart_drv.h"
#include "system.h"
#include "key_drv.h"

void HardWare_Init(void)
{
	//优先级分组
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	//systick
	Systick_Init(72);
	//LED初始化
	LED_Init();
	//USART1初始化
	USART1_Init(115200);
	//按键初始化
	Key_Init();
}

int main (void){
	
	HardWare_Init();

	u8 key;
	
	while(1)
	{
		//模拟耗时任务
		delay_nms(300);
		printf("%d\n",PAin(1));
		key = Key_Scan(1);
		if(key == KEY_1)
			printf("KEY_1按下\n");
		else if(key == KEY_2)
			printf("KEY_2按下\n");
		else if(key == KEY_3)
			printf("KEY_3按下\n");
	}
}
