/*-----------------------------------------------------------------------*/
/* Low level disk I/O module SKELETON for FatFs     (C)ChaN, 2025        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "ff.h"			/* Basic definitions of FatFs */
#include "diskio.h"		/* Declarations FatFs MAI */

/* Example: Declarations of the platform and disk functions in the project */
//#include "platform.h"
//#include "storage.h"

/* Example: Mapping of physical drive number for each drive */
#define DEV_SPI_FLASH	0	/* Map FTL to physical drive 0 */
#define DEV_MMC		1	/* Map MMC/SD card to physical drive 1 */
#define DEV_USB		2	/* Map USB MSD to physical drive 2 */


/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/


DSTATUS disk_status (
        BYTE pdrv                /* Physical drive nmuber to identify the drive */
)
{
        DSTATUS stat = STA_NOINIT;

        switch (pdrv) {
        case DEV_SPI_FLASH :
        {
            //SPI FLASH 状态检测：读取 SPI FLASH 设备 ID
            uint16_t FlashId = W25QXX_ReadID();
            if(FlashId == W25Q128)
            {
                //设备ID读取结果正确
                stat &= ~STA_NOINIT;
            }
            else
            {
                //设备ID读取结果错误
                stat = STA_NOINIT;
            }
            break;
        }
        
        default:
            stat = STA_NOINIT;
        }
        return stat;
}




/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
        BYTE pdrv                                /* Physical drive nmuber to identify the drive */
)
{
        DSTATUS stat = STA_NOINIT;
        uint16_t i;

        switch (pdrv) {
        case DEV_SPI_FLASH :
        {
            //初始化SPI Flash
            SPI1_Config();
            //延时等待
            i = 500;
            while(--i);
            //唤醒SPI Flash
            W25QXX_WAKEUP();
            //获取SPI Flash状态
            stat = disk_status(DEV_SPI_FLASH);
            break;
        }
                
        default:
            stat = STA_NOINIT;
        }
        return stat;
}




/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
        BYTE pdrv,                /* Physical drive nmuber to identify the drive */
        BYTE *buff,                /* Data buffer to store read data */
        LBA_t sector,        /* Start sector in LBA */
        UINT count                /* Number of sectors to read */
)
{
        DRESULT stat = RES_PARERR;

        switch (pdrv) {
            case DEV_SPI_FLASH :
            {
                //扇区偏移2MB，外部flash文件系统空间放在SPI Flash后面6M空间
                sector +=512;
                W25QXX_Read(buff,sector<<12,count<<12);
                stat = RES_OK;
                break;
            }
    
            default:
                stat = RES_PARERR;
        }

        return stat;
}




/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/


DRESULT disk_write (
        BYTE pdrv,                        /* Physical drive nmuber to identify the drive */
        const BYTE *buff,        /* Data to be written */
        LBA_t sector,                /* Start sector in LBA */
        UINT count                        /* Number of sectors to write */
)
{
        DRESULT stat = RES_PARERR;
        uint32_t write_addr;

    if(!count){
        return RES_PARERR;
    }
    
        switch (pdrv) {
        case DEV_SPI_FLASH :
        {
            //扇区偏移2MB，外部Flash文件系统空间放在SPI Flash后面6MB空间
            sector += 512;
            write_addr = sector<<12;
            W25QXX_Write((u8 *)buff,write_addr,count<<12);
            
            stat = RES_OK;
            break;
        }
                
        default:
            stat = RES_PARERR;
        }

        return stat;
}



/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
        BYTE pdrv,                /* Physical drive nmuber (0..) */
        BYTE cmd,                /* Control code */
        void *buff                /* Buffer to send/receive control data */
)
{
        DRESULT stat = RES_PARERR;

        switch (pdrv) {
        case DEV_SPI_FLASH :
        {
            switch(cmd){
                //扇区数量：1536*4096/1024/1024=6(MB)
                case GET_SECTOR_COUNT:
                    *(DWORD* )buff = 1536;
                    break;
                //扇区大小
                case GET_SECTOR_SIZE:
                    *(WORD* )buff = 4096;
                    break;
                //同时擦除扇区个数
                case GET_BLOCK_SIZE :
                  *(DWORD * )buff = 1;
                    break;  
            }
            stat = RES_OK;
            break;
        }
        
        default:
            stat = RES_PARERR;
        }

        return stat;
}


DWORD get_fattime(void) {
        /* 返回当前时间戳 */
        return          ((DWORD)(2025 - 1980) << 25)        /* Year 2015 */
                        | ((DWORD)1 << 21)                                /* Month 1 */
                        | ((DWORD)1 << 16)                                /* Mday 1 */
                        | ((DWORD)0 << 11)                                /* Hour 0 */
                        | ((DWORD)0 << 5)                                  /* Min 0 */
                        | ((DWORD)0 >> 1);                                /* Sec 0 */
}
