/**
  ******************************************************************************
  * @file    bsp_basic_tim.c
  * @author  STMicroelectronics
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   通用定时器定时范例
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火  STM32 F103 开发板  
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "./tim/bsp_general_tim.h"
#include "./usart/bsp_debug_usart.h"
#include <math.h>
#include <stdlib.h>

void TIM_SetTIMxCompare(TIM_TypeDef *TIMx,uint32_t channel,uint32_t compare);
void TIM_SetPWM_period(TIM_TypeDef *TIMx,uint32_t TIM_period);

/**
  * @brief  配置TIM复用输出PWM时用到的I/O
  * @param  无
  * @retval 无
  */
static void TIMx_GPIO_Config(void) 
{
   GPIO_InitTypeDef GPIO_InitStruct;
   
  /* 定时器通道功能引脚端口时钟使能 */
	
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	TIM2_AF_ENABLE();
  
  /* 定时器通道1功能引脚IO初始化 */
	/*设置输出类型*/
   GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  /*设置引脚速率 */ 
   GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	/*选择要控制的GPIO引脚*/	
   GPIO_InitStruct.Pin = GENERAL_TIM4_CH1_PIN|GENERAL_TIM4_CH2_PIN|GENERAL_TIM4_CH3_PIN|GENERAL_TIM4_CH4_PIN;
  
	/*调用库函数，使用上面配置的GPIO_InitStructure初始化GPIO*/
   HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = GENERAL_TIM2_CH2_PIN;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = GENERAL_TIM2_CH1_PIN;	
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}


/*
 * 注意：TIM_TimeBaseInitTypeDef结构体里面有5个成员，TIM6和TIM7的寄存器里面只有
 * TIM_Prescaler和TIM_Period，所以使用TIM6和TIM7的时候只需初始化这两个成员即可，
 * 另外三个成员是通用定时器和高级定时器才有.
 *-----------------------------------------------------------------------------
 * TIM_Prescaler         都有
 * TIM_CounterMode			 TIMx,x[6,7]没有，其他都有（基本定时器）
 * TIM_Period            都有
 * TIM_ClockDivision     TIMx,x[6,7]没有，其他都有(基本定时器)
 * TIM_RepetitionCounter TIMx,x[1,8]才有(高级定时器)
 *-----------------------------------------------------------------------------
 */
TIM_HandleTypeDef  TIM_TimeBaseStructure4;
TIM_HandleTypeDef  TIM_TimeBaseStructure2;
static void TIM4_PWMOUTPUT_Config(void)                     //TIM4
{
  TIM_OC_InitTypeDef  TIM4_OCInitStructure; 
  TIM_MasterConfigTypeDef sMasterConfig;
  TIM_ClockConfigTypeDef sClockSourceConfig;  
	
  /*使能定时器*/
  GENERAL_TIM4_CLK_ENABLE();
	
  TIM_TimeBaseStructure4.Instance = GENERAL_TIM4;
  /* 累计 TIM_Period个后产生一个更新或者中断*/		
  //当定时器从0计数到PWM_PERIOD_COUNT，即为PWM_PERIOD_COUNT+1次，为一个定时周期
  TIM_TimeBaseStructure4.Init.Period = PWM_PERIOD_COUNT;
	// 通用控制定时器时钟源TIMxCLK = HCLK=72MHz 
	// 设定定时器频率为=TIMxCLK/(PWM_PRESCALER_COUNT+1)
  TIM_TimeBaseStructure4.Init.Prescaler = PWM_PRESCALER_COUNT;	
	
	/*计数方式*/
  TIM_TimeBaseStructure4.Init.CounterMode = TIM_COUNTERMODE_UP;
	/*采样时钟分频*/
  TIM_TimeBaseStructure4.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
	/*初始化定时器*/
  HAL_TIM_Base_Init(&TIM_TimeBaseStructure4);
  
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  HAL_TIM_ConfigClockSource(&TIM_TimeBaseStructure4, &sClockSourceConfig);
  
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&TIM_TimeBaseStructure4, &sMasterConfig);
  
	/*PWM模式配置*/
  TIM4_OCInitStructure.OCMode = TIM_OCMODE_PWM1;      // 配置为PWM模式1
  TIM4_OCInitStructure.Pulse = 0.5/20.0*PWM_PERIOD_COUNT;    // 默认占空比
  TIM4_OCInitStructure.OCFastMode = TIM_OCFAST_DISABLE;
	/*当定时器计数值小于CCR1_Val时为高电平*/
  TIM4_OCInitStructure.OCPolarity = TIM_OCPOLARITY_HIGH;	
	
	/*配置PWM通道*/
    HAL_TIM_PWM_ConfigChannel(&TIM_TimeBaseStructure4, &TIM4_OCInitStructure, PWM_CHANNEL1);
	HAL_TIM_PWM_ConfigChannel(&TIM_TimeBaseStructure4, &TIM4_OCInitStructure, PWM_CHANNEL2);
	HAL_TIM_PWM_ConfigChannel(&TIM_TimeBaseStructure4, &TIM4_OCInitStructure, PWM_CHANNEL3);
	HAL_TIM_PWM_ConfigChannel(&TIM_TimeBaseStructure4, &TIM4_OCInitStructure, PWM_CHANNEL4);
	/*开始输出PWM*/
	HAL_TIM_PWM_Start(&TIM_TimeBaseStructure4,PWM_CHANNEL1);
	HAL_TIM_PWM_Start(&TIM_TimeBaseStructure4,PWM_CHANNEL2);
	HAL_TIM_PWM_Start(&TIM_TimeBaseStructure4,PWM_CHANNEL3);
	HAL_TIM_PWM_Start(&TIM_TimeBaseStructure4,PWM_CHANNEL4);
}

