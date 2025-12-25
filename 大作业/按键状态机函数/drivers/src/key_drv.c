#include "key_drv.h"

u8 key_val;
u8 key_shake_cnt;
u8 key_press_cnt;
u8	double_wait_cnt;
/**
 * @brief 按键初始化
 * @param 无
 * @retval 无
 */
void Key_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	EXTI_InitTypeDef EXTI_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;

	//配置时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);

	//将对应的GPIO口映射到指定的EXTI线路上
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource1);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource4);

	//配置引脚
	//PA0
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD;  //下拉输入
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	//PA1 PA4
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;  //上拉输入
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_4;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	//EXTI线路初始化
	EXTI_InitStruct.EXTI_Line = EXTI_Line0;  //中断线
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;  //中断线使能
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;  //中断模式
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;  //上升沿触发
	EXTI_Init(&EXTI_InitStruct);

	EXTI_InitStruct.EXTI_Line = EXTI_Line1;  //中断线
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;  //中断线使能
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;  //中断模式
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;  //下升沿触发
	EXTI_Init(&EXTI_InitStruct);

	EXTI_InitStruct.EXTI_Line = EXTI_Line4;  //中断线
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;  //中断线使能
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;  //中断模式
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;  //下升沿触发
	EXTI_Init(&EXTI_InitStruct);

	//NVIC初始化
	NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn;  //中断通道
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;  //使能
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStruct);

	NVIC_InitStruct.NVIC_IRQChannel = EXTI1_IRQn;  //中断通道
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;  //使能
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStruct);

	NVIC_InitStruct.NVIC_IRQChannel = EXTI4_IRQn;  //中断通道
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;  //使能
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStruct);
}


/**
 * @brief 按键扫描
 * @param mode:0单次检测模式，1连续检测模式
 * @retval 被按下的按键
 */
u8 Key_Scan(u8 mode)
{
	u8 key_val = KEY_NO;
	static u8 key_flag = 1;  //用来表示按键有没有被按下0表示被按下 1表示没有按下
	if(mode == 1)
		key_flag = 1;
	
	if((PAin(0) || !PAin(1) || !PAin(4)) && key_flag)
	{
		delay_ms(20);
		key_flag = 0;
		if(PAin(0))
		{
			key_val = KEY_1;
		}
		if(!PAin(1))
		{
			key_val = KEY_2;
		}
		if(!PAin(4))
		{
			key_val = KEY_3;
		}
	}
	else if(!PAin(0) && PAin(1) && PAin(4))
	{
		key_flag = 1;
	}
	return key_val;
}


/**
 * @brief 按键1的中断服务函数
 * @param 无
 * @retval 无
 */
void EXTI0_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line0) == SET)
	{
		EXTI_ClearITPendingBit(EXTI_Line0);

		//检测到电平变化之后,先清空CNT寄存器,再开启定时器
		TIM_SetCounter(TIM6, 0);
		TIM_Cmd(TIM6, ENABLE);
	}
}

/**
 * @brief 按键2的中断服务函数
 * @param 无
 * @retval 无
 */
void EXTI1_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line1) == SET)
	{
		EXTI_ClearITPendingBit(EXTI_Line1);

		//检测到电平变化之后,先清空CNT寄存器,再开启定时器
		TIM_SetCounter(TIM6, 0);
		TIM_Cmd(TIM6, ENABLE);
	}
}

/**
 * @brief 按键3的中断服务函数
 * @param 无
 * @retval 无
 */
void EXTI4_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line4) == SET)
	{
		EXTI_ClearITPendingBit(EXTI_Line4);

		//检测到电平变化之后,先清空CNT寄存器,再开启定时器
		TIM_SetCounter(TIM6, 0);
		TIM_Cmd(TIM6, ENABLE);
	}
}

u8 State_Machine(void)
{
	u8 key_type = KEY_TYPE_NO;//按键默认是没有按下
	static u8 key_state = KEY_STATE_IDLE;//默认是空闲状态
    static u8 temp = 0;//用来保存按压的时间
	static u8 key_state_prev = KEY_STATE_IDLE;//双击态
	switch(key_state){
		case KEY_STATE_IDLE: //空闲状态
			if(KEY_PRESS){
				key_state = KEY_STATE_SHAKE;//进入消抖状态
				TIM_SetCounter(TIM6, 0);//将cnt的值置0；
				key_shake_cnt = 0;
			}
			break;
		case KEY_STATE_SHAKE: //消抖状态
			if(key_shake_cnt >= 3 && KEY_PRESS)//消抖时间大于30ms,并且按键仍然是按下状态
				{
					key_state = KEY_STATE_PERSS;//切换为按压状态
					TIM_SetCounter(TIM6, 0);
					key_press_cnt = 0;
				}
			else if(key_shake_cnt >= 3 && KEY_RELEASE)//经过30ms消抖后按键是松开的，可能是误触导致的
				{
					key_state = KEY_STATE_IDLE;	//返回空闲状态
				}
			break;
		case KEY_STATE_PERSS:	
			if(KEY_RELEASE)//判断长按短按，主要是看在按键释放后的press_cnt的值
				{
					temp = key_press_cnt;	//保存按下的时间
					key_state = KEY_STATE_RELEASE;
				}
			break;
		case KEY_STATE_RELEASE:
			if(temp >= 100)//按压时间超过1s
				{
					key_type = KEY_TYPE_LONG;
					key_state = KEY_STATE_IDLE;
				}
				
			else
				{
					// 若当前是“等待双击期间的第二次按下”→ 直接判定为双击
					if(key_state_prev == KEY_STATE_WAIT_DOUBLE)  // 新增一个静态变量保存上一状态
					{
						key_type = KEY_TYPE_DOUBLE;
						key_state = KEY_STATE_IDLE;
						key_state_prev = KEY_STATE_IDLE;
					}
					else  // 第一次短按 → 进入等待双击态
					{
						key_state_prev = key_state;  // 保存当前状态为上一状态（静态变量）
						key_state = KEY_STATE_WAIT_DOUBLE;
						TIM_SetCounter(TIM6, 0);
						double_wait_cnt = 0;
					}
				}
				break;
		case KEY_STATE_WAIT_DOUBLE:
			if(double_wait_cnt >= 50)
            {
                key_type = KEY_TYPE_SHORT;
                key_state = KEY_STATE_IDLE;  // 返回空闲态
            }
            // 情况2：超时前检测到第二次按下 → 进入消抖态（验证第二次按下）
            else if(KEY_PRESS)
            {
                key_state = KEY_STATE_SHAKE;  // 重新消抖（第二次按下）
                TIM_SetCounter(TIM6, 0);
                key_shake_cnt = 0;
				key_state_prev = KEY_STATE_WAIT_DOUBLE;
				
            }			
			break;
	}
	return key_type;
}
