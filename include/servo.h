#pragma once

#include <pca9685.h>

#include <inttypes.h>


#define L2 (uint32_t)200
#define L3 (uint32_t)200

#define L22 (uint32_t)40000
#define L32 (uint32_t)40000


#define NUMBER_OF_JOINTS 3

typedef struct joint
{
    int32_t angle;
    uint8_t channel;
} joint_t;

typedef struct manipulator
{
    uint32_t x;
    uint32_t y;
    uint32_t z;
    joint_t joints[NUMBER_OF_JOINTS];
} manipulator_t;


void set_servos(manipulator_t* mani);

void calculate_kinematic(manipulator_t* mani);



