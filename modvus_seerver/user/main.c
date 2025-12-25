#include "led_drv.h"
#include "usart_drv.h"
#include "systick.h"
#include "tim_drv.h"
#include "modbus.h"
#include "crc16.h"

#include <string.h>

#define SLAVE_ID 0x01

void HardWare_Init(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    SysTick_Init(72);
    LED_Init();
    Usart_MyConfig(115200);
    RS485_Init(9600);
    TIM_Config(100-1,720-1); //1ms定时
}


int main(void)
{
    HardWare_Init();
    
//    uint8_t prasebuff[RX_BUF_SIZE] = {0};
//    uint16_t prase_len = 0;
    
    uint16_t data[2] = {0x0005,0x000a};
    uint8_t state[] = {0x00,0x00,0x01,0x01,0x00,0x01,0x01,0x01,0x01};
    while (1)
    {
//        Modbus_ReadCoils(SLAVE_ID,0x0000,0x0004);
//        Modbus_WriteCoils(SLAVE_ID,0x0000,0x0004,state);
//        Modbus_ReadHoldingReg(SLAVE_ID,0x0000,0x0002);
//        Modbus_WriteHoldingReg(SLAVE_ID,0x0000,0x0002,data);
//        delay_ms(300);
//        memset(prasebuff,0,RX_BUF_SIZE);
//        prase_len = 0;
        
        if(uart_recv_end_flag)
        {
            printf("我是从机：\r\n");
            uint8_t tx_buf[48] = {0};
            uint8_t len = 0;
            
//            prase_len = uart_rx_count;
//            memcpy(prasebuff,Uart_Rx_Buff,prase_len);
              
            for(uint16_t i =0; i < uart_rx_count; i++)
            {
                printf("%02X ",Uart_Rx_Buff[i]);
            }
            printf("\r\n");
            
            uint16_t rec_crc = Uart_Rx_Buff[uart_rx_count-1] <<8 | Uart_Rx_Buff[uart_rx_count-2];
            uint16_t crc = Get_CRC16(Uart_Rx_Buff,uart_rx_count-2);
            
            //判断数据校验是否正确
            if(crc != rec_crc)
            {
                printf("crc 校验出错\r\n");
            }
            
            //判断从机地址是否正常
            if(Uart_Rx_Buff[0] != SLAVE_ID)
            {
                printf("设备地址错误\r\n");
            }
            
            uint8_t cmd = Uart_Rx_Buff[1]; //接收到的功能码
            uint16_t addr = (Uart_Rx_Buff[2]<<8)|Uart_Rx_Buff[3];  //接收到的寄存器地址
            uint16_t tx_crc = 0;
            switch(cmd)
            {
                printf("功能码解析\r\n");
                case 0x01:
                {
                    uint16_t reg_num = Uart_Rx_Buff[4]<<8|Uart_Rx_Buff[5];
                    uint8_t bytes = 0;
                    uint16_t index = 0;
                    uint8_t data = 0;
                    
                    bytes = reg_num/8;  //读取的字节数
                    if(reg_num%8 !=0)
                        bytes++;
                    
                    if(addr >= REG_COIL_ADDR_BASE && (addr + reg_num) <= REGX_COIL_ADDR) //判断读取的寄存器是否合法
                    {
                        //构建响应数据
                        tx_buf[len++] = Uart_Rx_Buff[0]; //从机地址
                        tx_buf[len++] = cmd; //响应功能码
                        tx_buf[len++] = bytes; //读取的字节数
                        
                        for(uint8_t j = 0; j < bytes; j++)
                        {
                            for(uint8_t i = 0; i < 8; i++) //按字节将线圈寄存器的状态值进行封装
                            {
                                index = i + j * 8;
                                data >>=0x01;
                                
                                if(index < reg_num)  //判断读取的数据个数是否大于要读的寄存器个数
                                {
                                    if(state[index] != 0)
                                    {
                                        data |= 0x80;
                                    }
                                }
                                
                            }
                            tx_buf[len++] = data;
                        }
                        
                    }
                    else
                    {
                        tx_buf[len++] = Uart_Rx_Buff[0]; //从机地址
                        tx_buf[len++] = cmd|0x80; //响应功能码
                        tx_buf[len++] = 0x02; //异常码
                    }
                    
                    tx_crc = Get_CRC16(tx_buf,len); //计算响应数据的CRC校验值
                    tx_buf[len++] = tx_crc;
                    tx_buf[len++] = tx_crc>>8;
                    RS485_SendBuff(RS485_USART,tx_buf,len);
                    break;
                }
                
                case 0x03:
                {
                    uint16_t reg_num = Uart_Rx_Buff[4]<<8|Uart_Rx_Buff[5];
                    uint8_t bytes = 0;
                    
                    bytes = reg_num * 2;
                    if(addr >= REG_HOLD_ADDR_BASE && (addr + reg_num) <= REGX_HOLD_ADDR)
                    {
                        //构建响应数据
                        tx_buf[len++] = Uart_Rx_Buff[0]; //从机地址
                        tx_buf[len++] = cmd; //响应功能码
                        tx_buf[len++] = bytes; //读取的字节数
                        
                        for(uint8_t i = 0; i < reg_num; i++)
                        {
                            tx_buf[len++] = data[i]>>8;
                            tx_buf[len++] = data[i];
                        }
                    }
                    else
                    {
                        tx_buf[len++] = Uart_Rx_Buff[0]; //从机地址
                        tx_buf[len++] = cmd|0x80; //响应功能码
                        tx_buf[len++] = 0x02; //异常码
                    }
                    tx_crc = Get_CRC16(tx_buf,len); //计算响应数据的CRC校验值
                    tx_buf[len++] = tx_crc;
                    tx_buf[len++] = tx_crc>>8;
                    RS485_SendBuff(RS485_USART,tx_buf,len);
                    break;
                }
                
                case 0x0F:
                {
                    uint16_t reg_num = Uart_Rx_Buff[4]<<8|Uart_Rx_Buff[5];
                    
                    if(addr >= REG_COIL_ADDR_BASE && (addr + reg_num) <= REGX_COIL_ADDR) //判断读取的寄存器是否合法
                    {
                        //构建响应数据
                        tx_buf[len++] = Uart_Rx_Buff[0]; //从机地址
                        tx_buf[len++] = cmd; //响应功能码
                        tx_buf[len++] = Uart_Rx_Buff[2];  //寄存器起始地址
                        tx_buf[len++] = Uart_Rx_Buff[3];
                        tx_buf[len++] = Uart_Rx_Buff[4]; //寄存器个数
                        tx_buf[len++] = Uart_Rx_Buff[5];
                    }
                    else
                    {
                        tx_buf[len++] = Uart_Rx_Buff[0]; //从机地址
                        tx_buf[len++] = cmd|0x80; //响应功能码
                        tx_buf[len++] = 0x02; //异常码
                    }
                    tx_crc = Get_CRC16(tx_buf,len); //计算响应数据的CRC校验值
                    tx_buf[len++] = tx_crc;
                    tx_buf[len++] = tx_crc>>8;
                    RS485_SendBuff(RS485_USART,tx_buf,len);
                    break;
                }
                
                case 0x10:
                {
                    uint16_t reg_num = Uart_Rx_Buff[4]<<8|Uart_Rx_Buff[5];
                    if(addr >= REG_HOLD_ADDR_BASE && (addr + reg_num) <= REGX_HOLD_ADDR)
                    {
                        //构建响应数据
                        tx_buf[len++] = Uart_Rx_Buff[0]; //从机地址
                        tx_buf[len++] = cmd; //响应功能码
                        tx_buf[len++] = Uart_Rx_Buff[2];  //寄存器起始地址
                        tx_buf[len++] = Uart_Rx_Buff[3];
                        tx_buf[len++] = Uart_Rx_Buff[4]; //寄存器个数
                        tx_buf[len++] = Uart_Rx_Buff[5];
                    }
                    else
                    {
                        tx_buf[len++] = Uart_Rx_Buff[0]; //从机地址
                        tx_buf[len++] = cmd|0x80; //响应功能码
                        tx_buf[len++] = 0x02; //异常码
                    }
                    tx_crc = Get_CRC16(tx_buf,len); //计算响应数据的CRC校验值
                    tx_buf[len++] = tx_crc;
                    tx_buf[len++] = tx_crc>>8;
                    RS485_SendBuff(RS485_USART,tx_buf,len);
                    break;
                }
                
                default:
                {
                    tx_buf[len++] = Uart_Rx_Buff[0]; //从机地址
                    tx_buf[len++] = cmd|0x80; //响应功能码
                    tx_buf[len++] = 0x01; //异常码
                    tx_crc = Get_CRC16(tx_buf,len); //计算响应数据的CRC校验值
                    tx_buf[len++] = tx_crc;
                    tx_buf[len++] = tx_crc>>8;
                    RS485_SendBuff(RS485_USART,tx_buf,len);
                    break;
                }
                    
            }
            
            uart_recv_end_flag = 0;
            memset(Uart_Rx_Buff,0,RX_BUF_SIZE);
            uart_rx_count = 0;
        }
    }
}

