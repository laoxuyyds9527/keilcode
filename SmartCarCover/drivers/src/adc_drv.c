#include "adc_drv.h"

/**
 * @brief ADC1CH16初始化
 * @param 无
 * @retval 无
 */
void ADC1_CH16_Init(void)
{
	ADC_InitTypeDef ADC_InitStruct;

	//开启ADC时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	//ADC时钟分频
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);

	//ADC配置
	ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;  //连续转换模式
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;  //数据对齐方式
	ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;  //外部触发方式
	ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;  //独立工作模式
	ADC_InitStruct.ADC_NbrOfChannel = 1;  //转换的通道数
	ADC_InitStruct.ADC_ScanConvMode = DISABLE;  //扫描模式
	ADC_Init(ADC1, &ADC_InitStruct);

	//设置ADC的规则通道/采样顺序/采样时间
	ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 1, ADC_SampleTime_239Cycles5);

	//开启ADC
	ADC_Cmd(ADC1, ENABLE);
	
	//复位ADC校准寄存器
	ADC_ResetCalibration(ADC1);

	//检查校准寄存器是否复位完成
	while(ADC_GetResetCalibrationStatus(ADC1) == SET);
	
	//校准ADC
	ADC_StartCalibration(ADC1);

	//检查ADC校准是否完成
	while(ADC_GetCalibrationStatus(ADC1) == SET)

	//开启内部温度传感器和内部参考电压通道(仅支持ADC1)
	ADC_TempSensorVrefintCmd(ENABLE);

	//软件开启ADC转换
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}


/**
 * @brief ADC1CH2初始化
 * @param 无
 * @retval 无
 */
void ADC1_CH2_Init(void)
{
	ADC_InitTypeDef ADC_InitStruct;
	GPIO_InitTypeDef GPIO_InitStruct;

	//开启外设时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);

	//ADC时钟分频
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	
	//GPIO初始化
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	//ADC配置
	ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;  //连续转换模式
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;  //数据对齐方式
	ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;  //外部触发方式
	ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;  //独立工作模式
	ADC_InitStruct.ADC_NbrOfChannel = 2;  //转换的通道数
	ADC_InitStruct.ADC_ScanConvMode = ENABLE;  //扫描模式
	ADC_Init(ADC1, &ADC_InitStruct);

	//设置ADC的规则通道/采样顺序/采样时间
	ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 1, ADC_SampleTime_239Cycles5);

	
	
	//开启ADC
	ADC_Cmd(ADC1, ENABLE);
	
	//复位ADC校准寄存器
	ADC_ResetCalibration(ADC1);

	//检查校准寄存器是否复位完成
	while(ADC_GetResetCalibrationStatus(ADC1) == SET);
	
	//校准ADC
	ADC_StartCalibration(ADC1);

	//检查ADC校准是否完成
	while(ADC_GetCalibrationStatus(ADC1) == SET)

	
	//开启DMA请求
	ADC_DMACmd(ADC1,ENABLE);
	
	//软件开启ADC转换
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}


/**
 * @brief ADC1CH3初始化
 * @param 无
 * @retval 无
 */
void ADC1_CH3_Init(void)
{
	ADC_InitTypeDef ADC_InitStruct;
	GPIO_InitTypeDef GPIO_InitStruct;

	//开启外设时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);

	//ADC时钟分频
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	
	//GPIO初始化
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOC, &GPIO_InitStruct);

	//ADC配置
	ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;  //连续转换模式
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;  //数据对齐方式
	ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;  //外部触发方式
	ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;  //独立工作模式
	ADC_InitStruct.ADC_NbrOfChannel = 2;  //转换的通道数
	ADC_InitStruct.ADC_ScanConvMode = ENABLE;  //扫描模式
	ADC_Init(ADC1, &ADC_InitStruct);
	
	
	//设置ADC的规则通道/采样顺序/采样时间
	ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 2, ADC_SampleTime_239Cycles5);

	//开启ADC
	ADC_Cmd(ADC1, ENABLE);
	
	//复位ADC校准寄存器
	ADC_ResetCalibration(ADC1);

	//检查校准寄存器是否复位完成
	while(ADC_GetResetCalibrationStatus(ADC1) == SET);
	
	//校准ADC
	ADC_StartCalibration(ADC1);

	//检查ADC校准是否完成
	while(ADC_GetCalibrationStatus(ADC1) == SET)

	//开启DMA请求
	ADC_DMACmd(ADC1,ENABLE);
	
	//软件开启ADC转换
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}










/**
 * @brief 获取内部温度值
 * @param 无
 * @retval 温度值
 */
float Get_TempVal(void)
{
	float V_25 = 1.43;
	float Avg_Slope = 4.3;
	float V_sense = 0;
	float temp = 0;

	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) != SET);  //等待转换完成

	V_sense = ADC_GetConversionValue(ADC1) * 3.3 / 4095;  //计算真实电压值

	ADC_ClearFlag(ADC1, ADC_FLAG_EOC);  //手动清除EOC标志位

	temp = ((V_25 - V_sense) * 1000 / Avg_Slope) + 25;

	return temp;
}


/**
 * @brief 获取振动的平均值
 * @param 无
 * @retval 平均值
 */
u16 Get_ConvVal(void)
{
	u16 data;
	
	for(int i = 0;i < 10;i++)
	{
		while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) != SET);  //等待转换完成

		data += Conv_Val[0];
		ADC_ClearFlag(ADC1, ADC_FLAG_EOC);  //手动清除EOC标志位
	}

	return (data / 10);
}


u16 Get_Power(void)
{

	u16 power;
	for(int i = 0;i < 10;i++)
	{
		while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) != SET);  //等待转换完成

		
		power += Conv_Val[1];
		

		ADC_ClearFlag(ADC1, ADC_FLAG_EOC);  //手动清除EOC标志位
	}

	return (power/ 10);
}


