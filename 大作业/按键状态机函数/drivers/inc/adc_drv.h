#ifndef __ADC_DRV_H
#define __ADC_DRV_H

#include "stm32f10x.h"

void ADC1_CH16_Init(void);
void ADC1_CH2_Init(void);
float Get_TempVal(void);
u16 Get_ConvVal(void);


#endif

