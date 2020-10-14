/** @file:      icon.h
    @author:    Tawatchai Holmes (Andy) tho78, Zhedong Cao (Barry) zca19
    @date:      15 OCT 2020
    @Descr:     icon functions in .h format
*/

#ifndef ICON_H
#define ICON_H

#include "system.h"
#include "pio.h"
#include "pacer.h"
#include "navswitch.h"

/*Define PIO pins driving LED matrix rows.*/
static const pio_t rows[];


/*Define PIO pins driving LED matrix columns.*/
static const pio_t cols[];


/* Initilise the LED Metrics */
void ledm_init(void);


/* Display the LED */
void display_column(uint8_t row_pattern, uint8_t current_column);


#endif
