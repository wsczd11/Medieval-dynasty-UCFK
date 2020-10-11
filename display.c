#include <avr/io.h>
#include "system.h"
#include "pio.h"
#include "pacer.h"
#include "navswitch.h"
#include "ir_uart.h"
#include "button.h"
#include "led.h"
#include "compare.h"

/* Define PIO pins driving LED matrix rows.  */
const pio_t rows[] = {
    LEDMAT_ROW1_PIO, LEDMAT_ROW2_PIO, LEDMAT_ROW3_PIO,
    LEDMAT_ROW4_PIO, LEDMAT_ROW5_PIO, LEDMAT_ROW6_PIO,
    LEDMAT_ROW7_PIO
};

/* Define PIO pins driving LED matrix columns.  */
const pio_t cols[] = {
    LEDMAT_COL1_PIO, LEDMAT_COL2_PIO, LEDMAT_COL3_PIO,
    LEDMAT_COL4_PIO, LEDMAT_COL5_PIO
};
/* Display the letter 'W' */
const uint8_t winner_screen[] = {
    0x0E, 0x30, 0x0E, 0x30, 0x0E
};

/* Display the letter 'L' */
const uint8_t loser_screen[] = {
    0x00, 0x3E, 0x20, 0x20, 0x00
};

/* Display the letter 'S' */
const uint8_t equal_screen[] = {
    0x00, 0x24, 0x2A, 0x12, 0x00
};

/* Initialise LED */
void inf(void)
{
    for (int i = 0; i < 5; i++) {
        pio_config_set(cols[i], PIO_OUTPUT_HIGH);
    }
    for (int j = 0; j < 7; j++) {
        pio_config_set(rows[j], PIO_OUTPUT_HIGH);
    }
}

/* Display the LED */
void display_column (uint8_t row_pattern, uint8_t current_column)
{
    for (int i = 0; i < 5; i++) {
        pio_output_high(cols[i]);
    }
    for (int j = 0; j < 7; j++) {
        pio_output_high(rows[j]);
    }

    pio_output_low(cols[current_column]);
    for (uint8_t current_row = 0; current_row < 7; current_row++) {
        if ((row_pattern >> current_row) & 1) {
            pio_output_low(rows[current_row]);
        } else {
            pio_output_high(rows[current_row]);
        }
    }
}

/* Display 'W' on Winner screen */
void winner_display(uint8_t current_choice, uint8_t current_column)
{
    while (1) {
        pacer_wait();
        navswitch_update ();
        display_column (winner_screen[current_column], current_column);
        current_column++;
        if (current_column == 5) {
            current_column = 0;
        }
        if (button_pressed_p()) {
            ir_uart_putc (current_choice + 1);
        }
        if (navswitch_push_event_p (NAVSWITCH_PUSH)) {
            break;
        }
    }
}


/* Display 'L' on Loser screen */
void loser_display(uint8_t current_choice, uint8_t current_column)
{
    while (1) {
        pacer_wait();
        navswitch_update ();
        display_column (loser_screen[current_column], current_column);
        current_column++;
        if (current_column == 5) {
            current_column = 0;
        }
        if (button_pressed_p()) {
            ir_uart_putc (current_choice + 1);
        }
        if (navswitch_push_event_p (NAVSWITCH_PUSH)) {
            break;
        }
    }
}


/* Display 'S' on both of the screen when they have the same choice */
void equal_display(uint8_t current_choice, uint8_t current_column)
{
    while (1) {
        pacer_wait();
        navswitch_update ();
        display_column (equal_screen[current_column], current_column);
        current_column++;
        if (current_column == 5) {
            current_column = 0;
        }
        if (button_pressed_p()) {
            ir_uart_putc (current_choice + 1);
        }
        if (navswitch_push_event_p (NAVSWITCH_PUSH)) {
            break;
        }
    }
}
