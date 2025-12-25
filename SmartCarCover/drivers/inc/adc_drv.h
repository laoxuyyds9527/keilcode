#ifndef __ADC_DRV_H
#define __ADC_DRV_H

#include "stm32f10x.h"
#include "dma_drv.h"
#include "usart_drv.h"

void ADC1_CH16_Init(void);
void ADC1_CH2_Init(void);
float Get_TempVal(void);
u16 Get_ConvVal(void);
void ADC1_CH3_Init(void);
u16 Get_Power(void);

#endif

