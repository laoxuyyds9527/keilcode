#ifndef __SPI_DRV_H__
#define __SPI_DRV_H__

#include "stm32f10x.h"

#define SPI1_CS_SET   GPIO_SetBits(GPIOA,GPIO_Pin_4)
#define SPI1_CS_CLR   GPIO_ResetBits(GPIOA,GPIO_Pin_4)

void SPI1_Config(void);
uint8_t SPI1_ReadWriteByte(uint8_t byte);
uint8_t SPI1_ReadByte(void);

#endif
