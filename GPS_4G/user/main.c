#include "n32wb452.h"
#include "stdio.h"
#include "led.h"
#include "usart.h"
#include "systick.h"
#include "gpio.h"
#include "uart.h"
#include "tim.h"
#include "fat_core.h"
#include "lte_demo.h"
#include "systick.h"
#include "usart_drv.h"
#include "stdio.h"
#include <stdlib.h>
#include "math.h"

void errorLog(int num);
void parseGpsBuffer(void);
void printGpsBuffer(void);


double lati=0;
double longi=0;


void headware_init(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	Systick_Init(144);
	led_init();	
	pen_gpio_init();
	UART5_init(115200);
	uart2_init();
	uart_init(9600);
	
	clrStruct();
	tim2_init(FAT_TIMER_VAL);
	reg_fat_uart_send_byte(uart2_send_byte);

}



void errorLog(int num)
{
	
	while (1)
	{
	  	printf("ERROR%d\r\n",num);
	}
}

void parseGpsBuffer()
{
	char *subString;
	char *subStringNext;
	char i = 0;
	if (Save_Data.isGetData)
	{
		Save_Data.isGetData = false;
		printf("**************\r\n");
		printf(Save_Data.GPS_Buffer);

		
		for (i = 0 ; i <= 6 ; i++)
		{
			if (i == 0)
			{
				if ((subString = strstr(Save_Data.GPS_Buffer, ",")) == NULL)
					errorLog(1);	//解析错误
			}
			else
			{
				subString++;
				if ((subStringNext = strstr(subString, ",")) != NULL)
				{
					char usefullBuffer[2]; 
					switch(i)
					{
						case 1:memcpy(Save_Data.UTCTime, subString, subStringNext - subString);break;	//获取UTC时间
						case 2:memcpy(usefullBuffer, subString, subStringNext - subString);break;	//获取UTC时间
						case 3:memcpy(Save_Data.latitude, subString, subStringNext - subString);break;	//获取纬度信息
						case 4:memcpy(Save_Data.N_S, subString, subStringNext - subString);break;	//获取N/S
						case 5:memcpy(Save_Data.longitude, subString, subStringNext - subString);break;	//获取经度信息
						case 6:memcpy(Save_Data.E_W, subString, subStringNext - subString);break;	//获取E/W

						default:break;
					}

					subString = subStringNext;
					Save_Data.isParseData = true;
					if(usefullBuffer[0] == 'A')
						Save_Data.isUsefull = true;
					else if(usefullBuffer[0] == 'V')
						Save_Data.isUsefull = false;

				}
				else
				{
					errorLog(2);	//解析错误
				}
			}


		}
	}

}

void printGpsBuffer()
{
	if (Save_Data.isParseData)
	{
		Save_Data.isParseData = false;
		
		printf("Save_Data.UTCTime = ");
		printf(Save_Data.UTCTime);
		printf("\r\n");

		if(Save_Data.isUsefull)
		{
			Save_Data.isUsefull = false;
			printf("Save_Data.latitude = ");
			printf(Save_Data.latitude);
			printf("\r\n");
			
			printf("Save_Data.N_S = ");
			printf(Save_Data.N_S);
			printf("\r\n");
			
			double a=atof(Save_Data.latitude);
			lati=((int)a)/100+(a-(((int)a)/100)*100)/60;
			
			
			printf("Save_Data.longitude = ");
			printf(Save_Data.longitude);
			printf("\r\n");

			printf("Save_Data.E_W = ");
			printf(Save_Data.E_W);
			printf("\r\n");
			
			double b=atof(Save_Data.longitude);
			longi=((int)b)/100+(b-(((int)b)/100)*100)/60;
		}
		else
		{
			printf("GPS DATA is not usefull!\r\n");
		}
		
	}
}





int main(void)
{  
	
	headware_init();
	
  	while (1)
    {	
    	parseGpsBuffer();
		printGpsBuffer();
		delay_ms(100);
        module_MQTT();

    
	}
}

