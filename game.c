#include "system.h"
#include "pio.h"
#include "pacer.h"
#include "navswitch.h"
#include "ir_uart.h"
#include "button.h"




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


static const uint8_t bitmap[] = {
    0x00, 0x10, 0x3E, 0x10, 0x00,      //sword
    0x00, 0x1E, 0x3E, 0x1E, 0x00,      //shield
    0x00, 0x28, 0x1C, 0x28, 0x00,      //human
    0x38, 0x3E, 0x38, 0x3E, 0x38,    //body armour
    0x00, 0x38, 0x3E, 0x3A, 0x00      //grenade
};



static void display_column (uint8_t row_pattern, uint8_t current_column)
{
    /* TODO */
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


int main (void)
{
    uint8_t partner_choice;
    uint8_t current_choice = 0;
    uint8_t current_column = 0;

    system_init ();
    pacer_init (500);
    navswitch_init ();
    button_init ();
    ir_uart_init ();


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

        if (navswitch_push_event_p (NAVSWITCH_PUSH)) {
            while(1) {
                pacer_wait();
                button_update();
                if (button_push_event_p(0)) {
                    ir_uart_putc (current_choice);
                }
                if (ir_uart_read_ready_p()) {
                    partner_choice = ir_uart_getc();
                }

            }
        }
    }

static void compare (uint8_t current_choes,uint8_t paretner_choes) {



}


}
