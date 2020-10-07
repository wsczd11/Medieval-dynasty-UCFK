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


static const uint8_t bitmap[] = {
    0x00, 0x10, 0x3E, 0x10, 0x00,      //sword
    0x00, 0x1E, 0x3E, 0x1E, 0x00,      //shield
    0x00, 0x28, 0x1C, 0x28, 0x00,      //human
    0x04, 0x04, 0x0C, 0x04, 0x1C,      //gun
    0x00, 0x38, 0x3E, 0x3A, 0x00       //grenade
};

static const uint8_t close[] = {
    0x00, 0x00, 0x00, 0x00, 0x00,
};

static const uint8_t winner[] = {
    0x0E, 0x30, 0x0E, 0x30, 0x0E
};

static const uint8_t loser[] = {
    0x00, 0x3F, 0x20, 0x20, 0x00
};

static const uint8_t same[] = {
    0x00, 0x24, 0x2A, 0x12, 0x00
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

// if one_choes win, return 0, else return 1.
static int judgment(uint8_t one_choes, uint8_t another_choes)
{
    if (one_choes == 0) {
        if (another_choes == 2 || another_choes == 3) {
            return 0;
        } else {
            return 1;
        }
    } else if (one_choes == 1) {
        if (another_choes == 0 || another_choes == 4) {
            return 0;
        } else {
            return 1;
        }
    } else if (one_choes == 2) {
        if (another_choes == 1 || another_choes == 4) {
            return 0;
        } else {
            return 1;
        }
    } else if (one_choes == 3) {
        if (another_choes == 1 || another_choes == 2) {
            return 0;
        } else {
            return 1;
        }
    } else {
        if (another_choes == 0 || another_choes == 3) {
            return 0;
        } else {
            return 1;
        }
    }
}

static void winner_display(void)
{
    uint8_t current_column = 0;
    pacer_wait();
    while (1) {
        pacer_wait();
        navswitch_update ();
        display_column (winner[current_column], current_column);
        current_column++;
        if (current_column == 4) {
            current_column = 0;
        }
        if (navswitch_push_event_p (NAVSWITCH_PUSH)) {
            break;
        }
    }
}

static void loser_display(void)
{
    uint8_t current_column = 0;
    pacer_wait();
    while (1) {
        pacer_wait();
        navswitch_update ();
        display_column (loser[current_column], current_column);
        current_column++;
        if (current_column == 5) {
            current_column = 0;
        }
        if (navswitch_push_event_p (NAVSWITCH_PUSH)) {
            break;
        }
    }
}

static void same_display(void)
{
    uint8_t current_column = 0;
    pacer_wait();
    while (1) {
        pacer_wait();
        navswitch_update ();
        display_column (same[current_column], current_column);
        current_column++;
        if (current_column == 5) {
            current_column = 0;
        }
        if (navswitch_push_event_p (NAVSWITCH_PUSH)) {
            break;
        }
    }
}

static void job(uint8_t current_choes)
{
    uint8_t current_column = 0;
    uint8_t paretner_choes;
    while(1) {
        pacer_wait();
        display_column (close[current_column], current_column);
        if (button_pressed_p()) {
            ir_uart_putc (current_choes + 1);
        }
        if (ir_uart_read_ready_p()) {
            paretner_choes = ir_uart_getc() - 1;

            if (paretner_choes != 0 && paretner_choes != 1 && paretner_choes != 2 && paretner_choes != 3 && paretner_choes != 4 && paretner_choes == 5 && paretner_choes == 6 && paretner_choes == 7) {
                continue;
            } else if (paretner_choes == 5) {
                loser_display();
                break;
            } else if (paretner_choes == 6) {
                winner_display();
                break;
            } else if (paretner_choes == 7) {
                same_display();
                break;
            } else if (current_choes == paretner_choes) {
                ir_uart_putc (7 + 1);
                same_display();
                break;
            } else if (judgment(current_choes, paretner_choes) == 0) {
                //current_choes win
                ir_uart_putc (5 + 1);
                winner_display();
                break;
            } else {
                //paretner_choes win
                ir_uart_putc (6 + 1);
                loser_display();
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
    uint8_t current_choes = 0;
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
                if (current_choes == 4) {
                    current_choes = 0;
                } else {
                    current_choes++;
                }
            }
            if (navswitch_push_event_p (NAVSWITCH_WEST)) {
                if (current_choes == 0) {
                    current_choes = 4;
                } else {
                    current_choes--;
                }
            }

            display_column (bitmap[current_choes * 5 + current_column], current_column);

            current_column++;

            if (current_column > (LEDMAT_COLS_NUM - 1)) {
                current_column = 0;
            }

            if (navswitch_push_event_p (NAVSWITCH_NORTH)) {
                job(current_choes);
            }
        }
}

