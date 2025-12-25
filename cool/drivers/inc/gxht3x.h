#ifndef __GXHT3X__H
#define __GXHT3X__H


#include "stm32f10x.h"
#include "iic_drv.h"
#include "usart_drv.h"
#include "systick.h"

#define GXHT3X_ADDR_W	((0x44 << 1) + 0)
#define GXHT3X_ADDR_R	((0x44 << 1) + 1)

#define SHT30_ADDR (uint8_t)(0x44<<1) //sht30 i2c地址，ADDR管脚接低电平时为0x44,接高电平为0x45
#define SHT30_READ_HUMITURE (uint16_t)0x2c06  //读温湿度
#define POLYNOMIAL 0x31 // X^8 + X^5 + X^4 + 1CRC多项式



unsigned char crc_gxht(uint8_t *data, uint8_t len);
void SHT30_CMD(uint16_t cmd);
u8 SHT30_Read_Humiture(int *temp,uint16_t *humi);




#endif
