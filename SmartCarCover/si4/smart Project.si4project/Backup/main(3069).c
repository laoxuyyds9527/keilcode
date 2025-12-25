#include "stm32f10x.h"
#include "systick.h"
#include "usart_drv.h"
#include "system.h"
#include "adc_drv.h"
#include "feng.h"
#include "dma_drv.h"

void HardWare_Init(void)
{
	//优先级分组
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	//systick
	Systick_Init(72);
	//USART1初始化
	USART1_Init(115200);
	//初始化振动传感器
	ADC1_CH2_Init();
	//初始化电源检测
	ADC1_CH3_Init();
	//初始化蜂鸣器
	feng_ming();
	//dma
	DMA1_CH1_Init();
}


int main (void){
	
	HardWare_Init();
	u16 vbira=0;
	u16 power=0;
	u16 flag=0;
	baojin(flag);
	while(1)
	{
		printf("振动强度 = %d\n",Get_ConvVal());
		vbira=Get_ConvVal();
		if(vbira==0)
		{
		
			printf("有人动车子了\n");
		
		}
		
		
		printf("电压强度 = %d\n",Get_Power());
		delay_ms(1000);
	}
}
