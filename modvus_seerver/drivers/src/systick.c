#include "systick.h"

u32 nus;//记录定时1微秒的计数次数
u32 nms;//记录定时1毫秒的计数次数

/*
 * @brief:systick初始化
 * @param:sysclk系统时钟频率
 * @retval:无
 */
void SysTick_Init(u8 sysclk)
{
	//选择时钟源
	SysTick -> CTRL &= ~(0x01<<2);
	nus = sysclk/8;
	nms = nus*1000;
}

/*
 * @brief:微秒级延时(注意：延时不能超过1864135微秒)
 * @param:n延时时间
 * @retval:无
 */
void delay_us(u32 n)
{
	u32 temp; 
	//设置自动重装载值
	SysTick -> LOAD = n*nus;
	//清除当前值
	SysTick -> VAL = 0;
	//使能systick
	SysTick -> CTRL |= 0x01;
	do{
		temp = SysTick -> CTRL;
	}while((temp & 0x01) && !(temp & (0x01 << 16)));
	//关闭systick	
	SysTick -> CTRL &= ~(0x01);
	SysTick -> VAL = 0;
}

/*
 * @brief:毫秒级延时(注意：延时不能超过1864毫秒)
 * @param:n延时时间
 * @retval:无
 */
void delay_ms(u32 n)
{
	u32 temp; 
	//设置自动重装载值
	SysTick -> LOAD = n*nms;
	//清除当前值
	SysTick -> VAL = 0;
	//使能systick
	SysTick -> CTRL |= 0x01;
	do{
		temp = SysTick -> CTRL;
	}while((temp & 0x01) && !(temp & (0x01 << 16)));
	//关闭systick	
	SysTick -> CTRL &= ~(0x01);
	SysTick -> VAL = 0;
}


void delay_nms(u16 n)
{
	u16 round,remain;
	u8 i;
	round = n/1000;
	remain = n%1000;
	
	if(round != 0)
	{
		for(i = 0;i < round;i++)
		{
			delay_ms(1000);//延时1000ms
		}
		if(remain != 0)
		{
			delay_ms(remain);
		}
	}
	else
	{
		if(remain != 0)
		{
			delay_ms(remain);
		}
	}
}
