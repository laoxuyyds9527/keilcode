#ifndef __UART_H
#define __UART_H

#include "n32wb452.h"
#include <stdio.h>

extern double lati;
extern double longi;


void uart2_init(void);
void UART5_init(u32 baud);
void uart2_send_byte(uint8_t data);
#endif
