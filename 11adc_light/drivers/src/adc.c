#include "adc.h"


void adc_init(void)
{

	ADC_InitTypeDef  ADC_InitStruct;
	
	//开启时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	//醉倒时钟频率是14m，先分频
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	//配置初始化adc
	ADC_InitStruct.ADC_ContinuousConvMode=ENABLE;//设置为连续的模式
	ADC_InitStruct.ADC_DataAlign=ADC_DataAlign_Right;//右对齐
	ADC_InitStruct.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;//
	ADC_InitStruct.ADC_Mode=ADC_Mode_Independent;//独立工作模式
	ADC_InitStruct.ADC_NbrOfChannel=1;//转换的通道数
	ADC_InitStruct.ADC_ScanConvMode=ENABLE;//扫描模式
	ADC_Init(ADC1, &ADC_InitStruct);

	//设置指定 ADC 的规则组通道，设置它们的转化顺序和采样时间
	ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 1,ADC_SampleTime_239Cycles5);
	//复位校准寄存器
	ADC_ResetCalibration(ADC1);
	//是否复位成功
	while(ADC_GetCalibrationStatus(ADC1)==SET);
	//开始校准
	ADC_StartCalibration(ADC1);
	//是否校准成功
	while(ADC_GetCalibrationStatus(ADC1)==SET);

	//使能温度传感器和内部参考电压通道的新状态
	ADC_TempSensorVrefintCmd(ENABLE);

	//开启软件adc开始转换
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);


}



//@breif获取内部温度
//@param没有
//@retval浮点的温度


float Get_temp(void)
{
	float v25=1.43;
	float avg_slope=4.3;
	float V_sense=0;
	float temp=0;
	while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC)!=SET)
	V_sense= ADC_GetConversionValue(ADC1)*3.3 / 4095;
	temp=((v25-V_sense)*1000/avg_slope)+25;
	return temp;

}



