#ifndef __LED__H
#define __LED__H

#include "n32wb452.h"
#include <stdio.h>
void UART5_init(u16 buad);
void UART5_tx(uint16_t Data);
u16 UART5_rx(uint16_t data);




#endif

