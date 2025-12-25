#ifndef __GPIO_H
#define __GPIO_H

#include "n32wb452.h"

#define PEN_GPIO_SET_HIGH  GPIO_SetBits(GPIOA, GPIO_Pin_0)  //PEN引脚设置为高电平
#define PEN_GPIO_SET_LOW  GPIO_ResetBits(GPIOA, GPIO_Pin_0) //PEN引脚设置为低电平

void pen_gpio_init(void);   //PEN引脚初始化

#endif


