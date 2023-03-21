/**
  ******************************************************************************
  * @file    bsp_stepper_init.c
  * @author  fire
  * @version V1.0
  * @date    2019-xx-xx
  * @brief   初始化
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火  STM32 F103 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
#include "./stepper/bsp_stepper_init.h"
#include "bsp_debug_usart.h"

#include "core_delay.h"   

/**
  * @brief  步进电机旋转
  * @param  tim         方波周期 单位MS 周期越短频率越高，转速越快 细分为1时最少10ms
  * @param  angle       需要转动的角度值
  * @param  dir       选择正反转(取值范围：0,1) 
  * @param  subdivide   细分值
  * @note   无
  * @retval 无
  */
void stepper_turn(int tim,float angle,float subdivide,uint8_t dir,uint8_t x)  
{
  int n,i;
  /*根据细分数求得步距角被分成多少个方波*/
  n=(int)(angle/(1.8/subdivide));
  switch(x)
  {
	  case 1:  if(dir==CW) //顺时针
				  {
					MOTOR1_DIR(CW);
				  }
				  else if(dir==CCW)//逆时针
				  {
					MOTOR1_DIR(CCW);
				  }
				  /*开使能*/
				  MOTOR_EN(ON);
				  /*模拟方波*/
				  for(i=0;i<n;i++)
				  {   
					MOTOR1_PUL(HIGH);
					delay_us(tim/2);
					MOTOR1_PUL(LOW);
					delay_us(tim/2);
				  }
				  break;
	  case 2:  if(dir==CW) //顺时针
				  {
					MOTOR2_DIR(CW);
				  }
				  else if(dir==CCW)//逆时针
				  {
					MOTOR2_DIR(CCW);
				  }
				  /*开使能*/
				  MOTOR_EN(ON);
				  /*模拟方波*/
				  for(i=0;i<n;i++)
				  {   
					MOTOR2_PUL(HIGH);
					delay_us(tim/2);
					MOTOR2_PUL(LOW);
					delay_us(tim/2);
				  }
				  break;
	  case 3:  if(dir==CW) //顺时针
				  {
					MOTOR3_DIR(CW);
				  }
				  else if(dir==CCW)//逆时针
				  {
					MOTOR3_DIR(CCW);
				  }
				  /*开使能*/
				  MOTOR_EN(ON);
				  /*模拟方波*/
				  for(i=0;i<n;i++)
				  {   
					MOTOR3_PUL(HIGH);
					delay_us(tim/2);
					MOTOR3_PUL(LOW);
					delay_us(tim/2);
				  }
				  break;
	 default:break;
  }
  /*关使能*/
  MOTOR_EN(OFF);
}


void Zaxle_move(int tim,float distance,float subdivide,uint8_t dir)  
{
  int n,i;
  /*根据细分数求得步距角被分成多少个方波*/
  n=(int)((distance*200*16)/8);
  if(dir==CW) //顺时针
	{
		MOTOR1_DIR(CW);
	}
	else if(dir==CCW)//逆时针
	{
		MOTOR1_DIR(CCW);
	}
	/*开使能*/
	MOTOR_EN(ON);
	/*模拟方波*/
	 for(i=0;i<n;i++)
	 {   
	MOTOR1_PUL(HIGH);
	delay_us(tim/2);
	MOTOR1_PUL(LOW);
	delay_us(tim/2);
	 }
  /*关使能*/
  MOTOR_EN(OFF);
}

void axle_1_move(int tim,float angle,int dir)   //dir代表正负方向  轴1正方向为逆时针
{
	if(dir == 1)dir = 0;//正方向转动 逆时针 dir = 0 
	else dir = 1;       //逆方向转动 顺时针 dir = 1
	stepper_turn( tim, angle, axle_1_subdivide, dir, 1);//stepper_turn(int tim,float angle,float subdivide,uint8_t dir,uint8_t x) 
	
}

void axle_2_move(int tim,float angle,int dir)    //dir代表正负方向  轴2正方向为顺时针
{
	if(dir == 1)dir = 1;//正方向转动 顺时针 dir = 1 
	else dir = 0;       //逆方向转动 逆时针 dir = 0
	stepper_turn( tim, angle, axle_2_subdivide, dir, 2);//stepper_turn(int tim,float angle,float subdivide,uint8_t dir,uint8_t x) 
}

