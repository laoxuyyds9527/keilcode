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

typedef enum{
	KEY_TYPE_NO = 0,
	KEY_TYPE_SHORT,
	KEY_TYPE_LONG,
	KEY_TYPE_DOUBLE
}KEY_TYPE;

typedef enum{
	KEY_STATE_IDLE = 0,
	KEY_STATE_SHAKE,
	KEY_STATE_PERSS,
	KEY_STATE_RELEASE,
	KEY_STATE_WAIT_DOUBLE
}KEY_STATE;
extern u8 key_val;
extern u8 key_press_cnt;
extern u8 key_shake_cnt;
extern u8 double_wait_cnt ;
#define KEY_PRESS (PAin(0))
#define KEY_RELEASE (!PAin(0))
void Key_Init(void);
u8 Key_Scan(u8 mode);
u8 State_Machine(void);

#endif

