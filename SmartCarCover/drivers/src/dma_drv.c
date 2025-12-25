#include "dma_drv.h"

u16 Conv_Val[2];

/**
 * @brief DMA1通道1初始化
 * @param 无
 * @retval 无
 */
void DMA1_CH1_Init(void)
{
	DMA_InitTypeDef DMA_InitStruct;

	//开启外设时钟
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	//DMA初始化
	DMA_InitStruct.DMA_BufferSize = 2;  //数据传输的个数
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC;  //数据搬运方向,外设寄存器作为数据源
	DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;  //存储器到存储器模式
	DMA_InitStruct.DMA_MemoryBaseAddr = (u32)Conv_Val;  //存储器地址
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;  //数据宽度,16位
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;  //存储器地址自增
	DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;  //循环搬运模式
	DMA_InitStruct.DMA_PeripheralBaseAddr = (u32)&ADC1->DR;
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;  //外设数据寄存器数据宽度
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //外设地址不自增
	DMA_InitStruct.DMA_Priority = DMA_Priority_VeryHigh;  //优先级
	DMA_Init(DMA1_Channel1, &DMA_InitStruct);

	//开启DMA
	DMA_Cmd(DMA1_Channel1, ENABLE);
}

