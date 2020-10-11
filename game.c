#include "system.h"
#include "pio.h"
#include "pacer.h"
#include "navswitch.h"
#include "ir_uart.h"
#include "button.h"
#include "led.h"



/** Define PIO pins driving LED matrix rows.  */
static const pio_t rows[] = {
    LEDMAT_ROW1_PIO, LEDMAT_ROW2_PIO, LEDMAT_ROW3_PIO,
    LEDMAT_ROW4_PIO, LEDMAT_ROW5_PIO, LEDMAT_ROW6_PIO,
    LEDMAT_ROW7_PIO
};


/** Define PIO pins driving LED matrix columns.  */
static const pio_t cols[] = {
    LEDMAT_COL1_PIO, LEDMAT_COL2_PIO, LEDMAT_COL3_PIO,
    LEDMAT_COL4_PIO, LEDMAT_COL5_PIO
};

//Different choices
static const uint8_t bitmap[] = {
    0x00, 0x10, 0x3E, 0x10, 0x00,      //sword
    0x00, 0x1E, 0x3E, 0x1E, 0x00,      //shield
    0x00, 0x28, 0x1C, 0x28, 0x00,      //human
    0x04, 0x04, 0x0C, 0x04, 0x1C,      //gun
    0x00, 0x38, 0x3E, 0x3A, 0x00       //grenade
};

//Turn off the screen
static const uint8_t turnoff_screen[] = {
    0x00, 0x00, 0x00, 0x00, 0x00,
};

//Display the letter 'W'
static const uint8_t winner_screen[] = {
    0x0E, 0x30, 0x0E, 0x30, 0x0E
};

//Display the letter 'L'
static const uint8_t loser_screen[] = {
    0x00, 0x3F, 0x20, 0x20, 0x00
};

//Display the letter 'S'
static const uint8_t equal_screen[] = {
    0x00, 0x24, 0x2A, 0x12, 0x00
};

static void display_column (uint8_t row_pattern, uint8_t current_column)
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

// If player1_choice win, return 0, else return 1. (Compare)
static int compare_choice(uint8_t player1_choice, uint8_t player2_choice)
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

//Display 'W' on Winner screen
static void winner_display(uint8_t current_choice, uint8_t current_column)
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


//Display 'L' on Loser screen
static void loser_display(uint8_t current_choice, uint8_t current_column)
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


//Display 'S' on both of the screen when they have the same choice
static void equal_display(uint8_t current_choice, uint8_t current_column)
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

static void job(uint8_t current_choice, uint8_t current_column)
{
    uint8_t partner_choice = 5;
    while(1) {
        pacer_wait();
        display_column (turnoff_screen[current_column], current_column);
        if (button_pressed_p()) {
            ir_uart_putc (current_choice + 1);
        }
        if (ir_uart_read_ready_p()) {
            partner_choice = ir_uart_getc() - 1;

            if (partner_choice != 0 && partner_choice != 1 && partner_choice != 2 && partner_choice != 3 && partner_choice != 4) {
                continue;
            } else if (current_choice == partner_choice) {
                equal_display(current_choice, current_column);
                partner_choice = 5;
                break;
            } else if (compare_choice(current_choice, partner_choice) == 0) {
                //current_choice win
                winner_display(current_choice, current_column);
                partner_choice = 5;
                break;
            } else {
                //partner_choice win
                loser_display(current_choice, current_column);
                partner_choice = 5;
                break;
            }
        }
        current_column++;
        if (current_column == 5) {
            current_column = 0;
        }
    }
}

int main (void)
{
    uint8_t current_choice = 0;
    uint8_t current_column = 0;

    system_init ();
    pacer_init (500);
    navswitch_init ();
    button_init ();
    ir_uart_init ();
    led_init ();

    /* TODO: Initialise LED matrix pins.  */
    for (int i = 0; i < 5; i++) {
        pio_config_set(cols[i], PIO_OUTPUT_HIGH);
    }
    for (int j = 0; j < 7; j++) {
        pio_config_set(rows[j], PIO_OUTPUT_HIGH);
    }

        while (1) {
            pacer_wait ();

            navswitch_update ();

            if (navswitch_push_event_p (NAVSWITCH_EAST)) {
                if (current_choice == 4) {
                    current_choice = 0;
                } else {
                    current_choice++;
                }
            }
            if (navswitch_push_event_p (NAVSWITCH_WEST)) {
                if (current_choice == 0) {
                    current_choice = 4;
                } else {
                    current_choice--;
                }
            }

            display_column (bitmap[current_choice * 5 + current_column], current_column);

            current_column++;

            if (current_column > (LEDMAT_COLS_NUM - 1)) {
                current_column = 0;
            }

            if (navswitch_push_event_p (NAVSWITCH_NORTH)) {
                job(current_choice, current_column);
            }
        }
}

