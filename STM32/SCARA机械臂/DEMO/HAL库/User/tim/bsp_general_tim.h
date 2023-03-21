#ifndef __BSP_GENERAL_TIM_H
#define	__BSP_GENERAL_TIM_H

#include "stm32f1xx.h"

/*宏定义*/
#define GENERAL_TIM4                        	TIM4
#define GENERAL_TIM2                        	TIM2
#define GENERAL_TIM4_CLK_ENABLE()  					__HAL_RCC_TIM4_CLK_ENABLE()
#define GENERAL_TIM2_CLK_ENABLE()  					__HAL_RCC_TIM2_CLK_ENABLE()

#define PWM_CHANNEL1                       TIM_CHANNEL_1
#define PWM_CHANNEL2                       TIM_CHANNEL_2
#define PWM_CHANNEL3                       TIM_CHANNEL_3
#define PWM_CHANNEL4                       TIM_CHANNEL_4

/* 累计 TIM_Period个后产生一个更新或者中断*/		
/* 当定时器从0计数到PWM_PERIOD_COUNT，即为PWM_PERIOD_COUNT+1次，为一个定时周期 */
#define PWM_PERIOD_COUNT     999

/* 通用控制定时器时钟源TIMxCLK = HCLK=72MHz */
/* 设定定时器频率为=TIMxCLK/(PWM_PRESCALER_COUNT+1) */
#define PWM_PRESCALER_COUNT     1440

/*PWM引脚*/
#define GENERAL_TIM4_CH1_GPIO_PORT           GPIOB                  //TIM4
#define GENERAL_TIM4_CH1_PIN                 GPIO_PIN_6

#define GENERAL_TIM4_CH2_GPIO_PORT           GPIOB
#define GENERAL_TIM4_CH2_PIN                 GPIO_PIN_7

#define GENERAL_TIM4_CH3_GPIO_PORT           GPIOB
#define GENERAL_TIM4_CH3_PIN                 GPIO_PIN_8

#define GENERAL_TIM4_CH4_GPIO_PORT           GPIOB
#define GENERAL_TIM4_CH4_PIN                 GPIO_PIN_9

#define GENERAL_TIM2_CH1_GPIO_PORT           GPIOA                  //TIM2
#define GENERAL_TIM2_CH1_PIN                 GPIO_PIN_15

#define GENERAL_TIM2_CH2_GPIO_PORT           GPIOB
#define GENERAL_TIM2_CH2_PIN                 GPIO_PIN_3

//#define GENERAL_TIM5_CH3_GPIO_PORT           GPIOB
//#define GENERAL_TIM5_CH3_PIN                 GPIO_PIN_2

//#define GENERAL_TIM5_CH4_GPIO_PORT           GPIOB
//#define GENERAL_TIM5_CH4_PIN                 GPIO_PIN_3
#define TIM2_AF_ENABLE()							__HAL_AFIO_REMAP_TIM2_PARTIAL_1();\
																			__HAL_AFIO_REMAP_SWJ_NOJTAG()
																			
#define axle3_channel  PWM_CHANNEL1
#define axle3_channel  PWM_CHANNEL1
#define axle3_channel  PWM_CHANNEL1



extern TIM_HandleTypeDef  TIM_TimeBaseStructure4;
extern TIM_HandleTypeDef  TIM_TimeBaseStructure2;

extern void TIMx_Configuration(void);
extern void TIM4_SetPWM_pulse(uint32_t channel,int compare);
extern void TIM2_SetPWM_pulse(uint32_t channel,int compare);
void TIM4_set_steering_gear_dutyfactor(uint16_t dutyfactor,uint32_t channel);
void TIM2_set_steering_gear_dutyfactor(uint16_t dutyfactor,uint32_t channel);
void TIM4_set_steering_gear_angle(uint16_t angle_temp,uint32_t channel);
void TIM2_set_steering_gear_angle(uint16_t angle_temp,uint32_t channel);
void cut_move(uint16_t angle_temp);   
void axle4_move(void);
void axle_3_move(uint16_t angle_temp ,int dir);
void show_help(void);
void deal_serial_data(void);

#endif

