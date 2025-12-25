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
	//选择AHB/8
	SysTick->CTRL &= ~(0x01 << 2);

	//计算定时1us和1ms所需的计数次数
	nus = sysclk / 8;
	nms = nus * 1000;
}


/**
 * @brief 微秒级延迟,不能超过1.86s
 * @param n:要延迟的微秒数
 * @retval 无
 */
void delay_us(u32 n)
{
	//设置重装载寄存器
	SysTick->LOAD = n * nus;

	//清空当前计数器值
	SysTick->VAL = 0;

	//使能定时器
	SysTick->CTRL |= 0x01;

	//等待计时结束
	u32 temp;
	do{
		temp = SysTick->CTRL;
	}while((temp & 0x01) && !(temp & (0x01 << 16)));

	//关闭定时器
	SysTick->CTRL &= ~0x01;

	//清空当前计数器值
	SysTick->VAL = 0;
}


/**
 * @brief 毫秒级延迟,不能超过1.86s
 * @param n:要延迟的毫秒数
 * @retval 无
 */
void delay_ms(u32 n)
{
	//设置重装载寄存器
	SysTick->LOAD = n * nms;

	//清空当前计数器值
	SysTick->VAL = 0;

	//使能定时器
	SysTick->CTRL |= 0x01;

	//等待计时结束
	u32 temp;
	do{
		temp = SysTick->CTRL;
	}while((temp & 0x01) && !(temp & (0x01 << 16)));

	//关闭定时器
	SysTick->CTRL &= ~0x01;

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
	round = n / 1800;
	remain = n % 1800;
	for(int i = 0;i < round;i++)
		delay_ms(1800);
	if(remain != 0)
		delay_ms(remain);
}
