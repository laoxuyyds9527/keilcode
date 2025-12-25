#ifndef __SYSTICK_H
#define __SYSTICK_H

/* 头文件 */
#include "stm32f10x.h"

/* 函数声明*/
void Systick_Init(u8 sysclk);
void SysTick_Handler(void);
void delay_us(u16 n);
void delay_ms(u16 n);
void delay_s(u16 n);

#endif
