#ifndef __SGP30__H
#define __SGP30__H



#include "stm32f10x.h"
#include "iic2.h"
#include "usart_drv.h"
#include "systick.h"

#define SGP30_read  0xb1                                    //SGP30的读地址
#define SGP30_write 0xb0                                    //SGP30的写地址


void SGP30_Write(u8 a, u8 b);
void SGP30_Init(void);
void Sgp30_Get_Co2_Value(u16 *co2_value);

#endif
