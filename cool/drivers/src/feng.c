#include "feng.h"



//初始化蜂鸣器

void feng_ming(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	//开启外设时钟
	RCC_APB2PeriphClockCmd ( RCC_APB2Periph_GPIOB, ENABLE);
	//初始化gpio引脚
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_15;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init (GPIOB, &GPIO_InitStruct);

	GPIO_ResetBits (GPIOB,GPIO_Pin_15);

}




//是否有人碰到到了
void baojin(u8 flag)
{

	if(flag)
	{
		GPIO_SetBits (GPIOB,GPIO_Pin_15);
		for(u16 i=0;i<2;i++)
		{
			delay_ms (10);
		}
		GPIO_ResetBits (GPIOB,GPIO_Pin_15);
	}
	else
	{
		GPIO_ResetBits (GPIOB,GPIO_Pin_15);
	}

}
 
