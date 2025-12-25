#ifndef __USART_DRV_H
#define __USART_DRV_H

#include "stm32f10x.h"
#include <stdio.h>
#include "tim.h"

#include "stm32f10x.h"
#include <stdio.h>
#include "tim.h"
void USART4_Init(uint32_t baud);
void USART1_Init(u32 baudrate);
void EXTI4_IRQHandler(void);
void RS485_SendByte(USART_TypeDef* USARTx,uint8_t data);
void RS485_SendBuff(USART_TypeDef* USARTx,uint8_t *buff,uint16_t len);

void Modbus_ReadCoils(uint8_t slave_id,uint16_t reg_addr,uint16_t reg_num);
void Modbus_HoldingReg(uint8_t slave_id,uint16_t reg_addr,uint16_t reg_num);
void MOdbus_WriteHoldingReg(uint8_t slave_id,uint16_t reg_addr,uint16_t reg_num,uint16_t *data);

#define RX_BUF_SIZE 256
extern uint8_t Uart_Rx_Buff[RX_BUF_SIZE] ;
extern uint16_t uart_rx_count;
extern uint8_t uart_recv_start_flag;
extern uint8_t uart_recv_end_flag;
extern uint8_t time_out_count;
#endif
