/** @file:      button.c
    @author:    Tawatchai Holmes (Andy), Zhedong Cao (Barry)
    @date:       15 OCT 2020
    @Descr:      Makefile for game
*/

#include "pio.h"
#include "button.h"

/* Return non-zero if button pressed.  */
int button_pressed_p (void)
{
    return pio_input_get(PD7_PIO);
}


/* Initialise button1.  */
void button_init (void)
{
    pio_config_set(PD7_PIO, PIO_INPUT);
}
