#ifndef __LED_DRV_H
#define __LED_DRV_H

/* 头文件 */
#include "stm32f10x.h"


/* 宏定义 */
#define LED_1 		1
#define LED_ON 		1
#define LED_OFF		0


/* 函数声明 */
void LED_Init(void);
void LED_Ctrl(u8 led_choose, u8 led_state);


#endif


