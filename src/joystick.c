#include "joystick.h"

#include <avr/io.h>
#include <avr/interrupt.h>

#include <stdlib.h>
#include <stdio.h>
// initialize ADC Channels
void init_adc()
{
    cli();

    DDRC&=~(1<<ADC_X_AXIS | 1<<ADC_Y_AXIS);

    //ADMUX|=(1<<REFS0);   

    ADCSRA=(1<<ADEN)|(1<<ADPS0)|(1<<ADPS1)|(1<<ADPS2); 

    sei();
}

uint16_t read_adc(uint8_t channel)
{
    // clear all channel
    ADMUX&=~((1<<MUX2)|(1<<MUX1)|(1<<MUX0));

    // select channel
    ADMUX=(1<<REFS0)|( channel & 0b111 );
    
    // perform conversion
    ADCSRA|=(1<<ADSC);

    while(ADCSRA & (1<<ADSC));

    return (uint16_t)((ADCH<<8)) | ADCL;

}

int16_t read_x_axis()
{
    return read_adc(ADC_X_AXIS)-ZERO_POSITION;
}

int16_t read_y_axis()
{
    return read_adc(ADC_Y_AXIS)-ZERO_POSITION;
}

void update_joystick(joystick_t* joy)
{
    joy->x=read_x_axis();
    joy->y=read_y_axis();

    if(abs(joy->x)<=DEAD_ZONE)
    {
        joy->x=0;
    }
    if(abs(joy->y)<=DEAD_ZONE)
    {
        joy->y=0;
    }
}