#ifndef __TIM__H
#define __TIM__H

#include "stm32f10x.h"
#include "system.h"
#include "usart_drv.h"
void Tim6_init(u16 per ,u16 psc);
unsigned short Get_CRC16(unsigned char *puchMsg, unsigned short usDataLen);




#endif
