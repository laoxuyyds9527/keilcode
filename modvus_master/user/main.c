#include "stm32f10x.h"
#include "systick.h"
#include "usart_drv.h"
#include "system.h"
#include "tim.h"
#include <string.h>



void HardWare_Init(void)
{
	//优先级分组
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	//systick
	Systick_Init(72);
	//USART1初始化
	USART1_Init(115200);
	//UART4初始化
	USART4_Init(9600);
	//
	Tim6_init(100-1,720-1);
}


int main (void){
	
	HardWare_Init();
//	uint16_t data[2] = {0x0006,0x0007};
	
	while(1)
	{
//		MOdbus_WriteHoldingReg(0x01, 0x0000,0x0002,data);
//		Modbus_ReadCoils(0x01,0x0000,0x0004);
		Modbus_HoldingReg(0x01,0x0000,0x0002);
		delay_ms(300);
		if(uart_recv_end_flag)
		{
			for(uint16_t i = 0; i< uart_rx_count;i++)
				{
					printf("%02X ",Uart_Rx_Buff[i]);
				}
			memset(Uart_Rx_Buff,0,RX_BUF_SIZE);
			uart_rx_count = 0;
			printf("\r\n");
	    }
		
			
	}
}