static void TIM2_PWMOUTPUT_Config(void)                     //TIM2
{
  TIM_OC_InitTypeDef  TIM_OCInitStructure; 
  TIM_MasterConfigTypeDef sMasterConfig;
  TIM_ClockConfigTypeDef sClockSourceConfig;  
	
  /*使能定时器*/
  GENERAL_TIM2_CLK_ENABLE();
	
  TIM_TimeBaseStructure2.Instance = GENERAL_TIM2;
  /* 累计 TIM_Period个后产生一个更新或者中断*/		
  //当定时器从0计数到PWM_PERIOD_COUNT，即为PWM_PERIOD_COUNT+1次，为一个定时周期
  TIM_TimeBaseStructure2.Init.Period = PWM_PERIOD_COUNT;
	// 通用控制定时器时钟源TIMxCLK = HCLK=72MHz 
	// 设定定时器频率为=TIMxCLK/(PWM_PRESCALER_COUNT+1)
  TIM_TimeBaseStructure2.Init.Prescaler = PWM_PRESCALER_COUNT;	
	
	/*计数方式*/
  TIM_TimeBaseStructure2.Init.CounterMode = TIM_COUNTERMODE_UP;
	/*采样时钟分频*/
  TIM_TimeBaseStructure2.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
	/*初始化定时器*/
  HAL_TIM_Base_Init(&TIM_TimeBaseStructure2);
  
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  HAL_TIM_ConfigClockSource(&TIM_TimeBaseStructure2, &sClockSourceConfig);
  
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&TIM_TimeBaseStructure2, &sMasterConfig);
  
	/*PWM模式配置*/
  TIM_OCInitStructure.OCMode = TIM_OCMODE_PWM1;      // 配置为PWM模式1
  TIM_OCInitStructure.Pulse = 0.5/20.0*PWM_PERIOD_COUNT;    // 默认占空比
  TIM_OCInitStructure.OCFastMode = TIM_OCFAST_DISABLE;
	/*当定时器计数值小于CCR1_Val时为高电平*/
  TIM_OCInitStructure.OCPolarity = TIM_OCPOLARITY_HIGH;	
	
	/*配置PWM通道*/
    HAL_TIM_PWM_ConfigChannel(&TIM_TimeBaseStructure2, &TIM_OCInitStructure, PWM_CHANNEL1);
	HAL_TIM_PWM_ConfigChannel(&TIM_TimeBaseStructure2, &TIM_OCInitStructure, PWM_CHANNEL2);
	/*开始输出PWM*/
	HAL_TIM_PWM_Start(&TIM_TimeBaseStructure2,PWM_CHANNEL1);
	HAL_TIM_PWM_Start(&TIM_TimeBaseStructure2,PWM_CHANNEL2);
}

