#include "SGP30.h"








void SGP30_Write(u8 a, u8 b)
{
  IIC2_Start();
  IIC2_WriteByte(SGP30_write); //发送器件地址+写指令
  if(IIC2_WaitACK()==IIC2_ACK)
  	{
  	IIC2_WriteByte(a);//发送控制字节
	if(IIC2_WaitACK()==IIC2_ACK)
  	{
  	IIC2_WriteByte(b);//发送控制字节
  	if(IIC2_WaitACK()==IIC2_ACK)
  	{  	
		IIC2_Stop();
//	 	delay_ms(100);	
  	}
  	}	
	
  	}
  	 
}

//初始化IIC接口
void SGP30_Init(void)
{
  SGP30_Write(0x20, 0x03);
	
}




u16 SGP30_Read(void)
{
	u8 buff[4]={0};
//	u16 co2=0;
  u16 dat;
  u8 crc;
  u8 crc2;
  IIC2_Start();
  IIC2_WriteByte(SGP30_read); //发送器件地址+读指令
  if(IIC2_WaitACK()==IIC2_ACK)
  	{
		buff[0]=IIC2_ReadByte();
		IIC2_SendACK();

		buff[1]=IIC2_ReadByte();
		IIC2_SendACK();

		crc = IIC2_ReadByte(); //crc数据，舍去
		crc = crc;  //为了不让出现编译警告
		IIC2_SendACK();
		buff[2]=IIC2_ReadByte();
		IIC2_SendACK();
		buff[3]=IIC2_ReadByte();
		IIC2_SendACK();
		crc2 = IIC2_ReadByte(); //crc数据，舍去
		crc2 = crc2;  //为了不让出现编译警告
		IIC2_SendNoACK();
		IIC2_Stop();
		dat=(buff[0]<<8)+buff[1];
  	}

  return dat;
}
/****
*******SGP30获取CO2和TVOC值函数
*****/
void Sgp30_Get_Co2_Value(u16 *co2_value)
{

  u32 sgp30_data = 0;
  SGP30_Write(0x20,0x08);
	delay_ms(15);
  sgp30_data = SGP30_Read();
  *co2_value = sgp30_data;

}



