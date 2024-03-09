#pragma once

#include <inttypes.h>

#ifndef ADC_X_AXIS

    #define ADC_X_AXIS PC0

#endif

#ifndef ADC_Y_AXIS

    #define ADC_Y_AXIS PC1

#endif

typedef struct joystick
{
    uint16_t x;
    uint16_t y;
} joystick_t;

// initialize ADC Channels
void init_adc();

uint16_t read_adc(uint8_t channel);

uint16_t read_x_axis();

uint16_t read_y_axis();

void update_joystick(joystick_t* joy);
