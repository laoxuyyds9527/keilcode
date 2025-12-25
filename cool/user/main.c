#include "stm32f10x.h"
#include "systick.h"
#include "usart_drv.h"
#include "system.h"
#include "iic_drv.h"
#include "iic2.h"
#include "sht30.h"
#include "SGP30.h"
#include "lcd_driver.h"
#include "gui.h"
#include "adc_drv.h"
#include "feng.h"
#include "cool.h"
#include "key_drv.h"
#include "pwn.h"
#include "esp8266.h"	


char *ServerIP = "mqtts.heclouds.com";           //存放服务器IP或是域名，根据服务器实际IP进行更改
int  ServerPort = 1883;                    //存放服务器的端口号区
char  Data_buff[2048];     //数据缓冲区


u16 Flame_threshold=3000;
u8 fire_off=0;
u16	co2_value=0;
u16 Humi = 0;	//温度和湿度
int Temp = 0;


// LCD显示刷新函数（适配你的全局结构体，显示所有数据
void LCD_UpdateAllData(u16 temp, uint16_t humi, uint16_t co2,u16 power)
	{
    uint8_t buf[16];  // 临时缓冲区（1602屏每行16字符，这里用于格式化）
    
    // 清屏（白色背景，避免残影）
    Lcd_Clear(WHITE);
	
	 // 1. 工作模式显示
    Gui_DrawFont_GBK16(16, 0,  RED, WHITE, (uint8_t*)"Mode:");
	
	Gui_DrawFont_GBK16(64, 0, BLUE, WHITE, (uint8_t*)"open");


	 // ==================== 第一行：工作模式显示 ====================
    Gui_DrawFont_GBK16(16, 0,  RED, WHITE, (uint8_t*)"Mode:");
    Gui_DrawFont_GBK16(64, 0, BLUE, WHITE, (uint8_t*)"open");

    // ==================== 第二行：温度显示 ====================
    // 显示"Temp:"（英文），如果需要中文可改为"温度："（需确保字库支持）
    Gui_DrawFont_GBK16(0, 16, BLACK, WHITE, (uint8_t*)"Temp:");
    // 格式化温度，添加℃单位（如果是小数，可改为sprintf(buf, "%.1f℃", (float)temp/10);）
    sprintf((char*)buf, "%d℃", temp);
    Gui_DrawFont_GBK16(60, 16, BLUE, WHITE, buf);

    // ==================== 第三行：湿度显示 ====================
    Gui_DrawFont_GBK16(0, 32, BLACK, WHITE, (uint8_t*)"Humi:");
    // 格式化湿度，添加%单位
    sprintf((char*)buf, "%d%%", humi);
    Gui_DrawFont_GBK16(60, 32, GREEN, WHITE, buf);

    // ==================== 第四行：二氧化碳浓度显示 ====================
    Gui_DrawFont_GBK16(0, 48, BLACK, WHITE, (uint8_t*)"CO2:");
    // 格式化二氧化碳，添加ppm单位
    sprintf((char*)buf, "%dppm", co2);
    Gui_DrawFont_GBK16(60, 48, RED, WHITE, buf);
	//==================== 第五行：火焰强度显示 ====================
    Gui_DrawFont_GBK16(0, 64, BLACK, WHITE, (uint8_t*)"fire:");
    // 格式化二氧化碳，添加ppm单位
    sprintf((char*)buf, "%dKW", power);
    Gui_DrawFont_GBK16(60, 64, RED, WHITE, buf);
	//	LCD_LED_CLR;//IO控制背光灭
	
}













void HardWare_Init(void)
{
	//优先级分组
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	//systick
	Systick_Init(72);
	//USART1初始化
	USART1_Init(115200);
	//IIC初始化
	IIC_Config();
	//输入模式
	IIC_Input();
	//IIC初始化
	IIC2_Config();
	//输入模式
	IIC2_Input();
	//二氧化碳
	SGP30_Init();
	//屏幕显示
	Lcd_Init();
	LCD_LED_SET;//通过IO控制背光亮	
	Lcd_Clear(WHITE);
	//火焰强度
	ADC1_CH2_Init();
	//蜂鸣器
	feng_ming();
	//制冷检测
	cool_init();
	//按键
	Key_Init();
	//初始化报警呼吸灯
	pwn_init(100-1,360-1);//2000hz的频率
	//uart4
	UART4_MyConfig(115200);;
	WiFi_ResetIO_Init();
	wifi_init();

}




int main (void){
	
	HardWare_Init();
	int time;
	u8 Temp_threshold=30;
	u16 power=0;
	u8 flag=0;
	u32 data=0;
	u8 cn=0;
	u8 uptime=2;
	while(1)
	{
		
		
	uptime++;
	if(uptime>2)	
	{
		uptime=0;
		//读取传感器
		SHT30_Read_Humiture(&Temp,&Humi);
		Sgp30_Get_Co2_Value(&co2_value);
		power=Get_ConvVal();
		printf("温度=%d\n",Temp);
		printf("湿度=%d\n",Humi);
		printf("二氧化碳=%d\n",co2_value);
		printf("火焰强度=%d\n",power);
		//检测温度是否过高
		if(Temp>25)
		{
			coolflag(1);
		}
		else
		{
			coolflag(0);
		}
		//检测是否着火
		if(power<Flame_threshold)
		{
			while(1)
			{
			//蜂鸣器报警
//			baojin(1);
			//呼吸警报灯
				
			if(flag)
			TIM_SetCompare1(TIM1,data--);//重新设置捕获的比较值
			else
			TIM_SetCompare1(TIM1,data++);
			if(data==99||data==0)
				flag=!flag;
			delay_ms(3);
			printf("捕捉=%d\n",data);
			cn++;
			if(cn>30)
			{
				SHT30_Read_Humiture(&Temp,&Humi);
				Sgp30_Get_Co2_Value(&co2_value);
				power=Get_ConvVal();
				printf("温度=%d\n",Temp);
				printf("湿度=%d\n",Humi);
				printf("二氧化碳=%d\n",co2_value);
				printf("火焰强度=%d\n",power);
				LCD_UpdateAllData(Temp,Humi,co2_value,power);	
				cn=0;
			}
			if(fire_off==1)
			{
				fire_off=0;
				break;
			}	
			}		
		}
		
	}
	printf("是否开始下发");
	delay_ms(1000);
	WiFi_ack();
	WiFi_ConnectServer();//上报数据
	delay_ms(1);
	//按键调节阈值
	printf("阈值=%d\n",Flame_threshold);
	//LCD显示
	LCD_UpdateAllData(Temp,Humi,co2_value,power);
	}
}
