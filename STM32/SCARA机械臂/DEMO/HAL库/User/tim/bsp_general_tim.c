/**
  ******************************************************************************
  * @file    bsp_basic_tim.c
  * @author  STMicroelectronics
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   ͨ�ö�ʱ����ʱ����
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��  STM32 F103 ������  
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :http://firestm32.taobao.com
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
  * @brief  ����TIM�������PWMʱ�õ���I/O
  * @param  ��
  * @retval ��
  */
static void TIMx_GPIO_Config(void) 
{
   GPIO_InitTypeDef GPIO_InitStruct;
   
  /* ��ʱ��ͨ���������Ŷ˿�ʱ��ʹ�� */
	
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	TIM2_AF_ENABLE();
  
  /* ��ʱ��ͨ��1��������IO��ʼ�� */
	/*�����������*/
   GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  /*������������ */ 
   GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	/*ѡ��Ҫ���Ƶ�GPIO����*/	
   GPIO_InitStruct.Pin = GENERAL_TIM4_CH1_PIN|GENERAL_TIM4_CH2_PIN|GENERAL_TIM4_CH3_PIN|GENERAL_TIM4_CH4_PIN;
  
	/*���ÿ⺯����ʹ���������õ�GPIO_InitStructure��ʼ��GPIO*/
   HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = GENERAL_TIM2_CH2_PIN;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = GENERAL_TIM2_CH1_PIN;	
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}


/*
 * ע�⣺TIM_TimeBaseInitTypeDef�ṹ��������5����Ա��TIM6��TIM7�ļĴ�������ֻ��
 * TIM_Prescaler��TIM_Period������ʹ��TIM6��TIM7��ʱ��ֻ���ʼ����������Ա���ɣ�
 * ����������Ա��ͨ�ö�ʱ���͸߼���ʱ������.
 *-----------------------------------------------------------------------------
 * TIM_Prescaler         ����
 * TIM_CounterMode			 TIMx,x[6,7]û�У��������У�������ʱ����
 * TIM_Period            ����
 * TIM_ClockDivision     TIMx,x[6,7]û�У���������(������ʱ��)
 * TIM_RepetitionCounter TIMx,x[1,8]����(�߼���ʱ��)
 *-----------------------------------------------------------------------------
 */
