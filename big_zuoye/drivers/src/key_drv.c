#include "key_drv.h"


/**
 * @brief 按键初始化
 * @param 无
 * @retval 无
 */
void Key_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	//配置时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	//配置引脚
	//PA0
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD;  //下拉输入
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	//PA1 PA4
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;  //上拉输入
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_4;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
}


/**
 * @brief 按键扫描
 * @param mode:0单次检测模式，1连续检测模式
 * @retval 被按下的按键
 */
u8 Key_Scan(u8 mode)
{
	u8 key_val = KEY_NO;
	static u8 key_flag = 1;  //用来表示按键有没有被按下0表示被按下 1表示没有按下
	if(mode == 1)
		key_flag = 1;
	
	if((PAin(0) || !PAin(1) || !PAin(4)) && key_flag)
	{
		delay_ms(20);
		key_flag = 0;
		if(PAin(0))
		{
			key_val = KEY_1;
		}
		if(!PAin(1))
		{
			key_val = KEY_2;
		}
		if(!PAin(4))
		{
			key_val = KEY_3;
		}
	}
	else if(!PAin(0) && PAin(1) && PAin(4))
	{
		key_flag = 1;
	}
	return key_val;
}
