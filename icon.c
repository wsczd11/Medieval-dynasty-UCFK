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


