#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "uart.h"

#include "pca9685.h"

#include "joystick.h"

#include "servo.h"

#include <Arduino.h>

#define UPDATE_TIME_MS 25


#define AXIS_BUTTON_PIN PD2
#define AXIS_BUTTON_DDR DDRD
#define AXIS_BUTTON_PORT PORTD
#define AXIS_BUTTON_OUT PIND


#define PWM_FREQ 50


// switch between x/y and x/z mode
volatile bool AxisMode=false;

volatile bool buttonState=false;

#define MOVE_SPEED_COF 100.f

#define ROT_SPEED_COF 150.f

volatile bool lastButtonState=false;

volatile int debounce=0;

static double clip(double x)
{
    if( x > 500.f)
    {
        return 500.f;
    }

    if( x < -500.f)
    {
        return -500.f;
    }

    return x;
}

double clip_z(double x)
{
    if( x > 700.f)
    {
        return 700.f;
    }

    if( x < -150.f)
    {
        return -150.f;
    }

    return x;
}

int main()
{
    lastButtonState=AxisMode;

    joystick_t joystick={0};
    
    manipulator_t manipulator={
        .x=0,
        .y=300,
        .z=0,
        .joints={
            {
                .channel=1,
                .angle=0
            },
            {
                .channel=0,
                .angle=0
            },
            {
                .channel=2,
                .angle=0
            }
        }
    };

    // initialize ADC used by joystick
    init_adc();

    uart_init();

    // initialize servo controller
    pca9685_init(0x00,PWM_FREQ);

    // pin interrupt

    EICRA=(1<<ISC01);

    EIMSK=(1<<INT0);

    // joystick button input pin axis switch

    AXIS_BUTTON_DDR&=~(1<<AXIS_BUTTON_PIN);
    AXIS_BUTTON_PORT|=(1<<AXIS_BUTTON_PIN);

    lastButtonState=(AXIS_BUTTON_OUT & AXIS_BUTTON_PIN);

    while(1)
    {
        // check if button is on

        /*buttonState=(AXIS_BUTTON_OUT & AXIS_BUTTON_PIN);

        if(lastButtonState != buttonState)
        {

            if(!buttonState)
            {
                uart_print("Axis switch!");
                AxisMode=!AxisMode;
            }

            lastButtonState=buttonState;

        }*/

        //char buffer[256]={0};

        //sprintf(buffer,"X: %hu : Y: %hu \n",joystick.x,joystick.y);

        //uart_print(buffer);

        // move around axis

        //joystick.x=read_x_axis();
        //joystick.y=read_y_axis();

        joystick.x=analogRead(A0)-ZERO_POSITION;
        joystick.y=analogRead(A1)-ZERO_POSITION;

        if(abs(joystick.x)<100)
        {
            joystick.x=0;
        }

        if(abs(joystick.y)<100)
        {
            joystick.y=0;
        }

        if(AxisMode)
        {
            // x/y mode

           
            manipulator.y+=(float)joystick.y/MOVE_SPEED_COF;

        }
        else
        {
            // z mode

            manipulator.z+=(float)joystick.y/MOVE_SPEED_COF;
        }

        // y will drive angle on q3

         manipulator.x+=(float)joystick.x/ROT_SPEED_COF;

        manipulator.x=clip(manipulator.x);
        manipulator.y=clip(manipulator.y);

        manipulator.z=clip_z(manipulator.z);

        float xy0=sqrt(manipulator.x*manipulator.x + manipulator.y*manipulator.y);

        if((xy0 >= 280)||(xy0<=380))
        {

        calculate_kinematic(&manipulator);

        }

        char buffer[256]={0};

        sprintf(buffer," q1:%f : q2:%f q3:%f x:%f y:%f z:%f\n",
        manipulator.joints[0].angle,manipulator.joints[1].angle,manipulator.joints[2].angle,
        manipulator.x,manipulator.y,manipulator.z
        );

        uart_print(buffer);

        set_servos(&manipulator);

        if(debounce>0)
        {
            debounce--;
        }

        _delay_ms(UPDATE_TIME_MS);

    }

    return 0;
}

ISR(INT0_vect) 
{
    if(debounce==0)
    {
        AxisMode=!AxisMode;
        debounce=5;
    }
}