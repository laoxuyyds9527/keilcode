#ifndef __LED_DRV_H__
#define __LED_DRV_H__

#include "stm32f10x.h"


//LED×´Ì¬
#define LED_ON  1
#define LED_OFF 0

//LED ±àºÅ
typedef enum{
    LED0,
    LED1,
    
    LED_MAX
}LED_Dev;


void LED_Init(void);
void LED_Ctrl(LED_Dev led_num,uint8_t led_state);
#endif
