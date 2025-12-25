#include "stm32f10x.h"
#include "systick.h"
#include "usart_drv.h"
#include "system.h"
#include  "pwn.h"

void HardWare_Init(void)
{
	//优先级分组
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	//systick
	Systick_Init(72);
	//USART1初始化
	USART1_Init(115200);
	//pwn的初始化
	pwn_init(100-1,360-1);//2000hz的频率
}


int main (void){
	
	HardWare_Init();
	u16 data=0;
	u16 flag=0;
	while(1)
	{
		if(flag)
		TIM_SetCompare1(TIM1,data--);//重新设置捕获的比较值
		else
		TIM_SetCompare1(TIM1,data++);
		if(data==99||data==0)
			data=!data;
	}
}
