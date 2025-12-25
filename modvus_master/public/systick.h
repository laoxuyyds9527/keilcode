#ifndef __SYSTICK_H
#define __SYSTICK_H

#include "stm32f10x.h"

void Systick_Init(u8 sysclk);
void delay_us(u32 n);
void delay_ms(u32 n);
void delay_nms(u32 n);



#endif

