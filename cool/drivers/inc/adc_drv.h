#ifndef __ADC_DRV_H
#define __ADC_DRV_H

#include "stm32f10x.h"

#include "usart_drv.h"


void ADC1_CH2_Init(void);
u16 Get_ConvVal(void);

#endif

