/** @file:       speaker.c
    @author:     Tawatchai Holmes (Andy), Zhedong Cao (Barry)
    @date:       15 OCT 2020
    @Descr:      Speaker function used in game.c to give the speaker sound
*/

#include "system.h"
#include "pio.h"
#include "pacer.h"

#define PIEZO1_PIO PIO_DEFINE (PORT_D, 4)
#define PIEZO2_PIO PIO_DEFINE (PORT_D, 6)


/* Initialise speaker */
void init_speaker(void)
{
    pio_config_set (PIEZO1_PIO, PIO_OUTPUT_LOW);
    pio_config_set (PIEZO2_PIO, PIO_OUTPUT_HIGH);
}


/* Output sound from speaker */
void use_speaker()
{
    pio_output_toggle (PIEZO1_PIO);
    pio_output_toggle (PIEZO2_PIO);
}
