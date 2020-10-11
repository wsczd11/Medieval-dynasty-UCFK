#ifndef DISPLAY_H
#define DISPLAY_H

#include "system.h"
#include "pio.h"
#include "pacer.h"
#include "navswitch.h"
#include "ir_uart.h"
#include "button.h"
#include "led.h"

/* Initialise LED */
void inf(void);

/* Initialise and display the LED */
void display_column (uint8_t row_pattern, uint8_t current_column);


/* Display 'W' on Winner screen */
void winner_display(uint8_t current_choice, uint8_t current_column);


/* Display 'L' on Loser screen */
void loser_display(uint8_t current_choice, uint8_t current_column);


/* Display 'S' on both of the screen when they have the same choice */
void equal_display(uint8_t current_choice, uint8_t current_column);

#endif
