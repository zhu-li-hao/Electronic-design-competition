/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2020-xx-xx
  * @brief   舵机驱动示例代码
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 F103 STM32 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f1xx.h"
#include "bsp_debug_usart.h"
#include <stdlib.h>
#include "bsp_key.h" 
#include "math_method.h" 
#include "bsp_general_tim.h"
#include "bsp_stepper_init.h"
#include <stdio.h>


__IO uint16_t ChannelPulse = 0.5/20.0*PWM_PERIOD_COUNT;
float theta[4] ;
float *p;
float *result;
int i = 1,cut = 0;

//extern float distance[] = {0};

float move_angle1 = 0;   //移动角度先赋值   第一轴
float move_angle2 = 0;
float move_angle3 = 0;

float CURRENT_angle[4] = {0,90.000015,90.000015,0};   //当前角度   第一轴
float NEXT_angle[4] = {0,0,0,0};   //当前角度   第一轴

float test01[4] = {0,0,0,0};   //当前角度   第一轴
float test02[4] = {0,90,90,0};   //当前角度   第一轴


/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void) 
{
	uint8_t len;	
	uint16_t times=0;
  //	int i = 0;
	
	/* 初始化系统时钟为72MHz */
	HAL_Init();
	SystemClock_Config();
	__HAL_RCC_SYSCFG_CLK_ENABLE();
	
	/* 初始化按键GPIO */
	Key_GPIO_Config();
  
  /* 初始化串口 */
  uart1_init(115200);					//初始化串口
	uart2_init(115200);					//初始化串口
	uart3_init(115200);					//初始化串口
  
  /* 初始化基本定时器定时，1s产生一次中断 */
	TIMx_Configuration();
	
	//tof_communication();
	
	while(1)
	{  
		raspberry_communication();
		//tof_communication();
		
	}
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 72000000
  *            HCLK(Hz)                       = 72000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 2
  *            APB2 Prescaler                 = 1
  *            HSE Frequency(Hz)              = 8000000
  *            HSE PREDIV1                    = 1
  *            PLLMUL                         = 9
  *            Flash Latency(WS)              = 2
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef clkinitstruct = {0};
  RCC_OscInitTypeDef oscinitstruct = {0};
  
  /* Enable HSE Oscillator and activate PLL with HSE as source */
  oscinitstruct.OscillatorType  = RCC_OSCILLATORTYPE_HSE;
  oscinitstruct.HSEState        = RCC_HSE_ON;
  oscinitstruct.HSEPredivValue  = RCC_HSE_PREDIV_DIV1;
  oscinitstruct.PLL.PLLState    = RCC_PLL_ON;
  oscinitstruct.PLL.PLLSource   = RCC_PLLSOURCE_HSE;
  oscinitstruct.PLL.PLLMUL      = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&oscinitstruct)!= HAL_OK)
  {
    /* Initialization Error */
    while(1); 
  }

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  clkinitstruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  clkinitstruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  clkinitstruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  clkinitstruct.APB2CLKDivider = RCC_HCLK_DIV1;
  clkinitstruct.APB1CLKDivider = RCC_HCLK_DIV2;  
  if (HAL_RCC_ClockConfig(&clkinitstruct, FLASH_LATENCY_2)!= HAL_OK)
  {
    /* Initialization Error */
    while(1); 
  }
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
