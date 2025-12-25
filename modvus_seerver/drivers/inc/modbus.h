#ifndef __MODBUS_H__
#define __MODBUS_H__

#include <stdint.h>
#include <stdio.h>

#define REG_COIL_ADDR_BASE     0x0000  //线圈寄存器起始地址
#define REG_HOLD_ADDR_BASE     0x0010  //保持寄存器起始地址

//线圈寄存器地址
typedef enum{
    REG0_COIL_ADDR = REG_COIL_ADDR_BASE,
    REG1_COIL_ADDR,
    REG2_COIL_ADDR,
    REG3_COIL_ADDR,
    
    REGX_COIL_ADDR

}Reg_Coil_Addr;


//保持寄存器地址
typedef enum{
    REG0_HOLD_ADDR = REG_HOLD_ADDR_BASE,
    REG1_HOLD_ADDR,

    REGX_HOLD_ADDR

}Reg_Hold_Addr;


void Modbus_ReadCoils(uint8_t slave_id,uint16_t reg_addr,uint16_t reg_num);
void Modbus_ReadHoldingReg(uint8_t slave_id,uint16_t reg_addr,uint16_t reg_num);
void Modbus_WriteHoldingReg(uint8_t slave_id,uint16_t reg_addr,uint16_t reg_num,uint16_t *data);
void Modbus_WriteCoils(uint8_t slave_id,uint16_t reg_addr,uint16_t reg_num,uint8_t *status);

#endif
