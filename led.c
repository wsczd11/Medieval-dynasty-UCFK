/** @file   beep1.c
    @author Tawatchai Holmes (Andy), Zhedong Cao (Barry)
    @date   15 OCT 2020
    @Descr:  Makefile for game
*/

#include "pio.h"
#include "led.h"

/* Turn LED on.  */
void led_on (void)
{
    pio_output_high(PC2_PIO);
}


/* Turn LED off.  */
void led_off (void)
{
    pio_output_low(PC2_PIO);
}


/* Initialise LED.  */
void led_init (void)
{
    pio_config_set(PC2_PIO, PIO_OUTPUT_HIGH);
}
