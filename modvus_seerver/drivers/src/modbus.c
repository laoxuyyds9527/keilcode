#include "modbus.h"
#include "crc16.h"
#include "usart_drv.h"


/**********************************************************************************
 * @brief : 读线圈寄存器
 *                
 * @param : slave_id 从机地址
 * @param : reg_addr 线圈寄存器起始地址
 * @param : reg_num 线圈寄存器的个数
 * @retval  :
**********************************************************************************/
void Modbus_ReadCoils(uint8_t slave_id,uint16_t reg_addr,uint16_t reg_num)
{
    uint8_t buf[8] = {0};
    buf[0] = slave_id; //从机地址
    buf[1] = 0x01;     //功能码
    buf[2] = reg_addr>>8; //线圈寄存器起始地址
    buf[3] = reg_addr;
    buf[4] = reg_num>>8;  //线圈寄存器的个数
    buf[5] = reg_num;
    
    uint16_t crc = Get_CRC16(buf,6);   //crc校验结果
    buf[6] = crc;
    buf[7] = crc>>8;
    
    RS485_SendBuff(RS485_USART,buf,8);
}


/**********************************************************************************
 * @brief : 写多个线圈寄存器
 *                
 * @param : slave_id 从机地址
 * @param : reg_addr 线圈寄存器起始地址
 * @param : reg_num 线圈寄存器的个数
 * @param : status 线圈寄存器的状态值
 * @retval  :
**********************************************************************************/
void Modbus_WriteCoils(uint8_t slave_id,uint16_t reg_addr,uint16_t reg_num,uint8_t *status)
{
    uint8_t buf[48] = {0};
    uint8_t len = 0;  //记录整个数据帧的长度
    uint8_t bytes = 0;
    uint8_t data = 0;
    uint16_t index = 0;
    
    buf[len++] = slave_id; //从机地址
    buf[len++] = 0x0F;     //功能码
    buf[len++] = reg_addr>>8; //线圈寄存器起始地址
    buf[len++] = reg_addr;
    buf[len++] = reg_num>>8;  //线圈寄存器的个数
    buf[len++] = reg_num;
    
    bytes = reg_num/8;  //写入的字节数
    if(reg_num%8 !=0)
        bytes++;
    
    buf[len++] = bytes;   
    
    for(uint8_t j = 0; j < bytes; j++)
    {
        for(uint8_t i = 0; i < 8; i++) //按字节将线圈寄存器的状态值进行封装
        {
            index = i + j * 8;
            data >>=0x01;
            
            if(index < reg_num)  //判断写入的数据个数是否大于要写的寄存器个数
            {
                if(status[index] != 0)
                {
                    data |= 0x80;
                }
            }
            
        }
        buf[len++] = data;
    }
    
    uint16_t crc = Get_CRC16(buf,len);   //crc校验结果
    buf[len++] = crc;
    buf[len++] = crc>>8;
    
    RS485_SendBuff(RS485_USART,buf,len);
}


/**********************************************************************************
 * @brief : 读保持寄存器
 *                
 * @param : slave_id 从机地址
 * @param : reg_addr 保持寄存器起始地址
 * @param : reg_num 保持寄存器的个数
 * @retval  :
**********************************************************************************/

void Modbus_ReadHoldingReg(uint8_t slave_id,uint16_t reg_addr,uint16_t reg_num)
{
    uint8_t buf[8] = {0};
    buf[0] = slave_id; //从机地址
    buf[1] = 0x03;     //功能码
    buf[2] = reg_addr>>8; //保持寄存器起始地址
    buf[3] = reg_addr;
    buf[4] = reg_num>>8;  //保持寄存器的个数
    buf[5] = reg_num;
    
    uint16_t crc = Get_CRC16(buf,6);   //crc校验结果
    buf[6] = crc;
    buf[7] = crc>>8;
    
    RS485_SendBuff(RS485_USART,buf,8);
}


/**********************************************************************************
 * @brief : 写多个保持寄存器
 *                
 * @param : slave_id 从机地址
 * @param : reg_addr 保持寄存器起始地址
 * @param : reg_num 写入的保持寄存器个数
 * @param : data 保持寄存器值
 * @retval  :
**********************************************************************************/
void Modbus_WriteHoldingReg(uint8_t slave_id,uint16_t reg_addr,uint16_t reg_num,uint16_t *data)
{
    uint8_t buf[32] = {0};
    uint8_t len = 0,nbyte = 0;
    
    buf[len++] = slave_id; //从机地址
    buf[len++] = 0x10;     //功能码
    buf[len++] = reg_addr>>8; //保持寄存器起始地址
    buf[len++] = reg_addr;
    buf[len++] = reg_num>>8;  //保持寄存器的个数
    buf[len++] = reg_num;
    
    nbyte = reg_num * 2;
    
    buf[len++] = nbyte; //写入的字节数
    
    for(uint8_t i =0;i<reg_num;i++)
    {
        buf[len++] = data[i]>>8;
        buf[len++] = data[i];
    }
    
    uint16_t crc = Get_CRC16(buf,len);   //crc校验结果
    buf[len++] = crc;
    buf[len++] = crc>>8;
    
    RS485_SendBuff(RS485_USART,buf,len);
}

