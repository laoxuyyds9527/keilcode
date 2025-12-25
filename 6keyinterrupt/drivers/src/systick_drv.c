#include "systick_drv.h"


volatile uint32_t uwTick;

/**
 * @brief:初始化systick
 * @param:sysclk系统时钟频率
 * @retval:无
 */
void Systick_Init(u8 sysclk)
{
	/* 选择AHB */
	SysTick -> CTRL |= (0x01 << 2);
}


/**
 * @brief:滴答定时器的中断处理函数
 * @param:无
 * @retval:无
 */
void SysTick_Handler(void)//滴答定时器中断服务函数
{
	if(uwTick != 0x00)
	{ 
		uwTick--;
	}
}

/**
 * @brief:100us级的滴答定时器
 * @param:us延迟100*us微秒
 * @retval:无
 */
void delay_us(uint16_t us)
{ 
	SysTick_Config(SystemCoreClock/10000);//开滴答定时器中断100us级
	
	uwTick = us;
	while(uwTick != 0);//等待时间到达
	
	SysTick->CTRL = 0x00;//关闭定时器
	SysTick->VAL = 0x00;//清空定时器的计数器
}

/**
 * @brief:毫秒级滴答定时器
 * @param:ms毫秒数
 * @retval:无
 */
void delay_ms(uint16_t ms)
{ 
	delay_us(10 * ms);
}

/**
 * @brief:秒级滴答定时器
 * @param:s秒数
 * @retval:无
 */
void delay_s(uint16_t s)
{ 
	delay_ms(1000 * s);
}

