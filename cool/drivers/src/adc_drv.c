#include "adc_drv.h"

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
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	//ADC配置
	ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;  //连续转换模式
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;  //数据对齐方式
	ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;  //外部触发方式
	ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;  //独立工作模式
	ADC_InitStruct.ADC_NbrOfChannel = 1;  //转换的通道数
	ADC_InitStruct.ADC_ScanConvMode = DISABLE;  //扫描模式
	ADC_Init(ADC1, &ADC_InitStruct);

	//设置ADC的规则通道/采样顺序/采样时间
	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 1, ADC_SampleTime_239Cycles5);

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

	//软件开启ADC转换
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}
//火焰强度
/**
 * @brief 获取平均值
 * @param 无
 * @retval 平均值
 */
u16 Get_ConvVal(void)
{
	u16 data;

	for(int i = 0;i < 10;i++)
	{
		while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) != SET);  //等待转换完成

		data += ADC_GetConversionValue(ADC1);

		ADC_ClearFlag(ADC1, ADC_FLAG_EOC);  //手动清除EOC标志位
	}

	return (data / 10);
}



