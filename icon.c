#include "system.h"
#include "pio.h"
#include "pacer.h"
#include "navswitch.h"


/**
 * @brief:Define PIO pins driving LED matrix rows.
*/
static const pio_t rows[] = {
    LEDMAT_ROW1_PIO, LEDMAT_ROW2_PIO, LEDMAT_ROW3_PIO,
    LEDMAT_ROW4_PIO, LEDMAT_ROW5_PIO, LEDMAT_ROW6_PIO,
    LEDMAT_ROW7_PIO
};


/**
 * @brief:Define PIO pins driving LED matrix columns.
*/
static const pio_t cols[] = {
    LEDMAT_COL1_PIO, LEDMAT_COL2_PIO, LEDMAT_COL3_PIO,
    LEDMAT_COL4_PIO, LEDMAT_COL5_PIO
};


/**
 * @brief:Different choices for the game
*/
static const uint8_t bitmap[] = {
    0x00, 0x10, 0x3E, 0x10, 0x00,      //Sword
    0x00, 0x1E, 0x3E, 0x1E, 0x00,      //Shield
    0x00, 0x28, 0x1C, 0x28, 0x00,      //Human
    0x04, 0x04, 0x0C, 0x04, 0x1C,      //Gun
    0x00, 0x38, 0x3E, 0x3A, 0x00       //Grenade
};


/**
 * @brief:Display the letter 'W'
*/
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
void ledm_init(void)
{
    for (int i = 0; i < 5; i++) {
        pio_config_set(cols[i], PIO_OUTPUT_HIGH);
    }
    for (int j = 0; j < 7; j++) {
        pio_config_set(rows[j], PIO_OUTPUT_HIGH);
    }
}


/* Display the LED */
void display_column(uint8_t row_pattern, uint8_t current_column)
{
    ledm_init();
    pio_output_low(cols[current_column]);
    for (uint8_t current_row = 0; current_row < 7; current_row++) {
        if ((row_pattern >> current_row) & 1) {
            pio_output_low(rows[current_row]);
        } else {
            pio_output_high(rows[current_row]);
        }
    }
}


void setting_saver(char key, uint8_t Player_1, uint8_t current_column)
{
    if (key == 'W') {
        display_column (winner_screen[current_column], current_column);
    } else if (key == 'L') {
        display_column (loser_screen[current_column], current_column);
    } else if (key == 'S') {
        display_column (equal_screen[current_column], current_column);
    } else {
        display_column(bitmap[Player_1 * 5 + current_column], current_column);
    }
    current_column++;

}
