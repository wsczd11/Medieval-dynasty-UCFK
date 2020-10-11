/** @file   beep1.c
    @author Tawatchai Holmes (Andy), Zhedong Cao (Barry)
    @date   15 OCT 2020
    @Descr:  Makefile for game
*/

#ifndef SPEAKER_H
#define SPEAKER_H

#include "system.h"
#include "pio.h"
#include "pacer.h"

#define PIEZO1_PIO PIO_DEFINE (PORT_D, 4)
#define PIEZO2_PIO PIO_DEFINE (PORT_D, 6)

/**
int main (void)
{
    system_init ();
    init_speaker();
    pacer_init (LOOP_RATE);

    while (1) {
        pacer_wait ();
        use_speaker();
    }
    return 0;
}
*/

/** Initialise speaker.  */
void init_speaker(void);


/** Turn on speaker.  */
void use_speaker(void);
#endif
