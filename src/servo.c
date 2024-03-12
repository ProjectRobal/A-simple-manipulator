#include <math.h>
#include "servo.h"


void set_servos(manipulator_t* mani)
{
    for(uint8_t i=0;i<NUMBER_OF_JOINTS;i++)
    {
        pca9685_servo(mani->joints[i].channel,mani->joints[i].angle);
    }
}

// do magic with kinematic, position x and y use to calculate each angel
void calculate_kinematic(manipulator_t* mani)
{

    double xy0=mani->x*mani->x + mani->y*mani->y;
    
    double q3= acos((mani->z*mani->z + xy0 -L22-L32)/((double)(2*L2*L3)));
    
    double q1 = asin(mani->y/(sqrt(mani->x*mani->x + mani->y*mani->y)));

    double q2 = (M_PI/2.f) - atan(mani->z/sqrt(xy0)) - atan(L3*sin(-q3)/(L2+L3*cos(-q3)));

    if(q1 != NAN)
    {
        mani->joints[0].angle=((q1/M_PI) * 180.f)-90.f;
    }
    if(q2 != NAN)
    {
        mani->joints[1].angle=((q2/M_PI) * 180.f)-90.f ;
    }
    if(q3 != NAN)
    {
        mani->joints[2].angle=((q3/M_PI) * 180.f)-90.f ;
    }
}