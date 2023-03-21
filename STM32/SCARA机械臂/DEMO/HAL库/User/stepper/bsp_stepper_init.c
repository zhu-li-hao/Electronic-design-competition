/**
  ******************************************************************************
  * @file    bsp_stepper_init.c
  * @author  fire
  * @version V1.0
  * @date    2019-xx-xx
  * @brief   ��ʼ��
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��  STM32 F103 ������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
#include "./stepper/bsp_stepper_init.h"
#include "bsp_debug_usart.h"

#include "core_delay.h"   

/**
  * @brief  ���������ת
  * @param  tim         �������� ��λMS ����Խ��Ƶ��Խ�ߣ�ת��Խ�� ϸ��Ϊ1ʱ����10ms
  * @param  angle       ��Ҫת���ĽǶ�ֵ
  * @param  dir       ѡ������ת(ȡֵ��Χ��0,1) 
  * @param  subdivide   ϸ��ֵ
  * @note   ��
  * @retval ��
  */
void stepper_turn(int tim,float angle,float subdivide,uint8_t dir,uint8_t x)  
{
  int n,i;
  /*����ϸ������ò���Ǳ��ֳɶ��ٸ�����*/
  n=(int)(angle/(1.8/subdivide));
  switch(x)
  {
	  case 1:  if(dir==CW) //˳ʱ��
				  {
					MOTOR1_DIR(CW);
				  }
				  else if(dir==CCW)//��ʱ��
				  {
					MOTOR1_DIR(CCW);
				  }
				  /*��ʹ��*/
				  MOTOR_EN(ON);
				  /*ģ�ⷽ��*/
				  for(i=0;i<n;i++)
				  {   
					MOTOR1_PUL(HIGH);
					delay_us(tim/2);
					MOTOR1_PUL(LOW);
					delay_us(tim/2);
				  }
				  break;
	  case 2:  if(dir==CW) //˳ʱ��
				  {
					MOTOR2_DIR(CW);
				  }
				  else if(dir==CCW)//��ʱ��
				  {
					MOTOR2_DIR(CCW);
				  }
				  /*��ʹ��*/
				  MOTOR_EN(ON);
				  /*ģ�ⷽ��*/
				  for(i=0;i<n;i++)
				  {   
					MOTOR2_PUL(HIGH);
					delay_us(tim/2);
					MOTOR2_PUL(LOW);
					delay_us(tim/2);
				  }
				  break;
	  case 3:  if(dir==CW) //˳ʱ��
				  {
					MOTOR3_DIR(CW);
				  }
				  else if(dir==CCW)//��ʱ��
				  {
					MOTOR3_DIR(CCW);
				  }
				  /*��ʹ��*/
				  MOTOR_EN(ON);
				  /*ģ�ⷽ��*/
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
  /*��ʹ��*/
  MOTOR_EN(OFF);
}


void Zaxle_move(int tim,float distance,float subdivide,uint8_t dir)  
{
  int n,i;
  /*����ϸ������ò���Ǳ��ֳɶ��ٸ�����*/
  n=(int)((distance*200*16)/8);
  if(dir==CW) //˳ʱ��
	{
		MOTOR1_DIR(CW);
	}
	else if(dir==CCW)//��ʱ��
	{
		MOTOR1_DIR(CCW);
	}
	/*��ʹ��*/
	MOTOR_EN(ON);
	/*ģ�ⷽ��*/
	 for(i=0;i<n;i++)
	 {   
	MOTOR1_PUL(HIGH);
	delay_us(tim/2);
	MOTOR1_PUL(LOW);
	delay_us(tim/2);
	 }
  /*��ʹ��*/
  MOTOR_EN(OFF);
}

void axle_1_move(int tim,float angle,int dir)   //dir������������  ��1������Ϊ��ʱ��
{
	if(dir == 1)dir = 0;//������ת�� ��ʱ�� dir = 0 
	else dir = 1;       //�淽��ת�� ˳ʱ�� dir = 1
	stepper_turn( tim, angle, axle_1_subdivide, dir, 1);//stepper_turn(int tim,float angle,float subdivide,uint8_t dir,uint8_t x) 
	
}

void axle_2_move(int tim,float angle,int dir)    //dir������������  ��2������Ϊ˳ʱ��
{
	if(dir == 1)dir = 1;//������ת�� ˳ʱ�� dir = 1 
	else dir = 0;       //�淽��ת�� ��ʱ�� dir = 0
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
			deltatheta = a - b ;//�� 2 ��ʱ��ת�� �� ������
			axle_2_move(tim , deltatheta , 0);
		}
		else 
		{
			deltatheta = b - a ;//�� 2 ˳ʱ��ת�� �� ������ �� ���ߦ�theta = 0���� 2 ��ת��
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
			deltatheta = b -a ; //�� 2 ˳ʱ��ת���� ������
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

	else //�� 1 û���ƶ��������� 2 �ƶ�
	{
		a = *(last+2);
		b = *(next+2);
		if(a >b)
		{
			deltatheta = a - b;//�� 2 ��ʱ��ת�� ������
			axle_2_move(tim , deltatheta , 0);			
		}
		else
		{
			deltatheta = b - a;//�� 2 ˳ʱ��ת�� ������
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
  * @brief  ���ų�ʼ��
  * @retval ��
  */
void stepper_Init()
{
  /*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
  GPIO_InitTypeDef  GPIO_InitStruct;

  /*����Motor��ص�GPIO����ʱ��*/
  MOTOR1_DIR_GPIO_CLK_ENABLE();
  MOTOR1_PUL_GPIO_CLK_ENABLE();
	
  MOTOR2_DIR_GPIO_CLK_ENABLE();
  MOTOR2_PUL_GPIO_CLK_ENABLE();
	
  MOTOR3_DIR_GPIO_CLK_ENABLE();
  MOTOR3_PUL_GPIO_CLK_ENABLE();
	
  MOTOR_EN_GPIO_CLK_ENABLE();

  /*ѡ��Ҫ���Ƶ�GPIO����*/                                 
  GPIO_InitStruct.Pin = MOTOR1_DIR_PIN|MOTOR2_DIR_PIN|MOTOR3_DIR_PIN;  //D8 D10 D12

  /*�������ŵ��������Ϊ�������*/
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;  

  GPIO_InitStruct.Pull =GPIO_PULLUP;

  /*������������Ϊ���� */   
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

  /*Motor �������� ��ʼ��*/ 
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);                               //DIR INIT   D8 D10 D12

  /*Motor �������� ��ʼ��*/
  GPIO_InitStruct.Pin = MOTOR1_PUL_PIN|MOTOR2_PUL_PIN|MOTOR3_PUL_PIN;   //PUL INIT  C6 C7 C8
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Motor ʹ������ ��ʼ��*/
  GPIO_InitStruct.Pin = MOTOR_EN_PIN;                                   //EN INIT   D9
  HAL_GPIO_Init(MOTOR_EN_GPIO_PORT, &GPIO_InitStruct);  

  /*�ص�ʹ��*/
  MOTOR_EN(OFF);
}





