TIM_HandleTypeDef  TIM_TimeBaseStructure4;
TIM_HandleTypeDef  TIM_TimeBaseStructure2;
static void TIM4_PWMOUTPUT_Config(void)                     //TIM4
{
  TIM_OC_InitTypeDef  TIM4_OCInitStructure; 
  TIM_MasterConfigTypeDef sMasterConfig;
  TIM_ClockConfigTypeDef sClockSourceConfig;  
	
  /*ʹ�ܶ�ʱ��*/
  GENERAL_TIM4_CLK_ENABLE();
	
  TIM_TimeBaseStructure4.Instance = GENERAL_TIM4;
  /* �ۼ� TIM_Period�������һ�����»����ж�*/		
  //����ʱ����0������PWM_PERIOD_COUNT����ΪPWM_PERIOD_COUNT+1�Σ�Ϊһ����ʱ����
  TIM_TimeBaseStructure4.Init.Period = PWM_PERIOD_COUNT;
	// ͨ�ÿ��ƶ�ʱ��ʱ��ԴTIMxCLK = HCLK=72MHz 
	// �趨��ʱ��Ƶ��Ϊ=TIMxCLK/(PWM_PRESCALER_COUNT+1)
  TIM_TimeBaseStructure4.Init.Prescaler = PWM_PRESCALER_COUNT;	
	
	/*������ʽ*/
  TIM_TimeBaseStructure4.Init.CounterMode = TIM_COUNTERMODE_UP;
	/*����ʱ�ӷ�Ƶ*/
  TIM_TimeBaseStructure4.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
	/*��ʼ����ʱ��*/
  HAL_TIM_Base_Init(&TIM_TimeBaseStructure4);
  
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  HAL_TIM_ConfigClockSource(&TIM_TimeBaseStructure4, &sClockSourceConfig);
  
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&TIM_TimeBaseStructure4, &sMasterConfig);
  
	/*PWMģʽ����*/
  TIM4_OCInitStructure.OCMode = TIM_OCMODE_PWM1;      // ����ΪPWMģʽ1
  TIM4_OCInitStructure.Pulse = 0.5/20.0*PWM_PERIOD_COUNT;    // Ĭ��ռ�ձ�
  TIM4_OCInitStructure.OCFastMode = TIM_OCFAST_DISABLE;
	/*����ʱ������ֵС��CCR1_ValʱΪ�ߵ�ƽ*/
  TIM4_OCInitStructure.OCPolarity = TIM_OCPOLARITY_HIGH;	
	
	/*����PWMͨ��*/
    HAL_TIM_PWM_ConfigChannel(&TIM_TimeBaseStructure4, &TIM4_OCInitStructure, PWM_CHANNEL1);
	HAL_TIM_PWM_ConfigChannel(&TIM_TimeBaseStructure4, &TIM4_OCInitStructure, PWM_CHANNEL2);
	HAL_TIM_PWM_ConfigChannel(&TIM_TimeBaseStructure4, &TIM4_OCInitStructure, PWM_CHANNEL3);
	HAL_TIM_PWM_ConfigChannel(&TIM_TimeBaseStructure4, &TIM4_OCInitStructure, PWM_CHANNEL4);
	/*��ʼ���PWM*/
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
	
  /*ʹ�ܶ�ʱ��*/
  GENERAL_TIM2_CLK_ENABLE();
	
  TIM_TimeBaseStructure2.Instance = GENERAL_TIM2;
  /* �ۼ� TIM_Period�������һ�����»����ж�*/		
  //����ʱ����0������PWM_PERIOD_COUNT����ΪPWM_PERIOD_COUNT+1�Σ�Ϊһ����ʱ����
  TIM_TimeBaseStructure2.Init.Period = PWM_PERIOD_COUNT;
	// ͨ�ÿ��ƶ�ʱ��ʱ��ԴTIMxCLK = HCLK=72MHz 
	// �趨��ʱ��Ƶ��Ϊ=TIMxCLK/(PWM_PRESCALER_COUNT+1)
  TIM_TimeBaseStructure2.Init.Prescaler = PWM_PRESCALER_COUNT;	
	
	/*������ʽ*/
  TIM_TimeBaseStructure2.Init.CounterMode = TIM_COUNTERMODE_UP;
	/*����ʱ�ӷ�Ƶ*/
  TIM_TimeBaseStructure2.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
	/*��ʼ����ʱ��*/
  HAL_TIM_Base_Init(&TIM_TimeBaseStructure2);
  
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  HAL_TIM_ConfigClockSource(&TIM_TimeBaseStructure2, &sClockSourceConfig);
  
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&TIM_TimeBaseStructure2, &sMasterConfig);
  
	/*PWMģʽ����*/
  TIM_OCInitStructure.OCMode = TIM_OCMODE_PWM1;      // ����ΪPWMģʽ1
  TIM_OCInitStructure.Pulse = 0.5/20.0*PWM_PERIOD_COUNT;    // Ĭ��ռ�ձ�
  TIM_OCInitStructure.OCFastMode = TIM_OCFAST_DISABLE;
	/*����ʱ������ֵС��CCR1_ValʱΪ�ߵ�ƽ*/
  TIM_OCInitStructure.OCPolarity = TIM_OCPOLARITY_HIGH;	
	
	/*����PWMͨ��*/
    HAL_TIM_PWM_ConfigChannel(&TIM_TimeBaseStructure2, &TIM_OCInitStructure, PWM_CHANNEL1);
	HAL_TIM_PWM_ConfigChannel(&TIM_TimeBaseStructure2, &TIM_OCInitStructure, PWM_CHANNEL2);
	/*��ʼ���PWM*/
	HAL_TIM_PWM_Start(&TIM_TimeBaseStructure2,PWM_CHANNEL1);
	HAL_TIM_PWM_Start(&TIM_TimeBaseStructure2,PWM_CHANNEL2);
}

/**
  * @brief  ����TIMͨ����ռ�ձ�
	* @param  channel		ͨ��	��1,2,3,4��
	* @param  compare		ռ�ձ�
	*	@note 	��
  * @retval ��
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
  * @brief  ��ʼ������ͨ�ö�ʱ��
  * @param  ��
  * @retval ��
  */
void TIMx_Configuration(void)
{
	TIMx_GPIO_Config();
  
    TIM4_PWMOUTPUT_Config();
	TIM2_PWMOUTPUT_Config();
}

/**
  * @brief  ���ö��ռ�ձ�
  * @param  angle: ռ�ձȣ���0.5/20.0*PWM_PERIOD_COUNT �� 2.5/20.0*PWM_PERIOD_COUNT��
  * @retval ��
  */
void TIM4_set_steering_gear_dutyfactor(uint16_t dutyfactor,uint32_t channel)
{
  #if 1
  {
    /* �Գ�����Χ��ռ�ձȽ��б߽紦�� */
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
    /* �Գ�����Χ��ռ�ձȽ��б߽紦�� */
    dutyfactor = 0.5/20.0*PWM_PERIOD_COUNT > dutyfactor ? 0.5/20.0*PWM_PERIOD_COUNT : dutyfactor;
    dutyfactor = 2.5/20.0*PWM_PERIOD_COUNT < dutyfactor ? 2.5/20.0*PWM_PERIOD_COUNT : dutyfactor;
  }
  #endif
  
	TIM2_SetPWM_pulse(channel, dutyfactor);
}

