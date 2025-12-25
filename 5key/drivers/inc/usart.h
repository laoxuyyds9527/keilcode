#ifndef __USART__
#define __USART __

#include "stm32f10x.h"
#include "stdio.h"
void Usart_init(u32 baudrate);
void USART1_IRQHander(void);
#endif
