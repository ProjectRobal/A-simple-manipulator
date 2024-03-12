#pragma once

#include <inttypes.h>

#define ZERO_POSITION 512
#define DEAD_ZONE 50

#ifndef ADC_X_AXIS

    #define ADC_X_AXIS PC0

#endif

#ifndef ADC_Y_AXIS

    #define ADC_Y_AXIS PC1

#endif

typedef struct joystick
{
    int16_t x;
    int16_t y;
} joystick_t;

// initialize ADC Channels
void init_adc();

uint16_t read_adc(uint8_t channel);

int16_t read_x_axis();

int16_t read_y_axis();

void update_joystick(joystick_t* joy);
