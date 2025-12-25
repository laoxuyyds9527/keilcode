#ifndef __INFRARED__H
#define __INFRARED__H

#include "stm32f10x.h"

#define HX1838_GPIO_PORT    GPIOA //红外接收器的gpio引脚
#define HX1838_GPIO_PIN		GPIOA_PIN_6 //
#define HX1838_GPIO_PIN_SOUREC  GPIO_PinSource6//gpio引脚源
#define HX1838_GPIO_LINE    EXTI_Line1//外部中断线
#define HX1838_GPIO_IRQN 	EXTI1_IRQn//外部中断irq通道





#endif
