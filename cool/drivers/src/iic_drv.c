#include "iic_drv.h"




/**
 * @brief IIC初始化配置
 * @param 无
 * @retval 无
 */
void IIC_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	//开启外设时钟
	RCC_APB2PeriphClockCmd(SCL_RCC | SDA_RCC, ENABLE);

	//初始化GPIO引脚
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStruct.GPIO_Pin = SCL_PIN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SCL_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStruct.GPIO_Pin = SDA_PIN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SDA_PORT, &GPIO_InitStruct);
}


/**
 * @brief IIC输出模式
 * @param 无
 * @retval 无
 */
void IIC_Output(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	//开启外设时钟
	RCC_APB2PeriphClockCmd(SDA_RCC, ENABLE);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStruct.GPIO_Pin = SDA_PIN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SDA_PORT, &GPIO_InitStruct);
}


/**
 * @brief IIC输入模式
 * @param 无
 * @retval 无
 */
void IIC_Input(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	//开启外设时钟
	RCC_APB2PeriphClockCmd(SDA_RCC, ENABLE);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin = SDA_PIN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SDA_PORT, &GPIO_InitStruct);
}


/**
 * @brief IIC起始信号
 * @param 无
 * @retval 无
 */
void IIC_Start(void)
{
	
	IIC_Output();
	SCL_DOWN;
	DELAY;
	SDA_UP;
	DELAY;
	SCL_UP;
	DELAY;
	SDA_DOWN;
	DELAY;
	SCL_DOWN;
	DELAY;
}


/**
 * @brief IIC停止信号
 * @param 无
 * @retval 无
 */
void IIC_Stop(void)
{
	IIC_Output();
	SCL_DOWN;
	DELAY;
	SDA_DOWN;
	DELAY;
	SCL_UP;
	DELAY;
	SDA_UP;
	DELAY;
	SCL_DOWN;
	DELAY;
}


/**
 * @brief IIC发送应答信号
 * @param 无
 * @retval 无
 */
void IIC_SendACK(void)
{
	IIC_Output();
	SCL_DOWN;
	DELAY;
	SDA_DOWN;
	DELAY;
	SCL_UP;
	DELAY;
	SCL_DOWN;
	DELAY;
}

/**
 * @brief IIC发送非应答信号
 * @param 无
 * @retval 无
 */
void IIC_SendNoACK(void)
{
	IIC_Output();
	SCL_DOWN;
	DELAY;
	SDA_UP;
	DELAY;
	SCL_UP;
	DELAY;
	SCL_DOWN;
	DELAY;
}


/**
 * @brief IIC等待应答信号
 * @param 无
 * @retval 应答或者非应答
 */
u8 IIC_WaitACK(void)
{
	u8 cnt = 0;
	IIC_Input();
	SCL_DOWN;
	DELAY;
	SCL_UP;
	DELAY;
	while(SDA_READ)
	{
		if(++cnt > 30)
		{
			IIC_Output();
			IIC_Stop();
			return IIC_NOACK;
		}
		DELAY;
	}
	SCL_DOWN;
	DELAY;
	return IIC_ACK;
}



/**
 * @brief IIC发送字节
 * @param 需要发送的字节
 * @retval 无
 */
void IIC_WriteByte(u8 data)
{
	IIC_Output();
	for(int i = 0;i < 8;i++)
	{
		SCL_DOWN;
		DELAY;
		(data & 0x80) ? (SDA_UP) : (SDA_DOWN);
		DELAY;
		SCL_UP;
		DELAY;
		SCL_DOWN;
		DELAY;
		data <<= 1;
	}
}


/**
 * @brief IIC读取字节
 * @param 无
 * @retval 接收到的字节
 */
u8 IIC_ReadByte(void)
{
	u8 data = 0;
	IIC_Input();
	for(int i = 0;i < 8;i++)
	{
		data <<= 1;
		SCL_DOWN;
		DELAY;
		SCL_UP;
		DELAY;
		if(SDA_READ)
			data |= 1;
	}
	return data;
}

