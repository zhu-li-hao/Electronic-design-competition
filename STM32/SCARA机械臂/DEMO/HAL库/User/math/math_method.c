#include "math_method.h"
#include <math.h>
#include "stdio.h"
#include "./stepper/bsp_stepper_init.h"
#include "./tim/bsp_general_tim.h"

//���г��ȼ��㶼������ɺ��׽���
//theta
//float theta0 = 0;//Z�Ჽ�������ת�Ƕ�
//float theta1 = 0;//��һ�ᣬ��������Ƕ� theata1Ϊ�����ݸ˵ĵ�һ��Ƕ��Դ�����
//float theta2 = 0;//�ڶ��ᣬ��������Ƕ�
//float theta3 = 0;//��һ�ᣬ����Ƕ�


float L1 = 92.5;//����
float L2 = 84.5;//����
float L3 = 143.0;//����

float current_x = 0;
float current_y = 0;
float current_z = 0;

float target_x = 0;
float target_y = 0;
float target_z = 0;

float calculationdata[2];

// FORWARD KINEMATICS ������
void forward_calculation(float angle1 ,float angle2 , float angle3)//����Ϊ�Ƕ�
{
    float theta1F = angle1 * PI / 180;   // �Ƕ�ת���ɻ���
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
	ptr[2] = acos((delta - L1*L1 - L2*L2)/(2*L1*L2))*180/PI;        // ����ת���ɽǶ�
	thetab = acos((L1*L1 + delta - L2*L2)/(2*L1*sqrt(delta)))*180/PI;// ����ת���ɽǶ�
	thetaa = atan(y/(x - L3))*180/PI;								  // ����ת���ɽǶ�
	ptr[1] = thetaa + thetab ; //�Ƕȣ��Ƕ�
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
	angle1 = angle[1];//ֱ�ӽǶ�,Ŀ��Ƕ�         
	angle2 = angle[2];
	angle3 = angle[3];
	
	printf("\nangle1 = %f\r\n" , angle1);
	printf("angle2 = %f\r\n" , angle2);
	printf("angle3 = %f\r\n" , angle3);
//	angle1 = angle[1];//ֱ�ӽǶ�,Ŀ��Ƕ�         
//	angle2 = angle[2];
//	angle3 = angle[3];
	
	result[1] = angle1 - movedtheta[1];//���һ�� �����Ҫ�ƶ��ĽǶ�
	if(result[1] > 0)
	{
		result[2] = 1;//������Ϊ��ʱ��
	}
	else result[2] = -1;
	result[1] = fabs(result[1]); //��Ҫ�ƶ�����ԽǶȾ���ֵ
/******************************************/	
    result[3] = angle2 - movedtheta[2];	
	if(result[3] > 0)
	{
		result[4] = 1;//������Ϊ˳ʱ��
	}
	else result[4] = -1;
	result[3] = fabs(result[3]);
/******************************************/		
	result[5] = angle3 - movedtheta[3];
		if(result[5] > 0)
	{
		result[6] = -1;//������Ϊ��ʱ��
	}
	else result[6] = 1;
	result[5] = fabs(result[5]);
/******************************************/			
	result[7] = angle1;//����ֱ�ӽǶ�,Ŀ��Ƕ�         
	result[8] = angle2;
	result[9] = angle3;
	return result;
}






