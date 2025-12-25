#ifndef __AT24CXX_H
#define __AT24CXX_H

#include "stm32f10x.h"
#include "iic_drv.h"
#include "usart_drv.h"

#define AT24C02_ADDR_W	((0x50 << 1) + 0)
#define AT24C02_ADDR_R	((0x50 << 1) + 1)

void AT24CXX_WriteByte(u8 addr,u8 data);
u8 AT24CXX_ReadByte(u8 addr);

#endif

