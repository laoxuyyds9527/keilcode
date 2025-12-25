#include "systick.h"

u32 nms;
u32 nus;

/**
 * @brief systick初始化
 * @param sysclk：系统时钟频率，单位M
 * @retval 无
 */
void Systick_Init(u8 sysclk)
{
	//选择AHB
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
	//计算定时1us和1ms所需的计数次数
	nus = sysclk;
	nms = nus * 1000;
}


/**
 * @brief 微秒级延迟,不能超过116508微秒
 * @param n:要延迟的微秒数
 * @retval 无
 */
void delay_us(u32 n)
{
	u32 temp;
	//设置重装载寄存器
	SysTick->LOAD = n * nus;

	//清空当前计数器值
	SysTick->VAL = 0;

	//使能定时器
	SysTick->CTRL |= 0x01;

	//等待计时结束

	do{
		temp = SysTick->CTRL;
	}while((temp & 0x01) && !(temp & (0x01 << 16)));

	//关闭定时器
	SysTick->CTRL &= ~(0x01);

	//清空当前计数器值
	SysTick->VAL = 0;
}


/**
 * @brief 毫秒级延迟,不能超过116毫秒
 * @param n:要延迟的毫秒数
 * @retval 无
 */
void delay_ms(u32 n)
{
	u32 temp;

	//设置重装载寄存器
	SysTick->LOAD = n * nms;

	//清空当前计数器值
	SysTick->VAL = 0;

	//使能定时器
	SysTick->CTRL |= 0x01;

	//等待计时结束

	do{
		temp = SysTick->CTRL;
	}while((temp & 0x01) && !(temp & (0x01 << 16)));

	//关闭定时器
	SysTick->CTRL &= ~(0x01);

	//清空当前计数器值
	SysTick->VAL = 0;
}


/**
 * @brief 毫秒级延迟
 * @param n:要延迟的毫秒数
 * @retval 无
 */
void delay_nms(u32 n)
{
	u32 round,remain;
	round = n / 100;
	remain = n % 100;
	for(int i = 0;i < round;i++)
		delay_ms(100);
	if(remain != 0)
		delay_ms(remain);
}
