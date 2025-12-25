#ifndef __USART_DRV_H__
#define __USART_DRV_H__

#include "stm32f10x.h"
#include <stdio.h>

#define RS485_USE_UART4  1

#if RS485_USE_UART4

#define RS485_USART     UART4

#define RS485_TX_PIN   GPIO_Pin_10
#define RS485_RX_PIN   GPIO_Pin_11 

#define RS485_TX_PORT   GPIOC
#define RS485_RX_PORT   GPIOC

#define RS485_RX_EN_PIN     
#define RS485_RX_EN_PORT    

#define RS485_RX_EN
#define RS485_TX_EN

#define RS485_Init(x)    USART4_Init(x)

#else

#define RS485_USART     USART2

#define RS485_TX_PIN   GPIO_Pin_2
#define RS485_RX_PIN   GPIO_Pin_3 

#define RS485_TX_PORT   GPIOA
#define RS485_RX_PORT   GPIOA

#define RS485_RX_EN_PIN     GPIO_Pin_3
#define RS485_RX_EN_PORT    GPIOG

#define RS485_RX_EN    GPIO_ResetBits(RS485_RX_EN_PORT,RS485_RX_EN_PIN)
#define RS485_TX_EN    GPIO_SetBits(RS485_RX_EN_PORT,RS485_RX_EN_PIN)

#define RS485_Init(x)    USART2_Init(x)

#endif

#define RX_BUF_SIZE 256

extern uint8_t Uart_Rx_Buff[RX_BUF_SIZE];
extern uint16_t uart_rx_count;

extern uint8_t uart_recv_start_flag;
extern uint8_t uart_recv_end_flag;
extern uint8_t time_out_count;

void Usart_MyConfig(u32 baudrate);
void USART4_Init(uint32_t baud);
void USART2_Init(uint32_t baud);
void RS485_SendByte(USART_TypeDef* USARTx,uint8_t data);
void RS485_SendBuff(USART_TypeDef* USARTx,uint8_t *buff,uint16_t len);

#endif
