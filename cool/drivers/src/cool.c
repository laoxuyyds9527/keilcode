#include "cool.h"



//初始化蜂鸣器

void cool_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	//开启外设时钟
	RCC_APB2PeriphClockCmd ( RCC_APB2Periph_GPIOC, ENABLE);
	//初始化gpio引脚
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_6;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init (GPIOC, &GPIO_InitStruct);

	GPIO_ResetBits (GPIOC,GPIO_Pin_6);

}




//是否开启制冷
void coolflag(u8 flag)
{

	if(flag)
	{
		GPIO_SetBits (GPIOB,GPIO_Pin_0);

	}
	else
	{
		GPIO_ResetBits (GPIOB,GPIO_Pin_0);
	}

}
 
