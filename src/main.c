#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>

#include "pca9685.h"

#include "joystick.h"

#include "servo.h"

#define UPDATE_TIME_MS 10

#define ON_BUTTON_PIN PD1
#define ON_BUTTON_DDR DDRD
#define ON_BUTTON_PORT PORTD
#define ON_BUTTON_OUT PIND


#define AXIS_BUTTON_PIN PD2
#define AXIS_BUTTON_DDR DDRD
#define AXIS_BUTTON_PORT PORTD
#define AXIS_BUTTON_OUT PIND


#define PWM_FREQ 50

volatile bool Enable=false;

// switch between x/y and x/z mode
volatile bool AxisMode=false;

#define MOVE_SPEED_COF 1000

int main()
{
    joystick_t joystick={0};
    
    manipulator_t manipulator={
        .x=0,
        .y=0,
        .z=0,
        .joints={
            {
                .channel=0,
                .angle=0
            },
            {
                .channel=1,
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

    // initialize servo controller
    pca9685_init(0x00,PWM_FREQ);

    // on/off button
    // input pin
    ON_BUTTON_DDR&=~(1<<ON_BUTTON_PIN);
    // hardware pullup
    ON_BUTTON_PORT|=(1<<ON_BUTTON_PIN);

    // joystick button input pin axis switch

    AXIS_BUTTON_DDR&=~(1<<AXIS_BUTTON_PIN);
    AXIS_BUTTON_PORT|=(1<<AXIS_BUTTON_PIN);

    while(1)
    {
        // check if button is on
        if(!(ON_BUTTON_OUT & (1<<ON_BUTTON_PIN)))
        {
            Enable=!Enable;
        }

        if(Enable)
        {
            if(!(AXIS_BUTTON_OUT & (1<<AXIS_BUTTON_PIN)))
            {
                AxisMode=!AxisMode;
            }

            update_joystick(&joystick);

            // move around axis

            if(AxisMode)
            {
                // x/y mode

                manipulator.x+=joystick.x/MOVE_SPEED_COF;
                manipulator.y+=joystick.y/MOVE_SPEED_COF;

            }
            else
            {
                // x/z mode

                manipulator.x+=joystick.x/MOVE_SPEED_COF;
                manipulator.z+=joystick.y/MOVE_SPEED_COF;
            }

            calculate_kinematic(&manipulator);
            set_servos(&manipulator);
        }

        _delay_ms(UPDATE_TIME_MS);
    }
}