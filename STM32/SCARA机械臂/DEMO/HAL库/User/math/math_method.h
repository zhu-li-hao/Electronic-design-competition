#ifndef __MATH_CALCULATION_H
#define __MATH_CALCULATION_H

#include "stm32f1xx.h"
#include "stdio.h"

#define PI 3.141592

struct math_theta { 

    float theta1, theta2,theta3,fulse; //×ª¶¯½Ç¶È
}; 

typedef struct math_theta theta_Struct; 



void forward_calculation(float angle1 ,float angle2 , float angle3);
float *inverse_calculation(float x, float y) ;
float *relative_move(float x , float y ,float movedtheta[] );

#endif /* __MATH_CALCULATION_H */