/**
  * @brief  设置TIM通道的占空比
	* @param  channel		通道	（1,2,3,4）
	* @param  compare		占空比
	*	@note 	无
  * @retval 无
  */
void TIM4_SetPWM_pulse(uint32_t channel,int compare)
{
		switch(channel)
	{
		case TIM_CHANNEL_1:   __HAL_TIM_SET_COMPARE(&TIM_TimeBaseStructure4,TIM_CHANNEL_1,compare);break;
		case TIM_CHANNEL_2:	  __HAL_TIM_SET_COMPARE(&TIM_TimeBaseStructure4,TIM_CHANNEL_2,compare);break;
		case TIM_CHANNEL_3:	  __HAL_TIM_SET_COMPARE(&TIM_TimeBaseStructure4,TIM_CHANNEL_3,compare);break;
		case TIM_CHANNEL_4:	  __HAL_TIM_SET_COMPARE(&TIM_TimeBaseStructure4,TIM_CHANNEL_4,compare);break;
	}
}

void TIM2_SetPWM_pulse(uint32_t channel,int compare)
{
		switch(channel)
	{
		case TIM_CHANNEL_1:   __HAL_TIM_SET_COMPARE(&TIM_TimeBaseStructure2,TIM_CHANNEL_1,compare);break;
		case TIM_CHANNEL_2:	  __HAL_TIM_SET_COMPARE(&TIM_TimeBaseStructure2,TIM_CHANNEL_2,compare);break;
		case TIM_CHANNEL_3:	  __HAL_TIM_SET_COMPARE(&TIM_TimeBaseStructure2,TIM_CHANNEL_3,compare);break;
		case TIM_CHANNEL_4:	  __HAL_TIM_SET_COMPARE(&TIM_TimeBaseStructure2,TIM_CHANNEL_4,compare);break;
	}
}


/**
  * @brief  初始化控制通用定时器
  * @param  无
  * @retval 无
  */
void TIMx_Configuration(void)
{
	TIMx_GPIO_Config();
  
    TIM4_PWMOUTPUT_Config();
	TIM2_PWMOUTPUT_Config();
}

/**
  * @brief  设置舵机占空比
  * @param  angle: 占空比，（0.5/20.0*PWM_PERIOD_COUNT 到 2.5/20.0*PWM_PERIOD_COUNT）
  * @retval 无
  */
void TIM4_set_steering_gear_dutyfactor(uint16_t dutyfactor,uint32_t channel)
{
  #if 1
  {
    /* 对超过范围的占空比进行边界处理 */
    dutyfactor = 0.5/20.0*PWM_PERIOD_COUNT > dutyfactor ? 0.5/20.0*PWM_PERIOD_COUNT : dutyfactor;
    dutyfactor = 2.5/20.0*PWM_PERIOD_COUNT < dutyfactor ? 2.5/20.0*PWM_PERIOD_COUNT : dutyfactor;
  }
  #endif
  
	TIM4_SetPWM_pulse(channel, dutyfactor);
}

void TIM2_set_steering_gear_dutyfactor(uint16_t dutyfactor,uint32_t channel)
{
  #if 1
  {
    /* 对超过范围的占空比进行边界处理 */
    dutyfactor = 0.5/20.0*PWM_PERIOD_COUNT > dutyfactor ? 0.5/20.0*PWM_PERIOD_COUNT : dutyfactor;
    dutyfactor = 2.5/20.0*PWM_PERIOD_COUNT < dutyfactor ? 2.5/20.0*PWM_PERIOD_COUNT : dutyfactor;
  }
  #endif
  
	TIM2_SetPWM_pulse(channel, dutyfactor);
}

/**
  * @brief  设置舵机角度
  * @param  angle: 角度，（0 到 180（舵机为0°-180°））
  * @retval 无
  */
