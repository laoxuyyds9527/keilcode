#ifndef __W25QXX_DRV_H__
#define __W25QXX_DRV_H__

#include "stm32f10x.h"

/*FLASH常用命令*/
#define W25X_WriteEnable 0x06
#define W25X_WriteDisable 0x04
#define W25X_ReadStatusReg 0x05
#define W25X_WriteStatusReg 0x01
#define W25X_ReadData 0x03
#define W25X_FastReadData 0x0B
#define W25X_FastReadDual 0x3B
#define W25X_PageProgram 0x02
#define W25X_BlockErase 0xD8
#define W25X_SectorErase 0x20
#define W25X_ChipErase 0xC7
#define W25X_PowerDown 0xB9
#define W25X_ReleasePowerDown 0xAB
#define W25X_DeviceID 0xAB
#define W25X_ManufactDeviceID 0x90
#define W25X_JedecDeviceID 0x9F

//W25X系列/Q系列芯片ID列表
#define W25Q80 	0XEF13 	
#define W25Q16 	0XEF14
#define W25Q32 	0XEF15
#define W25Q64 	0XEF16
#define W25Q128	0XEF17


uint8_t W25QXX_ReadSR(void);
void W25QXX_Write_SR(uint8_t sr);
void W25QXX_Wait_Busy(void);
void W25QXX_Write_Enable(void);
void W25QXX_Write_Disable(void);
uint16_t W25QXX_ReadID(void);
void W25QXX_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead);
void W25QXX_WritePage(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);
void W25QXX_Write_NoCheck(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);
void W25QXX_Erase_Sector(u32 Dst_Addr);
void W25QXX_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);
void W25QXX_PowerDown(void);
void W25QXX_WAKEUP(void);
void W25QXX_EraseBlock(uint32_t blockNB);

#endif
