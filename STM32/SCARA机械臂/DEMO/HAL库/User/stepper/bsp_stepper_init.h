#ifndef __BSP_STEP_MOTOR_INIT_H
#define __BSP_STEP_MOTOR_INIT_H

#include "stm32f1xx.h"
#include "stm32f1xx_hal.h"

//引脚定义
/*******************************************************/
//Motor 方向 
#define MOTOR1_DIR_PIN                   GPIO_PIN_8   
#define MOTOR1_DIR_GPIO_PORT             GPIOD                    
#define MOTOR1_DIR_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOD_CLK_ENABLE()
//Motor 使能 
#define MOTOR_EN_PIN                    GPIO_PIN_9
#define MOTOR_EN_GPIO_PORT              GPIOD                       
#define MOTOR_EN_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOD_CLK_ENABLE()
//Motor 脉冲
#define MOTOR1_PUL_PIN                   GPIO_PIN_6
#define MOTOR1_PUL_GPIO_PORT             GPIOC
#define MOTOR1_PUL_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOC_CLK_ENABLE()

/************************************************************/

/*******************************************************/
//Motor 方向 
#define MOTOR2_DIR_PIN                   GPIO_PIN_10   
#define MOTOR2_DIR_GPIO_PORT             GPIOD                    
#define MOTOR2_DIR_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOD_CLK_ENABLE()
//Motor 使能 
#define MOTOR_EN_PIN                    GPIO_PIN_9
#define MOTOR_EN_GPIO_PORT              GPIOD                       
#define MOTOR_EN_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOD_CLK_ENABLE()
//Motor 脉冲
#define MOTOR2_PUL_PIN                   GPIO_PIN_7
#define MOTOR2_PUL_GPIO_PORT             GPIOC
#define MOTOR2_PUL_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOC_CLK_ENABLE()

/************************************************************/

/*******************************************************/
//Motor 方向 
#define MOTOR3_DIR_PIN                   GPIO_PIN_12   
#define MOTOR3_DIR_GPIO_PORT             GPIOD                    
#define MOTOR3_DIR_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOD_CLK_ENABLE()
//Motor 使能 
#define MOTOR_EN_PIN                    GPIO_PIN_9
#define MOTOR_EN_GPIO_PORT              GPIOD                       
#define MOTOR_EN_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOD_CLK_ENABLE()
//Motor 脉冲
#define MOTOR3_PUL_PIN                   GPIO_PIN_8
#define MOTOR3_PUL_GPIO_PORT             GPIOC
#define MOTOR3_PUL_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOC_CLK_ENABLE()

/************************************************************/
#define HIGH GPIO_PIN_SET       //高电平
#define LOW  GPIO_PIN_RESET     //低电平

#define ON  LOW                 //开
#define OFF HIGH                //关

#define CW  HIGH                //顺时针
#define CCW LOW                 //逆时针

#define  normal_subdivide  16
#define  Z_subdivide  normal_subdivide*200/8      // 1mm  400steps 
#define  number_Z       1                         //PUL C6     DIR D8
#define  axle_1_subdivide  57.6     //normal_subdivide*(72/20)//PUL C7     DIR D10  
#define  axle_2_subdivide  93.1878//PUL C8     DIR D12
#define  axle2_disby_axle1   0.532

//控制使能引脚
/* 带参宏，可以像内联函数一样使用 */
#define MOTOR_EN(x)          HAL_GPIO_WritePin(MOTOR_EN_GPIO_PORT,MOTOR_EN_PIN,x)
#define MOTOR1_PUL(x)        HAL_GPIO_WritePin(MOTOR1_PUL_GPIO_PORT,MOTOR1_PUL_PIN,x)
#define MOTOR1_DIR(x)        HAL_GPIO_WritePin(MOTOR1_DIR_GPIO_PORT,MOTOR1_DIR_PIN,x)

#define MOTOR2_PUL(x)        HAL_GPIO_WritePin(MOTOR2_PUL_GPIO_PORT,MOTOR2_PUL_PIN,x)
#define MOTOR2_DIR(x)        HAL_GPIO_WritePin(MOTOR2_DIR_GPIO_PORT,MOTOR2_DIR_PIN,x)

#define MOTOR3_PUL(x)        HAL_GPIO_WritePin(MOTOR3_PUL_GPIO_PORT,MOTOR3_PUL_PIN,x)
#define MOTOR3_DIR(x)        HAL_GPIO_WritePin(MOTOR3_DIR_GPIO_PORT,MOTOR3_DIR_PIN,x)

extern void stepper_Init(void);
extern void stepper_turn(int tim,float angle,float subdivide,uint8_t dir,uint8_t x);  
void Zaxle_move(int tim,float distance,float subdivide,uint8_t dir);
void axle_1_move(int tim,float angle,int dir);
void axle_2_move(int tim,float angle,int dir);
void axle_2_move_theta(float tim ,float* last , float* next);
void axle_2_move_theta1(float tim ,float a ,float goat );


#endif /* __STEP_MOTOR_INIT_H */


