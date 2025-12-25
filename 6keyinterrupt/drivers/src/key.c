#include "key.h"


void key (void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	//配置时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	//配置GPIOpa0
	GPIO_InitStruct.GPIO_Mode= GPIO_Mode_IPD;
	GPIO_InitStruct.GPIO_Pin= GPIO_Pin_0;  
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	//GPIOPA1和4
	GPIO_InitStruct.GPIO_Mode= GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin= GPIO_Pin_1|GPIO_Pin_4;  
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	
	
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
}



//按键扫描
//mode的0单词模式，1连续模式
u8 keyscan(u8 mode)
{
	u8 keyval=KEY_NO;
	static u8 flag=1;
	if(mode=1)
	{
		flag=1;		
	}
	if ((PAin(0)||!PAin(1)||!PAin(4)&&flag)
	{
		delya_ms(20);
		flag=0;
		if(PAin(0))
		{
			keyval=KEY_1;
		}
		if(PAin(1))
		{
			keyval=KEY_2;
		}
		if(PAin(4))
		{
			keyval=KEY_3;
		}
		else id(!PAin(0)&&!PAin(1)&&!PAin(4))
		{
			flag=1;
		}
	}



	return keyval;
}
