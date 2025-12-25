#include "stm32f10x.h"
#include "systick.h"
#include "usart_drv.h"
#include "system.h"
#include "tim_drv.h"
#include "key_drv.h"
#include "pwn.h"
#include "adc_drv.h"

void HardWare_Init(void)
{
	//优先级分组
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	//systick
	Systick_Init(72);
	//USART1初始化
	USART1_Init(115200);
	//按键初始化
	Key_Init();
	//TIM6初始化
	TIM6_Init(100-1,7200-1);
	//pwn的初始化
	pwn_init(100-1,360-1);//2000hz的频率
	ADC1_CH2_Init();
}


int main (void){
	
	HardWare_Init();
	u8 key_type;
	u32 data=0;
	u16 off=0;
	while(1)
	{
		key_type = State_Machine();
		if(key_type == KEY_TYPE_LONG)
			printf("长按\n");
		else if(key_type == KEY_TYPE_SHORT)
		{
			printf("单次短按\n");
			// 短按后的PWM调整循环（添加退出条件）
			while(1)
			{
				//检测光敏值
				u8 max=ADC_GetConversionValue(ADC1)/(4095/100);
				
				// 1. 核心：在内部循环中持续检测按键（获取退出指令）
				key_type = State_Machine();
				if(key_type == KEY_TYPE_DOUBLE)  // 检测到任何有效按键（短按/长按/双击）
				{
					// 可选：打印退出原因	
					printf("双击 → 退出PWM调整\n");
					break;  // 跳出内部循环，回到主循环
				}
				if(1)
					TIM_SetCompare1(TIM1,max);//重新设置捕获的比较值
				delay_ms(5);
				off=off+1;
				if(off==12000)//60秒没动作
				{
					TIM_SetCompare1(TIM1,0);
					break;
				}
				
			}			
		}
		else if(key_type == KEY_TYPE_DOUBLE)
		{
			printf("双击短按\n");
			TIM_SetCompare1(TIM1,0);
					
		}			
	}
}
