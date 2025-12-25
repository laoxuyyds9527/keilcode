#ifndef __KEY_DRV_H
#define __KEY_DRV_H

#include "stm32f10x.h"
#include "systick.h"
#include "system.h"

enum{
	KEY_NO,
	KEY_1,
	KEY_2,
	KEY_3
};



void Key_Init(void);
u8 Key_Scan(u8 mode);

#endif

