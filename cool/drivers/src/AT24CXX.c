#include "AT24Cxx.h"


/**
 * @brief AT24CXX写字节
 * @param addr数据的写入地址
 * @param data写入的数据
 * @retval 无
 */
void AT24CXX_WriteByte(u8 addr,u8 data)
{
	IIC_Start();
	IIC_WriteByte(AT24C02_ADDR_W);
	if(IIC_WaitACK() == IIC_ACK)
	{
		IIC_WriteByte(addr);
		if(IIC_WaitACK() == IIC_ACK)
		{
			IIC_WriteByte(data);
			if(IIC_WaitACK() == IIC_ACK)
			{
				IIC_Stop();
			}
			else
				printf("Write ACK3 error\n");
		}
		else
			printf("Write ACK2 error\n");
	}
	else
		printf("Write ACK1 error\n");
}


/**
 * @brief AT24CXX写字节
 * @param addr数据的写入地址
 * @param data写入的数据
 * @retval 无
 */
u8 AT24CXX_ReadByte(u8 addr)
{
	u8 data = 0xFF;
	IIC_Start();
	IIC_WriteByte(AT24C02_ADDR_W);
	if(IIC_WaitACK() == IIC_ACK)
	{
		IIC_WriteByte(addr);
		if(IIC_WaitACK() == IIC_ACK)
		{
			IIC_Start();
			IIC_WriteByte(AT24C02_ADDR_R);
			if(IIC_WaitACK() == IIC_ACK)
			{
				data = IIC_ReadByte();
				IIC_SendNoACK();
				IIC_Stop();
			}
			else
				printf("Read ACK3 error\n");
		}
		else
			printf("Read ACK2 error\n");
	}
	else
		printf("Read ACK1 error\n");
	return data;
}


