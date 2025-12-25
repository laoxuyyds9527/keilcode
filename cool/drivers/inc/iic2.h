#ifndef __IIC2_H
#define __IIC2_H

#include "stm32f10x.h"
#include "systick.h"

#define SCL2_RCC		RCC_APB2Periph_GPIOB
#define SCL2_PORT		GPIOB
#define SCL2_PIN		GPIO_Pin_10

#define SDA2_RCC		RCC_APB2Periph_GPIOB
#define SDA2_PORT		GPIOB
#define SDA2_PIN		GPIO_Pin_11

#define SCL2_UP		GPIO_SetBits(SCL2_PORT, SCL2_PIN)
#define SCL2_DOWN	GPIO_ResetBits(SCL2_PORT, SCL2_PIN)
#define SDA2_UP		GPIO_SetBits(SDA2_PORT, SDA2_PIN)
#define SDA2_DOWN	GPIO_ResetBits(SDA2_PORT, SDA2_PIN)

#define DELAY		delay_us(1)

#define SDA2_READ	GPIO_ReadInputDataBit(SDA2_PORT, SDA2_PIN)

enum{
	IIC2_NOACK = 0,
	IIC2_ACK
};

void IIC2_Config(void);
void IIC2_Output(void);
void IIC2_Input(void);
void IIC2_Start(void);
void IIC2_Stop(void);
void IIC2_SendACK(void);
void IIC2_SendNoACK(void);
u8 IIC2_WaitACK(void);
void IIC2_WriteByte(u8 data);
u8 IIC2_ReadByte(void);



#endif

