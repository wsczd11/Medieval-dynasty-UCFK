/** @file   beep1.c
    @author Tawatchai Holmes (Andy), Zhedong Cao (Barry)
    @date   15 OCT 2020
    @Descr:  Makefile for game
*/

#include "system.h"
#include "pio.h"
#include "pacer.h"

/* Connect piezo tweeter to pins 6 and 8 of UCFK4 P1 connector
   for push-pull operation.  */
#define PIEZO1_PIO PIO_DEFINE (PORT_D, 4)
#define PIEZO2_PIO PIO_DEFINE (PORT_D, 6)

void init_speaker(void)
{
    pio_config_set (PIEZO1_PIO, PIO_OUTPUT_LOW);
    pio_config_set (PIEZO2_PIO, PIO_OUTPUT_HIGH);
}

void use_speaker()
{
    pio_output_toggle (PIEZO1_PIO);
    pio_output_toggle (PIEZO2_PIO);
}
