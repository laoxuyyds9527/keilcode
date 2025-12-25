#include "stm32f1xx_hal.h"
#include <stdio.h>
#include "keyinter.h"

#define KEY_1_PIN    GPIO_PIN_0
#define KEY_2_PIN    GPIO_PIN_1
#define KEY_3_PIN    GPIO_PIN_4
#define KEY_GPIO_PORT GPIOA



void GPIO_Key_Init(void);
void EXTI_Key_Init(void);
Key_ValueTypeDef Key_Scan(uint8_t mode);
void delay_ms(uint32_t ms);


void GPIO_Key_Init(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    
    __HAL_RCC_GPIOA_CLK_ENABLE();

    // PA0:KEY_1
    GPIO_InitStruct.Pin = KEY_1_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;  // ????
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(KEY_GPIO_PORT, &GPIO_InitStruct);

    // PA1:KEY_2PA4:KEY_3,
    GPIO_InitStruct.Pin = KEY_2_PIN | KEY_3_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;    // ????
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(KEY_GPIO_PORT, &GPIO_InitStruct);
}

//EXTI_Key_Init
 
void EXTI_Key_Init(void) {
    EXTI_InitTypeDef EXTI_InitStruct = {0};
    NVIC_InitTypeDef NVIC_InitStruct = {0};

   
    __HAL_RCC_AFIO_CLK_ENABLE();

    
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource1);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource4);

    
    EXTI_InitStruct.Line = EXTI_Line0;
    EXTI_InitStruct.Mode = EXTI_Mode_Interrupt;
    EXTI_InitStruct.Trigger = EXTI_Trigger_Rising;  
    EXTI_InitStruct.Enabled = ENABLE;
    HAL_EXTI_Init(&EXTI_InitStruct);

    
    EXTI_InitStruct.Line = EXTI_Line1;
    EXTI_InitStruct.Trigger = EXTI_Trigger_Falling;  

    // 4. 
    EXTI_InitStruct.Line = EXTI_Line4;
    EXTI_InitStruct.Trigger = EXTI_Trigger_Falling;
    HAL_EXTI_Init(&EXTI_InitStruct);

    // 5. 
    // EXTI0??(KEY_1)
    NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;  
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;         
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    HAL_NVIC_Init(&NVIC_InitStruct);

    // EXTI1??(KEY_2)
    NVIC_InitStruct.NVIC_IRQChannel = EXTI1_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
    HAL_NVIC_Init(&NVIC_InitStruct);

    // EXTI4??(KEY_3)
    NVIC_InitStruct.NVIC_IRQChannel = EXTI4_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;
    HAL_NVIC_Init(&NVIC_InitStruct);
}


Key_ValueTypeDef Key_Scan(uint8_t mode) 
{
    static uint8_t key_flag = 1;  // ??????(????,????)
    Key_ValueTypeDef key_val = KEY_NONE;

    // ????????
    if (mode == 1) {
        key_flag = 1;  // ??????,????????
    }

    if (key_flag && (HAL_GPIO_ReadPin(KEY_GPIO_PORT, KEY_1_PIN) == GPIO_PIN_SET ||
                    HAL_GPIO_ReadPin(KEY_GPIO_PORT, KEY_2_PIN) == GPIO_PIN_RESET ||
                    HAL_GPIO_ReadPin(KEY_GPIO_PORT, KEY_3_PIN) == GPIO_PIN_RESET)) {
        delay_ms(20);  // ????
        key_flag = 0;  // ???????,??????

        // ??????
        if (HAL_GPIO_ReadPin(KEY_GPIO_PORT, KEY_1_PIN) == GPIO_PIN_SET) {
            key_val = KEY_1_PRESS;
            printf("KEY_1 ????\r\n");
        } else if (HAL_GPIO_ReadPin(KEY_GPIO_PORT, KEY_2_PIN) == GPIO_PIN_RESET) {
            key_val = KEY_2_PRESS;
            printf("KEY_2 ????\r\n");
        } else if (HAL_GPIO_ReadPin(KEY_GPIO_PORT, KEY_3_PIN) == GPIO_PIN_RESET) {
            key_val = KEY_3_PRESS;
            printf("KEY_3 ????\r\n");
        }
    } else if (HAL_GPIO_ReadPin(KEY_GPIO_PORT, KEY_1_PIN) == GPIO_PIN_RESET &&
               HAL_GPIO_ReadPin(KEY_GPIO_PORT, KEY_2_PIN) == GPIO_PIN_SET &&
               HAL_GPIO_ReadPin(KEY_GPIO_PORT, KEY_3_PIN) == GPIO_PIN_SET) {
        key_flag = 1;  // ??????,????
    }

    return key_val;
}

void EXTI0_IRQHandler(void) {
    if (HAL_EXTI_GetITSource(&htim1, EXTI_Line0) != RESET) {
        delay_ms(20);  // ????
        if (HAL_GPIO_ReadPin(KEY_GPIO_PORT, KEY_1_PIN) == GPIO_PIN_SET) {
            g_key_value = KEY_1_PRESS;
            printf("KEY_1 ????\r\n");
        }
        HAL_EXTI_ClearITPendingBit(&htim1, EXTI_Line0);  // ???????
    }
}


void EXTI1_IRQHandler(void) {
    if (HAL_EXTI_GetITSource(&htim1, EXTI_Line1) != RESET) {
        delay_ms(20);  // ????
        if (HAL_GPIO_ReadPin(KEY_GPIO_PORT, KEY_2_PIN) == GPIO_PIN_RESET) {
            g_key_value = KEY_2_PRESS;
            printf("KEY_2 ????\r\n");
        }
        HAL_EXTI_ClearITPendingBit(&htim1, EXTI_Line1);  // ???????
    }
}


void EXTI4_IRQHandler(void) {
    if (HAL_EXTI_GetITSource(&htim1, EXTI_Line4) != RESET) {
        delay_ms(20);  // ????
        if (HAL_GPIO_ReadPin(KEY_GPIO_PORT, KEY_3_PIN) == GPIO_PIN_RESET) {
            g_key_value = KEY_3_PRESS;
            printf("KEY_3 ????\r\n");
        }
        HAL_EXTI_ClearITPendingBit(&htim1, EXTI_Line4);  // ???????
    }
}




int main(void) {
    HAL_Init();
    SystemClock_Config();  // 
    MX_USART1_UART_Init(); // 
    GPIO_Key_Init();       // 
    EXTI_Key_Init();       // 

    while (1) {
        // 
        if (g_key_value != KEY_NONE) {
            
            g_key_value = KEY_NONE; 
        }

        // 
        Key_Scan(0);

        HAL_Delay(10); 
    }
}