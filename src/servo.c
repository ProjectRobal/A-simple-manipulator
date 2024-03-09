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
    
    double q3= M_PI - acos((L22+L32 - mani->z*mani->z - mani->y*mani->x*mani->x)/((double)(2*L2*L3)));
    
    double q1 = asin(mani->x/(sqrt(mani->x*mani->x + mani->y*mani->y)));

    double q2 = (M_PI/2.f) - atan(L3*sin(q3)/(L2+L3*cos(q3))) - atan(mani->z/(mani->y*mani->x));

    if(q1 != NAN)
    {
        mani->joints[0].angle=(q1/M_PI) * 180000 ;
    }
    if(q2 != NAN)
    {
        mani->joints[1].angle=(q2/M_PI) * 180000 ;
    }
    if(q3 != NAN)
    {
        mani->joints[2].angle=(q3/M_PI) * 180000 ;
    }
}