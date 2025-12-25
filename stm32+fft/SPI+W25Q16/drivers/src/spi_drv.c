#include "spi_drv.h"


void SPI1_Config(void)
{
    //开时钟，SPI1，GPIOA 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_SPI1|RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC,ENABLE);
    
    //初始化GPIO
    GPIO_InitTypeDef GPIO_InitStruct;
    
    //SPI CS引脚
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_InitStruct);
    //SPI SCL MOSI引脚
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_InitStruct);
    //SPI MISO引脚
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	
    
    //SPI初始化  FLASH芯片 支持SPI模式0及模式3
    SPI_InitTypeDef SPI_InitStruct;
    //波特率分频系数 8分频 工作频率 72/8=9Mhz
    SPI_InitStruct.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_8;
    //时钟相位  第二个边沿（上升沿）采样
    SPI_InitStruct.SPI_CPHA=SPI_CPHA_2Edge;
    //时钟极性
    SPI_InitStruct.SPI_CPOL=SPI_CPOL_High;
    //CRC校验  stm32 spi带硬件ecc
    SPI_InitStruct.SPI_CRCPolynomial=7;
    //数据大小 一个字节8bit
    SPI_InitStruct.SPI_DataSize=SPI_DataSize_8b;
    //工作方式 双线全双工
    SPI_InitStruct.SPI_Direction=SPI_Direction_2Lines_FullDuplex;
    //数据传输顺序 高位在前
    SPI_InitStruct.SPI_FirstBit=SPI_FirstBit_MSB;
    //SPI主从模式选择  主模式
    SPI_InitStruct.SPI_Mode=SPI_Mode_Master;
    //片选 软件模拟控制
    SPI_InitStruct.SPI_NSS=SPI_NSS_Soft;
    SPI_Init(SPI1,&SPI_InitStruct);
    
    //拉高片选
    GPIO_SetBits(GPIOA,GPIO_Pin_13);
    //使能SPI
    SPI_Cmd(SPI1,ENABLE);
}


uint8_t SPI1_ReadWriteByte(uint8_t byte)
{
    /* 等待发送缓冲区为空，TXE事件 */
    while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE)==RESET);
    SPI_I2S_SendData(SPI1,byte);//写入数据寄存器，把要写入的数据写入发送缓冲区
    //等待接收缓冲区非空 RXNE事件
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
    //读取数据寄存器，获取接收缓冲区数据
    return SPI_I2S_ReceiveData(SPI1);
}

uint8_t SPI1_ReadByte(void)
{
    uint8_t byte=0;
    byte=SPI1_ReadWriteByte(0xff);
    
    return byte;
}