void TIM4_set_steering_gear_angle(uint16_t angle_temp,uint32_t channel)
{
  angle_temp = (0.5 + angle_temp / 180.0 * (2.5 - 0.5)) / 20.0 * PWM_PERIOD_COUNT;    // 计算角度对应的占空比
  
  TIM4_set_steering_gear_dutyfactor(angle_temp,channel);    // 设置占空比
}

void TIM2_set_steering_gear_angle(uint16_t angle_temp,uint32_t channel)
{
  angle_temp = (0.5 + angle_temp / 180.0 * (2.5 - 0.5)) / 20.0 * PWM_PERIOD_COUNT;    // 计算角度对应的占空比
  
  TIM2_set_steering_gear_dutyfactor(angle_temp,channel);    // 设置占空比
}

void axle_3_move(uint16_t angle_temp ,int dir) //B6  TIM4  CHANNEL_1   轴3转动 0度为输出107度左右  加角度为顺时针 
{
	TIM4_set_steering_gear_angle(107 + (dir)*angle_temp , PWM_CHANNEL1);      //1为顺时针，-1为逆时针
}

void axle4_move(void)                     //B7  TIM4  CHANNEL_2  棉签转动
{
	int i = 0;
	for(i = 0;i<5;i++)
	{
		TIM4_set_steering_gear_angle(180 , PWM_CHANNEL2);
		HAL_Delay(100);
		TIM4_set_steering_gear_angle(10 , PWM_CHANNEL2);
		HAL_Delay(100);
	}	
}

void cut_move(uint16_t angle_temp)                   //CUT 零点为60度 剪断为30度 顺时针
{
	angle_temp = (0.5 + angle_temp / 270.0 * (2.5 - 0.5)) / 20.0 * PWM_PERIOD_COUNT;
	TIM4_set_steering_gear_dutyfactor(angle_temp,PWM_CHANNEL2);                              //B7
}

///**
//  * @brief  打印帮助命令
//  * @param  无
//  * @retval 无
//  */
//void show_help(void)
//{
//    printf("——————————————野火舵机驱动演示程序——————————————\n\r");
//    printf("输入命令(<>内的内容，以回车结束)：\n\r");
//    printf("<?>       -帮助菜单\n\r");
//    printf("<a+角度>  -设置舵机的角度（范围：%d—%d），使用空格代替+号\n\r", 0, 180);
//}

//e['xtern uint16_t ChannelPulse;

///**
//  * @brief  处理串口接收到的数据
//  * @param  无
//  * @retval 无
//  */
//void deal_serial_data(void)
//{
//    int angle_temp=0;
//    
//    //接收到正确的指令才为1
//    char okCmd = 0;

//    //检查是否接收到指令
//    if(receive_cmd == 1)
//    {
//      if(UART1_RxBuffer[0] == 'a' || UART1_RxBuffer[0] == 'A')
//      {
//        //设置速度
//        if(UART1_RxBuffer[1] == ' ')
//        {
//          angle_temp = atoi((char const *)UART1_RxBuffer+2);
//          if(angle_temp>=0 && angle_temp <= 180)
//          {
//            printf("\n\r角度: %d\n\r", angle_temp);
//            ChannelPulse = (0.5 + angle_temp / 180.0 * (2.5 - 0.5)) / 20.0 * PWM_PERIOD_COUNT;    // 更新按钮控制的占空比
//            set_steering_gear_angle(angle_temp);
////            printf("\n\r角度: %d\n\r", (uint16_t)(angle_temp/PWM_PERIOD_COUNT*20.0/(2.5-0.5)*180.0));
//            okCmd = 1;
//          }
//        }
//      }
//      else if(UART1_RxBuffer[0] == '?')
//      {
//        //打印帮助命令
//        show_help();
//        okCmd = 1;
//      }
//      //如果指令有无则打印帮助命令
//      if(okCmd != 1)
//      {
//        printf("\n\r 输入有误，请重新输入...\n\r");
//        show_help();
//      }

//      //清空串口接收缓冲数组
//      receive_cmd = 0;
//      uart_FlushRxBuffer();

//    }
//}


/*********************************************END OF FILE**********************/
