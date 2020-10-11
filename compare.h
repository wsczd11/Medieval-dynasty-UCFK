#ifndef COMPARE_H
#define COMPARE_H

#include "system.h"
#include "pio.h"
#include "pacer.h"
#include "navswitch.h"
#include "ir_uart.h"
#include "button.h"
#include "led.h"

/* If player1_choice win, return 0, else return 1. (Compare) */
int compare_choice(uint8_t player1_choice, uint8_t player2_choice);


/* Determine whether player1 or player2 wins or its a tie */
int job(uint8_t current_choice, uint8_t current_column);

#endif
