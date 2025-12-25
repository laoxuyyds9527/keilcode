#ifndef __USART_DRV_H
#define __USART_DRV_H

/*头文件*/
#include "stm32f10x.h"
#include <stdio.h>
#include "stdarg.h"
#include "string.h"

#define USART1_TXBUFF_SIZE   256

/*函数声明*/
void Usart_MyConfig(u32 baudrate);
void USART2_Config(u32 baudrate);
void u1_printf(char* fmt,...) ;

#endif

