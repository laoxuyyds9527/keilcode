#include "stm32f10x.h"
#include "rcc.h"


/*
*@brief 手动设置系统时钟的频率
*@retval 没有
*
*/

void Rcc_MyData(uint32_t pllsrc,uint32_t pllmul)
{
	//将rcc寄存器设置为默认值
	RCC_DeInit();
	//使能外部高速晶振
	RCC_HSEConfig(RCC_HSE_ON);
	//等待外部高速晶振起振
	while(RCC_WaitForHSEStartUp()!= SUCCESS);
	//使能预取缓冲
	FLASH_PrefetchBufferCmd( FLASH Prefetch Buffer Enable);
	
	//设置两个等待周期
	FLASH_SetLatency(FLASH_Latency_2);
	//设置锁相环时钟源以及倍频系数
	RCC_PLLConfig(pllsrc, pllmul);
	//设置AHB分配系数
	RCC_HCLKConfig(RCC_SYSCLK_Div1);
	//设置APB1分频系数
	RCC_PCLK1Config(RCC_HCLK_Div2);
	//设置APB2分频系数
	RCC_PCLK2Config(RCC_HCLK_Div1);
	//使能锁相环时钟
	RCC_PLLCmd(ENABLE);
	//使能锁相环时钟是否就绪
	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) != SET);
	//将系统时钟元设置为锁相环时钟
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
	//检查系统时钟是否锁相环时钟
	while(RCC_GetSYSCLKSource() != 0x08);


}
