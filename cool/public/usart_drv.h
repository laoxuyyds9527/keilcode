#ifndef __USART_DRV_H
#define __USART_DRV_H

#include "stm32f10x.h"
#include <stdio.h>
#include "stdarg.h"
#include "string.h"

void USART1_Init(u32 baudrate);

#define UART4_TXBUFF_SIZE   1024
#define UART4_RXBUFF_SIZE   1024

extern unsigned int Uart4_RxCounter;
extern char Uart4_RxBuff[UART4_RXBUFF_SIZE];
extern char Uart4_RxCompleted ; //外部声明，其他文件可以调用该变量

/*函数声明*/

extern void UART4_MyConfig(u32 baudrate);

extern void u4_printf(char* fmt,...);


#endif

