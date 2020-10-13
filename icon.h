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


/*Different choices for the game*/
static const uint8_t bitmap[];


/*Display the letter 'W'*/
static const uint8_t winner_screen[];


/* Display the letter 'L' */
static const uint8_t loser_screen[];


/* Display the letter 'S' */
static const uint8_t equal_screen[];


/* Initilise the LED Metrics */
void ledm_init(void);


/* Display the LED */
void display_column(uint8_t row_pattern, uint8_t current_column);


void setting_saver(char key, uint8_t Player_1, uint8_t current_column);


#endif
