#include "usart_drv.h"
#include "spi_drv.h"
#include "w25qxx_drv.h"
#include "systick.h"
#include "ff.h"
#include "ffconf.h"

FATFS fs;           //fatfs文件系统对象
FIL fnew;           //文件对象
FRESULT res_flash;  //文件操作结果
UINT fnum;          //文件成功读写的数量
BYTE ReadBuffer[1024] = {0};  //读缓冲区
BYTE WriteBuffer[] = "fafts test\r\n";  //写缓冲区

static BYTE work[FF_MAX_SS];  //格式化操作所需的缓冲区大小，至少为FF_MAX_SS

void HardWare_Init(void)
{
    SysTick_Init(72);
        Usart_MyConfig(115200);
    SPI1_Config();
}

int main(void)
{
    HardWare_Init();
    //在外部SPI Flash挂载文件系统，文件系统挂载时会对SPI设备初始
	printf("%d\r\n",W25QXX_ReadID());
    res_flash = f_mount(&fs,"0:",1);
    //如果没有文件系统就格式化创建创建文件系统
    if(res_flash == FR_NO_FILESYSTEM)
    {
        printf("flash还没有文件系统，即将进行格式化...\r\n");
        
        //格式化
        res_flash = f_mkfs("0:",0,work,sizeof(work));
        if(res_flash == FR_OK)
        {
            printf("flash已成功格式化文件系统\r\n");
            
            res_flash = f_mount(NULL,"0:",1);
            res_flash = f_mount(&fs,"0:",1);
        }
        else
        {
            printf("格式化失败\r\n");
        }
    }
    else if(res_flash != FR_OK)
    {
        printf("外部flash挂载文件系统失败\r\n");
    }
    else
        printf("外部flash挂载文件系统成功\r\n");
    
        /* 打开文件，每次都以新建的形式打开，属性为可写 */
        printf("\r\n****** 即将进行文件写入测试... ******\r\n");        
        res_flash = f_open(&fnew, "0:FatFs读写测试.txt",FA_CREATE_ALWAYS | FA_WRITE );

        if ( res_flash == FR_OK )
        {
                printf("打开/创建FatFs读写测试文件.txt文件成功，向文件写入数据。\r\n");
        /* 将指定存储区内容写入到文件内 */
                res_flash=f_write(&fnew,WriteBuffer,sizeof(WriteBuffer),&fnum);
        if(res_flash==FR_OK)
        {
            printf("文件写入成功，写入字节数据：%d\n",fnum);
            printf("向文件写入的数据为：\r\n%s\r\n",WriteBuffer);
        }
        else
        {
            printf("文件写入失败：(%d)\n",res_flash);
        }    
            /* 不再读写，关闭文件 */
        f_close(&fnew);
        }
        else
        {        
                printf("打开/创建文件失败\r\n");
        }
    
    printf("****** 即将进行文件读取测试... ******\r\n");
    res_flash = f_open(&fnew, "0:FatFs读写测试.txt",FA_OPEN_EXISTING | FA_READ);

    if(res_flash == FR_OK)
    {
        printf("打开文件成功\r\n");
        res_flash = f_read(&fnew, ReadBuffer, sizeof(ReadBuffer), &fnum); 
        if(res_flash==FR_OK)
        {
            printf("文件读取成功,读到字节数据：%d\r\n",fnum);
            printf("读取得的文件数据为：\r\n%s \r\n", ReadBuffer);        
        }
        else
        {
            printf("文件读取失败：(%d)\n",res_flash);
        }                
    }
    else
    {
        printf("打开文件失败\r\n");
    }
    /* 不再读写，关闭文件 */
    f_close(&fnew);        

    /* 不再使用文件系统，取消挂载文件系统 */
    f_mount(NULL,"0:",1);
    
    while(1)
    {
            
    }
}

