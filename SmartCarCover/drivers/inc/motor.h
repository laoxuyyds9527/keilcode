#ifndef __MOTOR__H
#define __MOTOR__H

#include "stm32f10x.h"
#include "systick.h"
#include "usart_drv.h"

#define motor_a GPIO_Pin_6
#define motor_b GPIO_Pin_7
#define motor_c GPIO_Pin_8
#define motor_d GPIO_Pin_9
#define motor_port GPIOB

#define up_a GPIO_SetBits (GPIOB,GPIO_Pin_6)
#define down_a GPIO_ResetBits (GPIOB, GPIO_Pin_6)
#define up_b GPIO_SetBits (GPIOB,GPIO_Pin_7)
#define down_b GPIO_ResetBits (GPIOB, GPIO_Pin_7)
#define up_c GPIO_SetBits (GPIOB,GPIO_Pin_8)
#define down_c GPIO_ResetBits (GPIOB,GPIO_Pin_8)
#define up_d GPIO_SetBits (GPIOB,GPIO_Pin_9)
#define down_d GPIO_ResetBits (GPIOB, GPIO_Pin_9)


void Motor(void);
void bites_4(u16 step,u32 delay_xx);
void turn(u16 quan);
void bites_8(u16 step);





#endif


