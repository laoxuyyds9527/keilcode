#include "stm32f10x.h"                  // Device header

#include "infrared.h"                 // 包含头文件以使用宏定义

u32 hw_jsm=0;	  // 定义一个32位数据变量，保存接收码
u8 hw_jsbz=0;  // 定义一个8位数据的变量，用于指示接收标志

/*
 *==============================================================================
 *函数名称：HX1838_Init
 *函数功能：初始化红外遥控模块
 *输入参数：无
 *返回值：无
 *备  注：红外端口初始化函数，时钟端口及外部中断初始化
 *==============================================================================
 */
void HX1838_Init (void)
{
	// 结构体定义
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	// 开启时钟
	RCC_APB2PeriphClockCmd(HX1838_GPIO_CLK | HX1838_AFIO_CLK, ENABLE);

	// 配置GPIO结构体
	GPIO_InitStructure.GPIO_Pin = HX1838_GPIO_PIN;   // 红外接收
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   // 上拉输入模式
	GPIO_Init(HX1838_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, HX1838_GPIO_PIN_SOURCE);   // 选择GPIO管脚用作外部中断线路
	EXTI_ClearITPendingBit(HX1838_EXTI_LINE);
	
	// 配置外部中断
	EXTI_InitStructure.EXTI_Line = HX1838_EXTI_LINE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure); 

	// 配置NVIC结构体
	NVIC_InitStructure.NVIC_IRQChannel = HX1838_EXTI_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = HX1838_IRQ_PREEMPT_PRIORITY;   // 抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = HX1838_IRQ_SUB_PRIORITY;   // 响应优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;   // 使能
	NVIC_Init(&NVIC_InitStructure);
}

/*
 *==============================================================================
 *函数名称：HX1838_RecordHightTime
 *函数功能：记录高电平持续时间并返回
 *输入参数：无
 *返回值：t：高电平持续时间
 *备  注：无
 *==============================================================================
 */
u8 HX1838_RecordHightTime(void)
{
	u8 t = 0;
	while(GPIO_ReadInputDataBit(HX1838_GPIO_PORT, HX1838_GPIO_PIN) == 1)   // 高电平
	{
		t++;
		delay_us(50);
		if(t >= 250)
		{
			return t;   // 超时溢出
		}
	}
	return t;
}

/*
 *==============================================================================
 *函数名称：Med_Hw_ReadKeyValue
 *函数功能：读取键值
 *输入参数：无
 *返回值：红外键值
 *备  注：每一个键值是测试出来的，不同遥控器键值可能不同
 *==============================================================================
 */
u8 HX1838_ReadKeyValue (void)
{
	u8 keyValue = HX1838_KEY_INVALID; // 默认返回无效值
	
	// 如果接收到有效按键，自动清除接收标志位和接收码
	if(hw_jsbz != 1)
	{
		return keyValue;
	}
	
	if(hw_jsm == 0x7FCC33)
	{
		keyValue = 0;
	}
	else if(hw_jsm == 0x7FD12E)
	{
		keyValue = 1;
	}
	else if(hw_jsm == 0x7FB14E)
	{
		keyValue = 2;
	}
	else if(hw_jsm == 0x7FF10E)
	{
		keyValue = 3;
	}
	else if(hw_jsm == 0x7F916E)
	{
		keyValue = 4;
	}
	else if(hw_jsm == 0x7F817E)
	{
		keyValue = 5;
	}
	else if(hw_jsm == 0x7FE11E)
	{
		keyValue = 6;
	}
	else if(hw_jsm == 0x7FF00F)
	{
		keyValue = 7;
	}
	else if(hw_jsm == 0x7FD42B)
	{
		keyValue = 8;
	}
	else if(hw_jsm == 0x7FC837)
	{
		keyValue = 9;
	}
	else if(hw_jsm == 0x7FB44B)   // 按键*按下
	{
		keyValue = 10;
	}
	else if(hw_jsm == 0x7FD827)   // 按键#按下
	{
		keyValue = 11;
	}
	else if(hw_jsm == 0x7F9C63)   // 按键OK按下
	{
		keyValue = 12;
	}
	else if(hw_jsm == 0x7F8C73)   // 按键"上"按下
	{
		keyValue = 13;
	}
	else if(hw_jsm == 0x7FA55A)   // 按键"下"按下
	{
		keyValue = 14;
	}
	else if(hw_jsm == 0x7F8877)   // 按键"左"按下
	{
		keyValue = 15;
	}
	else if(hw_jsm == 0x7FAD52)   // 按键"右"按下
	{
		keyValue = 16;
	}
	
	hw_jsbz = 0;   // 清除接收完成标志
	hw_jsm = 0;    // 清空接收码
	
	return keyValue;

}

/*
 *==============================================================================
 *函数名称：HX1838_IRQ_HANDLER
 *函数功能：外部中断服务函数
 *输入参数：无
 *返回值：无
 *备  注：无
 *==============================================================================
 */
void HX1838_IRQ_HANDLER(void)   // 红外遥控外部中断
{
	
	u8 Tim = 0,Ok = 0,Data=0,Num = 0;
	if(EXTI_GetITStatus(EXTI_Line2) == SET)
	{
		hw_jsm = 0;
//		printf("11111\n");
		while(1)
	   {
			if(GPIO_ReadInputDataBit(HX1838_GPIO_PORT, HX1838_GPIO_PIN) == 1)
			{
				 Tim = HX1838_RecordHightTime();   //获得此次高电平时间
//				printf("Tim=%d\n",Tim);
				 if(Tim>=250)
				 {
					 break;   // 不是有用的信号
				 }

				 if(Tim >= 85 && Tim < 95)
				 {
					Ok = 1;   // 收到起始信号
				 }
				 else if(Tim >= 25 && Tim < 40)
				 {
					Data = 1;   // 收到数据 1
				 }
				 else if(Tim >= 8 && Tim < 15)
				 {
					Data = 0;   // 收到数据 0
				 }
				 
				 if(Ok == 1)
				 {
					
					hw_jsm <<= 1;
					hw_jsm |= Data;
					Num ++;
//					 printf("hw_jsm=%d\n",hw_jsm);
					 // 接收完成
					if(Num >= 32)
					{
						printf("hw_jsm=%d\n",hw_jsm);
						printf("Num=%d\n",Num);
						hw_jsbz = 1;
						break;
					}
				 }
				 
			}
	   }
	} 
	 // 清除中断标志位
   EXTI_ClearITPendingBit(HX1838_EXTI_LINE);	
}

