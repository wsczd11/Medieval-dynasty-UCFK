#include "system.h"
#include "pio.h"
#include "pacer.h"
#include "navswitch.h"
#include "ir_uart.h"
#include "button.h"
#include "led.h"


/* Define PIO pins driving LED matrix rows.  */
static const pio_t rows[] = {
    LEDMAT_ROW1_PIO, LEDMAT_ROW2_PIO, LEDMAT_ROW3_PIO,
    LEDMAT_ROW4_PIO, LEDMAT_ROW5_PIO, LEDMAT_ROW6_PIO,
    LEDMAT_ROW7_PIO
};


/* Define PIO pins driving LED matrix columns.  */
static const pio_t cols[] = {
    LEDMAT_COL1_PIO, LEDMAT_COL2_PIO, LEDMAT_COL3_PIO,
    LEDMAT_COL4_PIO, LEDMAT_COL5_PIO
};


/* Different choices for the game */
static const uint8_t bitmap[] = {
    0x00, 0x10, 0x3E, 0x10, 0x00,      //Sword
    0x00, 0x1E, 0x3E, 0x1E, 0x00,      //Shield
    0x00, 0x28, 0x1C, 0x28, 0x00,      //Human
    0x04, 0x04, 0x0C, 0x04, 0x1C,      //Gun
    0x00, 0x38, 0x3E, 0x3A, 0x00       //Grenade
};


/* Display the letter 'W' */
static const uint8_t winner_screen[] = {
    0x0E, 0x30, 0x0E, 0x30, 0x0E
};


/* Display the letter 'L' */
static const uint8_t loser_screen[] = {
    0x00, 0x3E, 0x20, 0x20, 0x00
};


/* Display the letter 'S' */
static const uint8_t equal_screen[] = {
    0x00, 0x24, 0x2A, 0x12, 0x00
};


/* Initilise the LED Metrics */
static void inf(void)
{
    for (int i = 0; i < 5; i++) {
        pio_config_set(cols[i], PIO_OUTPUT_HIGH);
    }
    for (int j = 0; j < 7; j++) {
        pio_config_set(rows[j], PIO_OUTPUT_HIGH);
    }
}


/* Display the LED */
static void display_column(uint8_t row_pattern, uint8_t current_column)
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


/* Comparing the 2 different choices from each of the player
 * If Player1 win, it will return 0 and if Player1 lose
 * it will return 1 */
static int compare_choice(uint8_t Player_1, uint8_t Player_2)
{
    if (Player_1 == 0) {
        if (Player_2 == 2 || Player_2 == 3) {
            return 0;
        } else {
            return 1;
        }
    } else if (Player_1 == 1) {
        if (Player_2 == 0 || Player_2 == 4) {
            return 0;
        } else {
            return 1;
        }
    } else if (Player_1 == 2) {
        if (Player_2 == 1 || Player_2 == 4) {
            return 0;
        } else {
            return 1;
        }
    } else if (Player_1 == 3) {
        if (Player_2 == 1 || Player_2 == 2) {
            return 0;
        } else {
            return 1;
        }
    } else {
        if (Player_2 == 0 || Player_2 == 3) {
            return 0;
        } else {
            return 1;
        }
    }
}


/* Display 'W' on Winner screen if the player wins */
static void winner_display(uint8_t Player_1, uint8_t current_column)
{
    while (1) {
        pacer_wait();
        navswitch_update();
        display_column(winner_screen[current_column], current_column);
        current_column++;
        if (current_column == 5) {
            current_column = 0;
        }
        if (button_pressed_p()) {
            ir_uart_putc (Player_1 + 1);
        }
        if (navswitch_push_event_p(NAVSWITCH_PUSH)) {
            break;
        }
    }
}


/* Display 'L' on Loser screen if the player loss */
static void loser_display(uint8_t Player_1, uint8_t current_column)
{
    while (1) {
        pacer_wait();
        navswitch_update();
        display_column(loser_screen[current_column], current_column);
        current_column++;
        if (current_column == 5) {
            current_column = 0;
        }
        if (button_pressed_p()) {
            ir_uart_putc (Player_1 + 1);
        }
        if (navswitch_push_event_p(NAVSWITCH_PUSH)) {
            break;
        }
    }
}


/* Display 'S' on both of the screen when both of the player
 * have the same choice */
static void equal_display(uint8_t Player_1, uint8_t current_column)
{
    while (1) {
        pacer_wait();
        navswitch_update();
        display_column (equal_screen[current_column], current_column);
        current_column++;
        if (current_column == 5) {
            current_column = 0;
        }
        if (button_pressed_p()) {
            ir_uart_putc (Player_1 + 1);
        }
        if (navswitch_push_event_p(NAVSWITCH_PUSH)) {
            break;
        }
    }
}


/* Compare both of the input that both of the player have chose and then
 * determine which of the player wins/lose or tie by the if's statement and
 * determine the display winner_display, loser_display or equal_display by the
 * player's choice */
static int determine_outcome(uint8_t Player_1, uint8_t current_column)
{
    uint8_t Player_2;
    inf();
    while(1) {
        pacer_wait();

        if (button_pressed_p()) {
            ir_uart_putc (Player_1 + 1); //Encode
        }
        if (ir_uart_read_ready_p()) {
            Player_2 = ir_uart_getc() - 1; //Decode
            if (Player_2 != 0 && Player_2 != 1 && Player_2 != 2 && Player_2 != 3 && Player_2 != 4) { //Check index
                continue;
            } else if (Player_1 == Player_2) {
                equal_display(Player_1, current_column);
                return 0;
            } else if (compare_choice(Player_1, Player_2) == 0) {
                winner_display(Player_1, current_column); //Player_1 win
                return 1;
            } else {
                loser_display(Player_1, current_column); //Player_2 win
                return 0;
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
    uint8_t Player_1 = 0;
    uint8_t current_column = 0;
    uint16_t number = 0;

    system_init ();
    pacer_init (500);
    navswitch_init ();
    button_init ();
    ir_uart_init ();
    led_init ();
    inf();

    while (1) {

        pacer_wait ();
        navswitch_update ();

        if (navswitch_push_event_p (NAVSWITCH_EAST)) { //Selection
            if (Player_1 == 4) {
                Player_1 = 0;
            } else {
                Player_1++;
            }
        }
        if (navswitch_push_event_p (NAVSWITCH_WEST)) { //Selection
            if (Player_1 == 0) {
                Player_1 = 4;
            } else {
                Player_1--;
            }
        }

        display_column(bitmap[Player_1 * 5 + current_column], current_column);
        current_column++;

        if (current_column == 5) {
            current_column = 0;
        }
        if (navswitch_push_event_p (NAVSWITCH_NORTH)) { //Comparision
            number += determine_outcome(Player_1, current_column);
        }
        if (number == 3){
            ir_uart_putc ('A');
            inf();
            break;
        }
        if (ir_uart_read_ready_p()) {
            if (ir_uart_getc() == 'A'){ //Turn LED off for player who loss
                led_off();
                inf();
                break;
            }
        }

    }
}

