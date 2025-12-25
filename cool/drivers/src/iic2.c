#include "iic2.h"




/**
 * @brief IIC初始化配置
 * @param 无
 * @retval 无
 */
void IIC2_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	//开启外设时钟
	RCC_APB2PeriphClockCmd(SCL2_RCC | SDA2_RCC, ENABLE);

	//初始化GPIO引脚
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStruct.GPIO_Pin = SCL2_PIN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SCL2_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStruct.GPIO_Pin = SDA2_PIN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SDA2_PORT, &GPIO_InitStruct);
}


/**
 * @brief IIC输出模式
 * @param 无
 * @retval 无
 */
void IIC2_Output(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	//开启外设时钟
	RCC_APB2PeriphClockCmd(SDA2_RCC, ENABLE);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStruct.GPIO_Pin = SDA2_PIN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SDA2_PORT, &GPIO_InitStruct);
}


/**
 * @brief IIC输入模式
 * @param 无
 * @retval 无
 */
void IIC2_Input(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	//开启外设时钟
	RCC_APB2PeriphClockCmd(SDA2_RCC, ENABLE);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin = SDA2_PIN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SDA2_PORT, &GPIO_InitStruct);
}


/**
 * @brief IIC起始信号
 * @param 无
 * @retval 无
 */
void IIC2_Start(void)
{
	
	IIC2_Output();
	SCL2_DOWN;
	DELAY;
	SDA2_UP;
	DELAY;
	SCL2_UP;
	DELAY;
	SDA2_DOWN;
	DELAY;
	SCL2_DOWN;
	DELAY;
}


/**
 * @brief IIC停止信号
 * @param 无
 * @retval 无
 */
void IIC2_Stop(void)
{
	IIC2_Output();
	SCL2_DOWN;
	DELAY;
	SDA2_DOWN;
	DELAY;
	SCL2_UP;
	DELAY;
	SDA2_UP;
	DELAY;
	SCL2_DOWN;
	DELAY;
}


/**
 * @brief IIC发送应答信号
 * @param 无
 * @retval 无
 */
void IIC2_SendACK(void)
{
	IIC2_Output();
	SCL2_DOWN;
	DELAY;
	SDA2_DOWN;
	DELAY;
	SCL2_UP;
	DELAY;
	SCL2_DOWN;
	DELAY;
}

/**
 * @brief IIC发送非应答信号
 * @param 无
 * @retval 无
 */
void IIC2_SendNoACK(void)
{
	IIC2_Output();
	SCL2_DOWN;
	DELAY;
	SDA2_UP;
	DELAY;
	SCL2_UP;
	DELAY;
	SCL2_DOWN;
	DELAY;
}


/**
 * @brief IIC等待应答信号
 * @param 无
 * @retval 应答或者非应答
 */
u8 IIC2_WaitACK(void)
{
	u8 cnt = 0;
	IIC2_Input();
	SCL2_DOWN;
	DELAY;
	SCL2_UP;
	DELAY;
	while(SDA2_READ)
	{
		if(++cnt > 30)
		{
			IIC2_Output();
			IIC2_Stop();
			return IIC2_NOACK;
		}
		DELAY;
	}
	SCL2_DOWN;
	DELAY;
	return IIC2_ACK;
}



/**
 * @brief IIC发送字节
 * @param 需要发送的字节
 * @retval 无
 */
void IIC2_WriteByte(u8 data)
{
	IIC2_Output();
	for(int i = 0;i < 8;i++)
	{
		SCL2_DOWN;
		DELAY;
		(data & 0x80) ? (SDA2_UP) : (SDA2_DOWN);
		DELAY;
		SCL2_UP;
		DELAY;
		SCL2_DOWN;
		DELAY;
		data <<= 1;
	}
}


/**
 * @brief IIC读取字节
 * @param 无
 * @retval 接收到的字节
 */
u8 IIC2_ReadByte(void)
{
	u8 data = 0;
	IIC2_Input();
	for(int i = 0;i < 8;i++)
	{
		data <<= 1;
		SCL2_DOWN;
		DELAY;
		SCL2_UP;
		DELAY;
		if(SDA2_READ)
			data |= 1;
	}
	return data;
}

