#ifndef __SYSTICK_H
#define __SYSTICK_H

/*头文件*/
#include "stm32f10x.h"

/*函数声明*/
void SysTick_Init(u8 sysclk);
void delay_us(u32 n);
void delay_ms(u32 n);


#endif

