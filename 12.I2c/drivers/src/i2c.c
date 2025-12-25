#include "i2c.h"



#define RCC_SCL RCC_APB2Periph_GPIOB//时钟线的引脚
#define SCL_PIN GPIO_Pin_6//scl的引脚
#define SCL_SPEED GPIO_Speed_50MHz//速率
#define SCL_DUANKOU GPIOB
#define SCL_up	GPIO_SetBits(GPIOB,GPIO_Pin_6)
#define SCL_down GPIO_ResetBits (GPIOB,GPIO_Pin_6)




#define RCC_SDA RCC_APB2Periph_GPIOB//数据线的引脚？是不是随笔的引脚可以

#define SDA_PIN GPIO_Pin_7//SDA的引脚
#define SDA_SPEED GPIO_Speed_50MHz//速率
#define SDA_DUANKOU GPIOB
#define SDA_up	GPIO_SetBits(GPIOB,GPIO_Pin_7)
#define SDA_down GPIO_ResetBits (GPIOB,GPIO_Pin_7)


#define delay delay_us (1)

//@brief初始化i2c
//@param 没有
//@retval 没有
void i2c_init(void)
{

	GPIO_InitTypeDef  GPIO_InitStruct;
	//开启时钟
	RCC_APB2PeriphClockCmd(RCC_SCL|RCC_SDA, ENABLE);

	//gpioB6和7的配置scl和sda
	GPIO_InitStruct.GPIO_Mode= GPIO_Mode_Out_OD;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_6;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(SCL_DUANKOU, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Mode= GPIO_Mode_Out_OD;
	GPIO_InitStruct.GPIO_Pin=SDA_PIN;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(SDA_DUANKOU , &GPIO_InitStruct);

}

//起始信号，结束信号，应答信号，飞应答信号
//等待应答信号，发送数据，读取数据，输出模式，输入模式。

//@brief i2c的sda输出模式
//@param 没有
//@retval 没有
void i2c_out(void)
{
	
	GPIO_InitTypeDef  GPIO_InitStruct;

	//开启时钟
	RCC_APB2PeriphClockCmd(RCC_SDA, ENABLE);
	//配置引脚
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_OD;
	GPIO_InitStruct.GPIO_Pin=SDA_PIN;
	GPIO_InitStruct.GPIO_Speed=SDA_SPEED;
	GPIO_Init(SDA_DUANKOU , &GPIO_InitStruct);



}




//@brief i2c的sda输入模式
//@param 没有
//@retval 没有
void i2c_input(void)
{
	
	GPIO_InitTypeDef  GPIO_InitStruct;

	//开启时钟
	RCC_APB2PeriphClockCmd(RCC_SDA, ENABLE);
	//配置引脚
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin=SDA_PIN;
	GPIO_InitStruct.GPIO_Speed=SDA_SPEED;
	GPIO_Init(SDA_DUANKOU, &GPIO_InitStruct);



}




//@brief i2c起始信号，检测到sda的下降沿的变化
//@param 没有
//@retval 没有
void i2c_start(void)
{
	i2c_out();
	//scl拉低
	SCL_down;
	delay;
	//sda接高电平
	SDA_up;
	delay;
	//scl接高电平
	SCL_up;
	delay;
	//sda拉低电平
	SDA_down;
	delay;
	//scl拉低电平
	SCL_down;

}




//@brief i2c停止信号，检测到sda的下降沿的变化
//@param 没有
//@retval 没有
void i2c_stop(void)
{
	i2c_out();
	//scl拉低
	SCL_down;
	delay;
	//sda接d低电平
	SDA_down;
	delay;
	//scl接高电平
	SCL_up;
	delay;
	//sda拉电平
	SDA_up;
	delay;
	//scl拉低电平
	SCL_down;
	delay;

}






//@brief i2c应答信号，检测到sda的下降沿的变化
//@param 没有
//@retval 没有