/**
  * @brief  ���ö���Ƕ�
  * @param  angle: �Ƕȣ���0 �� 180�����Ϊ0��-180�㣩��
  * @retval ��
  */
void TIM4_set_steering_gear_angle(uint16_t angle_temp,uint32_t channel)
{
  angle_temp = (0.5 + angle_temp / 180.0 * (2.5 - 0.5)) / 20.0 * PWM_PERIOD_COUNT;    // ����Ƕȶ�Ӧ��ռ�ձ�
  
  TIM4_set_steering_gear_dutyfactor(angle_temp,channel);    // ����ռ�ձ�
}

void TIM2_set_steering_gear_angle(uint16_t angle_temp,uint32_t channel)
{
  angle_temp = (0.5 + angle_temp / 180.0 * (2.5 - 0.5)) / 20.0 * PWM_PERIOD_COUNT;    // ����Ƕȶ�Ӧ��ռ�ձ�
  
  TIM2_set_steering_gear_dutyfactor(angle_temp,channel);    // ����ռ�ձ�
}

void axle_3_move(uint16_t angle_temp ,int dir) //B6  TIM4  CHANNEL_1   ��3ת�� 0��Ϊ���107������  �ӽǶ�Ϊ˳ʱ�� 
{
	TIM4_set_steering_gear_angle(107 + (dir)*angle_temp , PWM_CHANNEL1);      //1Ϊ˳ʱ�룬-1Ϊ��ʱ��
}

void axle4_move(void)                     //B7  TIM4  CHANNEL_2  ��ǩת��
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

void cut_move(uint16_t angle_temp)                   //CUT ���Ϊ60�� ����Ϊ30�� ˳ʱ��
{
	angle_temp = (0.5 + angle_temp / 270.0 * (2.5 - 0.5)) / 20.0 * PWM_PERIOD_COUNT;
	TIM4_set_steering_gear_dutyfactor(angle_temp,PWM_CHANNEL2);                              //B7
}

///**
//  * @brief  ��ӡ��������
//  * @param  ��
//  * @retval ��
//  */
//void show_help(void)
//{
//    printf("����������������������������Ұ����������ʾ���򡪡�������������������������\n\r");
//    printf("��������(<>�ڵ����ݣ��Իس�����)��\n\r");
//    printf("<?>       -�����˵�\n\r");
//    printf("<a+�Ƕ�>  -���ö���ĽǶȣ���Χ��%d��%d����ʹ�ÿո����+��\n\r", 0, 180);
//}

//e['xtern uint16_t ChannelPulse;

///**
//  * @brief  �����ڽ��յ�������
//  * @param  ��
//  * @retval ��
//  */
//void deal_serial_data(void)
//{
//    int angle_temp=0;
//    
//    //���յ���ȷ��ָ���Ϊ1
//    char okCmd = 0;

//    //����Ƿ���յ�ָ��
//    if(receive_cmd == 1)
//    {
//      if(UART1_RxBuffer[0] == 'a' || UART1_RxBuffer[0] == 'A')
//      {
//        //�����ٶ�
//        if(UART1_RxBuffer[1] == ' ')
//        {
//          angle_temp = atoi((char const *)UART1_RxBuffer+2);
//          if(angle_temp>=0 && angle_temp <= 180)
//          {
//            printf("\n\r�Ƕ�: %d\n\r", angle_temp);
//            ChannelPulse = (0.5 + angle_temp / 180.0 * (2.5 - 0.5)) / 20.0 * PWM_PERIOD_COUNT;    // ���°�ť���Ƶ�ռ�ձ�
//            set_steering_gear_angle(angle_temp);
////            printf("\n\r�Ƕ�: %d\n\r", (uint16_t)(angle_temp/PWM_PERIOD_COUNT*20.0/(2.5-0.5)*180.0));
//            okCmd = 1;
//          }
//        }
//      }
//      else if(UART1_RxBuffer[0] == '?')
//      {
//        //��ӡ��������
//        show_help();
//        okCmd = 1;
//      }
//      //���ָ���������ӡ��������
//      if(okCmd != 1)
//      {
//        printf("\n\r ������������������...\n\r");
//        show_help();
//      }

//      //��մ��ڽ��ջ�������
//      receive_cmd = 0;
//      uart_FlushRxBuffer();

//    }
//}


/*********************************************END OF FILE**********************/