void axle_2_move_theta(float tim ,float* last , float* next)
{
	float a , b , deltatheta;
	if(*(last+1) > *(next+1))
	{
		a = *(last+2) - *(next+1)*axle2_disby_axle1 ;
		b = *(next+2);
		if( a > b )
		{
			deltatheta = a - b ;//轴 2 逆时针转动 ， 反方向
			axle_2_move(tim , deltatheta , 0);
		}
		else 
		{
			deltatheta = b - a ;//轴 2 顺时针转动 ， 正方向 ， 或者Δtheta = 0，轴 2 不转动
			axle_2_move(tim , deltatheta , 1);
		}	
	printf("\n a0 = %f\r\n", a);
	printf("\n b0 = %f\r\n", b);
	}

	else if(*(last+1)  <  *(next+1))
	{
		a = *(last+2) + *(next+1)*axle2_disby_axle1 ;
		b = *(next+2);
		if(a < b)
		{
			deltatheta = b -a ; //轴 2 顺时针转动， 正方向
			axle_2_move(tim , deltatheta , 1);
		}
		if(a > b)
		{
			deltatheta = a - b ;
			axle_2_move(tim , deltatheta , 0);
		}	
	printf("\n a1 = %f\r\n", a);
	printf("\n b1 = %f\r\n", b);
	}

	else //轴 1 没有移动，单纯轴 2 移动
	{
		a = *(last+2);
		b = *(next+2);
		if(a >b)
		{
			deltatheta = a - b;//轴 2 逆时针转动 反方向
			axle_2_move(tim , deltatheta , 0);			
		}
		else
		{
			deltatheta = b - a;//轴 2 顺时针转动 正方向
			axle_2_move(tim , deltatheta , 1);			
		}	
		printf("\n a = %f\r\n", a);
		printf("\n b = %f\r\n", b);
	}


}	

void axle_2_move_theta1(float tim ,float a ,float goat )
{
	float moved = a*(axle2_disby_axle1);	
	float temp = 0;
 	if( moved < goat)
	{
		temp = goat - moved;
        axle_2_move(tim , temp , 1);
	}
	else if( moved > goat)
	{
		temp = moved - goat;
		axle_2_move(tim,  temp , 0);
	}
	else axle_2_move(tim,0, 0);
}



	

/**
  * @brief  引脚初始化
  * @retval 无
  */
void stepper_Init()
{
  /*定义一个GPIO_InitTypeDef类型的结构体*/
  GPIO_InitTypeDef  GPIO_InitStruct;

  /*开启Motor相关的GPIO外设时钟*/
  MOTOR1_DIR_GPIO_CLK_ENABLE();
  MOTOR1_PUL_GPIO_CLK_ENABLE();
	
  MOTOR2_DIR_GPIO_CLK_ENABLE();
  MOTOR2_PUL_GPIO_CLK_ENABLE();
	
  MOTOR3_DIR_GPIO_CLK_ENABLE();
  MOTOR3_PUL_GPIO_CLK_ENABLE();
	
  MOTOR_EN_GPIO_CLK_ENABLE();

  /*选择要控制的GPIO引脚*/                                 
  GPIO_InitStruct.Pin = MOTOR1_DIR_PIN|MOTOR2_DIR_PIN|MOTOR3_DIR_PIN;  //D8 D10 D12

  /*设置引脚的输出类型为推挽输出*/
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;  

  GPIO_InitStruct.Pull =GPIO_PULLUP;

  /*设置引脚速率为高速 */   
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

  /*Motor 方向引脚 初始化*/ 
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);                               //DIR INIT   D8 D10 D12

  /*Motor 脉冲引脚 初始化*/
  GPIO_InitStruct.Pin = MOTOR1_PUL_PIN|MOTOR2_PUL_PIN|MOTOR3_PUL_PIN;   //PUL INIT  C6 C7 C8
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Motor 使能引脚 初始化*/
  GPIO_InitStruct.Pin = MOTOR_EN_PIN;                                   //EN INIT   D9
  HAL_GPIO_Init(MOTOR_EN_GPIO_PORT, &GPIO_InitStruct);  

  /*关掉使能*/
  MOTOR_EN(OFF);
}





















