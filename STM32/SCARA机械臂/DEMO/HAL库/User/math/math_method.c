#include "math_method.h"
#include <math.h>
#include "stdio.h"
#include "./stepper/bsp_stepper_init.h"
#include "./tim/bsp_general_tim.h"

//所有长度计算都将换算成毫米进行
//theta
//float theta0 = 0;//Z轴步进电机旋转角度
//float theta1 = 0;//第一轴，步进电机角度 theata1为靠近螺杆的第一轴角度以此类推
//float theta2 = 0;//第二轴，步进电机角度
//float theta3 = 0;//第一轴，舵机角度


float L1 = 92.5;//毫米
float L2 = 84.5;//毫米
float L3 = 143.0;//毫米

float current_x = 0;
float current_y = 0;
float current_z = 0;

float target_x = 0;
float target_y = 0;
float target_z = 0;

float calculationdata[2];

// FORWARD KINEMATICS 正解算
void forward_calculation(float angle1 ,float angle2 , float angle3)//参数为角度
{
    float theta1F = angle1 * PI / 180;   // 角度转换成弧度
    float theta2F = angle2 * PI / 180;
	float theta3F = angle3 * PI / 180;
	
	calculationdata[0] =  L1*cos(angle1) + L2*cos(angle3) + L3;
	calculationdata[1] =  L1*sin(angle1) - L2*sin(angle3);
}

// INVERSE KINEMATICS
float *inverse_calculation(float x, float y)  
{
	float delta = 0; 
	float thetab = 0;
    float thetaa = 0;
	static float ptr[4]; 
	delta = x*x + y*y - 2*x*L3 + L3*L3;
	if(delta < 0) return 0;
	ptr[2] = acos((delta - L1*L1 - L2*L2)/(2*L1*L2))*180/PI;        // 弧度转换成角度
	thetab = acos((L1*L1 + delta - L2*L2)/(2*L1*sqrt(delta)))*180/PI;// 弧度转换成角度
	thetaa = atan(y/(x - L3))*180/PI;								  // 弧度转换成角度
	ptr[1] = thetaa + thetab ; //角度＋角度
	ptr[3] = ptr[2] - ptr[1];
	ptr[0] = 0;
	return ptr ;

}

float *relative_move(float x , float y ,float *movedtheta)
{
	float *angle;
	static float result[10];
	float angle1,angle2,angle3;
	angle = inverse_calculation( x,y);
	angle1 = angle[1];//直接角度,目标角度         
	angle2 = angle[2];
	angle3 = angle[3];
	
	printf("\nangle1 = %f\r\n" , angle1);
	printf("angle2 = %f\r\n" , angle2);
	printf("angle3 = %f\r\n" , angle3);
//	angle1 = angle[1];//直接角度,目标角度         
//	angle2 = angle[2];
//	angle3 = angle[3];
	
	result[1] = angle1 - movedtheta[1];//电机一轴 相对需要移动的角度
	if(result[1] > 0)
	{
		result[2] = 1;//正方向为逆时针
	}
	else result[2] = -1;
	result[1] = fabs(result[1]); //需要移动的相对角度绝对值
/******************************************/	
    result[3] = angle2 - movedtheta[2];	
	if(result[3] > 0)
	{
		result[4] = 1;//正方向为顺时针
	}
	else result[4] = -1;
	result[3] = fabs(result[3]);
/******************************************/		
	result[5] = angle3 - movedtheta[3];
		if(result[5] > 0)
	{
		result[6] = -1;//正方向为逆时针
	}
	else result[6] = 1;
	result[5] = fabs(result[5]);
/******************************************/			
	result[7] = angle1;//理论直接角度,目标角度         
	result[8] = angle2;
	result[9] = angle3;
	return result;
}






