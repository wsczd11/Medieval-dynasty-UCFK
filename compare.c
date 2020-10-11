#include <avr/io.h>
#include "system.h"
#include "pio.h"
#include "pacer.h"
#include "navswitch.h"
#include "ir_uart.h"
#include "button.h"
#include "led.h"
#include "display.h"



/* If player1_choice win, return 0, else return 1. (Compare) */
int compare_choice(uint8_t player1_choice, uint8_t player2_choice)
{
    if (player1_choice == 0) {
        if (player2_choice == 2 || player2_choice == 3) {
            return 0;
        } else {
            return 1;
        }
    } else if (player1_choice == 1) {
        if (player2_choice == 0 || player2_choice == 4) {
            return 0;
        } else {
            return 1;
        }
    } else if (player1_choice == 2) {
        if (player2_choice == 1 || player2_choice == 4) {
            return 0;
        } else {
            return 1;
        }
    } else if (player1_choice == 3) {
        if (player2_choice == 1 || player2_choice == 2) {
            return 0;
        } else {
            return 1;
        }
    } else {
        if (player2_choice == 0 || player2_choice == 3) {
            return 0;
        } else {
            return 1;
        }
    }
}

/* Determine whether player1 or player2 wins or its a tie */
int job(uint8_t current_choice, uint8_t current_column)
{
    uint8_t partner_choice;
    inf();
    while(1) {
        pacer_wait();

        if (button_pressed_p()) {
            ir_uart_putc (current_choice + 1);
        }
        if (ir_uart_read_ready_p()) {
            partner_choice = ir_uart_getc() - 1;
            if (partner_choice != 0 && partner_choice != 1 && partner_choice != 2 && partner_choice != 3 && partner_choice != 4) {
                continue;
            } else if (current_choice == partner_choice) {
                equal_display(current_choice, current_column);
                return 0;
            } else if (compare_choice(current_choice, partner_choice) == 0) {
                //current_choice win
                winner_display(current_choice, current_column);
                return 1;
            } else {
                //partner_choice win
                loser_display(current_choice, current_column);
                return 0;
            }
        }
        current_column++;
        if (current_column == 5) {
            current_column = 0;
        }
    }
}
