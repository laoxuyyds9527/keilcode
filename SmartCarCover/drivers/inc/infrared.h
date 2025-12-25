#ifndef __HX1838_H
#define __HX1838_H

#include "stm32f10x.h"                  // Device header
#include "systick.h"

#include "usart_drv.h"
// ==================== 引脚配置区域 ====================
// 在这里修改引脚定义，提高代码可移植性
#define HX1838_GPIO_PORT           GPIOA                    // 红外接收器连接的GPIO端口
#define HX1838_GPIO_PIN            GPIO_Pin_2               // 红外接收器连接的GPIO引脚
#define HX1838_GPIO_PIN_SOURCE     GPIO_PinSource2          // GPIO引脚源
#define HX1838_EXTI_LINE           EXTI_Line2               // 外部中断线
#define HX1838_EXTI_IRQn           EXTI2_IRQn               // 外部中断IRQ通道
#define HX1838_GPIO_CLK            RCC_APB2Periph_GPIOA     // GPIO时钟
#define HX1838_AFIO_CLK            RCC_APB2Periph_AFIO      // AFIO时钟


// 中断优先级配置
#define HX1838_IRQ_PREEMPT_PRIORITY    0                    // 抢占优先级
#define HX1838_IRQ_SUB_PRIORITY        1                    // 响应优先级
extern u32 hw_jsm;
// 中断处理函数宏定义
#define HX1838_IRQ_HANDLER         EXTI2_IRQHandler         // 中断处理函数名

// ==================== 常量定义 ====================
#define HX1838_KEY_INVALID         17                        // 无效按键值

// ==================== 函数声明 ====================
void HX1838_Init (void);
u8 HX1838_RecordHightTime (void);
u8 HX1838_ReadKeyValue (void);


#endif
