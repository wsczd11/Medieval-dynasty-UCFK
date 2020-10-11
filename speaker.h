/** @file:      beep1.c
    @author:    Tawatchai Holmes (Andy) tho78, Zhedong Cao (Barry) zca19
    @date:      15 OCT 2020
    @Descr:     Speaker function in .h form
*/

#ifndef SPEAKER_H
#define SPEAKER_H

#include "system.h"
#include "pio.h"
#include "pacer.h"

#define PIEZO1_PIO PIO_DEFINE (PORT_D, 4)
#define PIEZO2_PIO PIO_DEFINE (PORT_D, 6)


/* Initialise speaker */
void init_speaker(void);


/* Output sound from speaker */
void use_speaker(void);

#endif
