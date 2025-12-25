#include "motor.h"



//@brief 电机驱动
//@param
//@retval

void Motor(void)
{

	GPIO_InitTypeDef  GPIO_InitStruct;
	//开启时钟
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOB, ENABLE);
	//配置gpio引脚
	GPIO_InitStruct.GPIO_Pin=motor_a|motor_b|motor_c|motor_d;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init (motor_port,&GPIO_InitStruct);


	GPIO_ResetBits (motor_port, motor_a|motor_b|motor_c|motor_d);

}


//节奏是4拍
void bites_4(u16 step,u32 delay_xx)
{

	switch (step)
		{
		case 0:
			;
			break;
		case 1:
			down_a;
			up_b;
			up_c;
			down_d;
		printf("1111\n");
			break;
		case 2:
			down_a;
			down_b;
			up_c;
			up_d;
		printf("2222\n");
			break;
		case 3:
			up_a;
			down_b;
		    down_c;
			up_d;
		printf("3333\n");
		case 4:
			up_a;
			up_b;
			down_c;
			down_d;
		printf("4444\n");
		}

	delay_ms (delay_xx);
}



//节奏是8拍
void bites_8(u16 step)
{

	switch (step)
		{
		case 0:
//			printf("0000\n");
			break;
		case 1:
			down_a;
			up_b;
			up_c;
			up_d;
//		printf("1111\n");
			break;
		case 2:
			down_a;
			down_b;
			up_c;
			up_d;
//		printf("2222\n");
			break;
		case 3:
			up_a;
			down_b;
		    up_c;
			up_d;
//		printf("3333\n");
		break;
		case 4:
			up_a;
			down_b;
		    down_c;
			up_d;
//		printf("4444\n");
		break;
		case 5:
			up_a;
			up_b;
		    down_c;
			up_d;
//		printf("5555\n");
		break;
		case 6:
			up_a;
			up_b;
		    down_c;
			down_d;
//		printf("6666\n");
		break;
		case 7:
			up_a;
			up_b;
		    up_c;
			down_d;
//		printf("7777\n");
		break;
		case 8:
			down_a;
			up_b;
		    up_c;
			down_d;
//		printf("88888\n");
		break;
		default:
		break;
		}
	

}








////顺时针转动
void turn(u16 quan)
{
//	for(u16 i=0;i<quan;i++)
	{
		for(u16 j=1;j<9;j++)
			{
				bites_8(j);	
				delay_ms (2);
			}
	}
}

