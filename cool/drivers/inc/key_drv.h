#ifndef __KEY_DRV_H
#define __KEY_DRV_H

#include "stm32f10x.h"
#include "systick.h"
#include "system.h"
#include "usart_drv.h"

enum{
	KEY_NO,
	KEY_1,
	KEY_2,
	KEY_3
};

extern u8 key_val;
extern u16 Flame_threshold;
extern u8 fire_off;
void Key_Init(void);
u8 Key_Scan(u8 mode);

#endif

