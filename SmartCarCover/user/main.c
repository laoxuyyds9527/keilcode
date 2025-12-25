#include "stm32f10x.h"
#include "systick.h"
#include "usart_drv.h"
#include "system.h"
#include "adc_drv.h"
#include "feng.h"
#include "dma_drv.h"
#include "motor.h"
#include "infrared.h"
#include "lcd_driver.h"
#include "gui.h"





// LCD显示刷新函数（适配你的全局结构体，显示所有数据）
void LCD_UpdateAllData(void) {
    uint8_t buf[17];  // 临时缓冲区（1602屏每行16字符，这里用于格式化）
    
    // 清屏（白色背景，避免残影）
//    Lcd_Clear(WHITE);
	
	 // 1. 工作模式显示
    Gui_DrawFont_GBK16(16, 0,  RED, WHITE, (uint8_t*)"Mode:");
	
	Gui_DrawFont_GBK16(64, 0, BLUE, WHITE, (uint8_t*)"open");
//    if (g_sys_state.work_mode == MODE_MANUAL) {
//        Gui_DrawFont_GBK16(64, 0, BLUE, WHITE, (uint8_t*)"Manual");
//    } else {
//        Gui_DrawFont_GBK16(64, 0, BLUE, WHITE, (uint8_t*)"Auto");
//    }
//    // 2. 风扇档位显示
//    Gui_DrawFont_GBK16(0, 16, BLUE, WHITE, (uint8_t*)"Fan");
//    switch (g_sys_state.fan_gear) {
//        case FAN_OFF:  Gui_DrawFont_GBK16(48, 16, BLUE, WHITE, (uint8_t*)"Off"); break;
//        case FAN_LOW:  Gui_DrawFont_GBK16(48, 16, BLUE, WHITE, (uint8_t*)"Low"); break;
//        case FAN_MID:  Gui_DrawFont_GBK16(48, 16, BLUE, WHITE, (uint8_t*)"Mid"); break;
//        case FAN_HIGH: Gui_DrawFont_GBK16(48, 16, BLUE, WHITE, (uint8_t*)"High"); break;
//    }

//    // ==================== 上中部区域（第2行） ====================
    // 区域范围：x=0~127, y=32~47
    // 内容：温度 + 湿度
//    Gui_DrawFont_GBK16(0, 32, BLUE, WHITE, (uint8_t*)"Temp:");
//    sprintf((char*)buf, "%.1f", g_sys_state.dht11_data.temperature);
//    Gui_DrawFont_GBK16(45, 32, BLUE, WHITE, (uint8_t*)buf);

//    Gui_DrawFont_GBK16(80, 32, RED, WHITE, (uint8_t*)"Hum:");
//    sprintf((char*)buf, "%.1f", g_sys_state.dht11_data.humidity);
//    Gui_DrawFont_GBK16(112, 32, BLUE, WHITE, (uint8_t*)buf);

//    // ==================== 中部区域（第3行） ====================
//    // 区域范围：x=0~127, y=48~63
//    // 内容：PM2.5浓度
//    Gui_DrawFont_GBK16(0, 48, BLUE, WHITE, (uint8_t*)"PM2.5:");
//    if (g_sys_state.pm25_data.data_valid) {
//        sprintf((char*)buf, "%03d", g_sys_state.pm25_data.pm25_concentration);
//        Gui_DrawFont_GBK16(64, 48, GREEN, WHITE, (uint8_t*)buf);
//        Gui_DrawFont_GBK16(96, 48, BLACK, WHITE, (uint8_t*)"ug");
//    } else {
//        Gui_DrawFont_GBK16(64, 48, RED, WHITE, (uint8_t*)"--");
//    }

//    // ==================== 底部区域（第4行） ====================
//    // 区域范围：x=0~127, y=64~79
//    // 内容：定时状态 + 剩余时间
//    Gui_DrawFont_GBK16(0, 64, RED, WHITE, (uint8_t*)"Timer:");
//    if (g_sys_state.Timer_State == TIMER_ON) {
//        sprintf((char*)buf, "%02d:%02d", g_sys_state.timer_remaining / 60, g_sys_state.timer_remaining % 60);
//        Gui_DrawFont_GBK16(64, 64, GREEN, WHITE, (uint8_t*)buf);
//    } else {
//        Gui_DrawFont_GBK16(64, 64, BLACK, WHITE, (uint8_t*)"Off");
//    }

//    // ==================== 最底部区域（第5行） ====================
//    // 区域范围：x=0~127, y=80~95
//    // 内容：操作提示信息
//    if (g_sys_state.work_mode == MODE_MANUAL) {
//        Gui_DrawFont_GBK16(0, 80, BLACK, WHITE, (uint8_t*)"KEY1:Gear");
//		Gui_DrawFont_GBK16(0, 95, BLACK, WHITE, (uint8_t*)"KEY2:Mode");
//    } else {
//        Gui_DrawFont_GBK16(0, 80, GREEN, WHITE, (uint8_t*)"Auto Mode:");
//		Gui_DrawFont_GBK16(0, 95, GREEN, WHITE, (uint8_t*)"PM2.5 Ctrl");
//    }
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
	//初始化振动传感器
	ADC1_CH2_Init();
	//初始化电源检测
	ADC1_CH3_Init();
	//初始化蜂鸣器
	feng_ming();
	//dma
	DMA1_CH1_Init();
	//步进电机
	Motor();
	//红外初始化
	HX1838_Init ();
	//lcd初始化
	Lcd_Init();
	LCD_LED_SET;//通过IO控制背光亮	
	Lcd_Clear(WHITE);

//	Gui_DrawFont_GBK16(16, 0,  RED, WHITE, (uint8_t*)"Mode:");
}


int main (void){
	
	HardWare_Init();
	u16 vbira=0;

	u16 flag=0;
	baojin(flag);
	u16 shijian=0;
	u32 HwKeyValue = 0;
	float power=0;
	while(1)
	{
		printf("振动强度 = %d\n",Get_ConvVal());
		vbira=Get_ConvVal();
		if(vbira==0)
		{
		
			printf("有人动车子了\n");
		
		}
		
		power=Get_Power();
		
		printf("电压强度 = %f\n",power);
		delay_ms(1000);
//		turn(1);

// 获取红外键值，如果接收到有效按键则自动处理

		
		

//		printf("电\n");
//		LCD_UpdateAllData();
//		delay_ms(100);

	
	}		
		
	
	
}
