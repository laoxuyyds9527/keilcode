/********************************************************************************
* @file    		gpio.c
* @function     GPIO配置
* @brief 		用3.5.0版本库建的工程模板
* @attention	该例程仅作为参考，如需实际开发，请根据实际需求更改
* @version 		V1.1
* @company  	深圳市飞思创电子科技有限公司
* @website  	http://www.freestrong.com
* @tel			0755-86528386
* @Author       freestrong
* @date    		2020/09/20
********************************************************************************/

#include "gpio.h"

/**
 * @description: 初始化PA0引脚控制模组PEN引脚
 * @param None
 * @return None
 */
void pen_gpio_init(void)
{
 
  GPIO_InitType  GPIO_InitStruct;

  RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOB, ENABLE);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP ;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.Pin = GPIO_PIN_15;
	GPIO_InitPeripheral(GPIOB, & GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP ;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.Pin = GPIO_PIN_12;
	GPIO_InitPeripheral(GPIOB, & GPIO_InitStruct);
	
	GPIO_SetBits(GPIOB,GPIO_PIN_15);
	GPIO_SetBits(GPIOB,GPIO_PIN_12);
}
