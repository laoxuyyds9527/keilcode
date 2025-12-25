#include "gxht3x.h"
#define POLYNOMIAL_CXDZ 0x31 // X^8 + X^5 + X^4 + 1


unsigned char crc_gxht(uint8_t *data, uint8_t len)
{
	u8 	bit;        // bit mask
	u8  crc = 0xFF; // calculated checksum
	u8  byteCtr;    // byte counter
	
	// calculates 8-Bit checksum with given polynomial @GZCXDZ
	for(byteCtr = 0; byteCtr < len; byteCtr++) {
			crc ^= (data[byteCtr]);
			for(bit = 8; bit > 0; --bit) 
			{
				if(crc & 0x80) 
				{
					crc = (crc << 1) ^ POLYNOMIAL_CXDZ;
				}  
				else 
				{
					crc = (crc << 1);
				}
			}
	}
  return crc;

}


void SHT30_CMD(uint16_t cmd)
{
	IIC_Start();
  	IIC_WriteByte(GXHT3X_ADDR_W);  //发送设备地址,写寄存器
	if(IIC_WaitACK() == IIC_ACK)
	{
		IIC_WriteByte((cmd>>8)&0xff); //MSB
		if(IIC_WaitACK() == IIC_ACK)
		{
			IIC_WriteByte(cmd&0xff); //LSB
			if(IIC_WaitACK() == IIC_ACK)
			{
				IIC_Stop();
				delay_ms(30);//命令发完后需要等待20ms以上才能读写
			}
		}
		
	}	
}



//SHT30读取温湿度
//temp:温度，-400~1250，实际温度=temp/10,分辨率0.1℃,精度±0.3℃
//humi:湿度，0~1000，实际湿度=humi/10,分辨率0.1%rh,精度±3
//返回0成功，1失败

u8 SHT30_Read_Humiture(int *temp,uint16_t *humi)
{
	uint8_t buff[6];//定义一个数组
	
	SHT30_CMD(SHT30_READ_HUMITURE);//读温湿度命令
	
	IIC_Start();
	IIC_WriteByte(GXHT3X_ADDR_R); //发送设备地址,读寄存器
	if(IIC_WaitACK() == IIC_ACK)
	{
		buff[0]=IIC_ReadByte();//继续读，给应答
		IIC_SendACK();
		buff[1]=IIC_ReadByte();//继续读，给应答
		IIC_SendACK();
		buff[2]=IIC_ReadByte();//继续读，给应答
		IIC_SendACK();
		buff[3]=IIC_ReadByte();//继续读，给应答
		IIC_SendACK();
		buff[4]=IIC_ReadByte();//继续读，给应答
		IIC_SendACK();
		buff[5]=IIC_ReadByte();//不继续给停止应答
		IIC_SendNoACK();
		IIC_Stop();
	}


//	printf("buff=%d,%d,%d,%d,%d,%d\r\n",buff[0],buff[1],buff[2],buff[3],buff[4],buff[5]);
	//CRC校验
	if(crc_gxht(&buff[0],2)==buff[2] && crc_gxht(&buff[3],2)==buff[5])
	{

		*temp=(-45+(175.0*((buff[0]<<8)+buff[1])/65535.0));
		*humi=100*(((buff[3]<<8)+buff[4])/65535.0);
		if(*temp>12500) 
			*temp=12500;
		else if
			(*temp<-4000) *temp=-4000;
		return 0;
	}
	else return 1;	
	
}











