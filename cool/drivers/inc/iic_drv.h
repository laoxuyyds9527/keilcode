#ifndef __IIC_DRV_H
#define __IIC_DRV_H

#include "stm32f10x.h"
#include "systick.h"

#define SCL_RCC		RCC_APB2Periph_GPIOB
#define SCL_PORT	GPIOB
#define SCL_PIN		GPIO_Pin_6

#define SDA_RCC		RCC_APB2Periph_GPIOB
#define SDA_PORT	GPIOB
#define SDA_PIN		GPIO_Pin_7

#define SCL_UP		GPIO_SetBits(SCL_PORT, SCL_PIN)
#define SCL_DOWN	GPIO_ResetBits(SCL_PORT, SCL_PIN)
#define SDA_UP		GPIO_SetBits(SDA_PORT, SDA_PIN)
#define SDA_DOWN	GPIO_ResetBits(SDA_PORT, SDA_PIN)

#define DELAY		delay_us(1)

#define SDA_READ	GPIO_ReadInputDataBit(SDA_PORT, SDA_PIN)

enum{
	IIC_NOACK = 0,
	IIC_ACK
};

void IIC_Config(void);
void IIC_Output(void);
void IIC_Input(void);
void IIC_Start(void);
void IIC_Stop(void);
void IIC_SendACK(void);
void IIC_SendNoACK(void);
u8 IIC_WaitACK(void);
void IIC_WriteByte(u8 data);
u8 IIC_ReadByte(void);



#endif

