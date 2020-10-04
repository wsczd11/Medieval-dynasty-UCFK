#include "pio.h"
#include "button.h"

/** Return non-zero if button pressed.  */
int button_pressed_p (void)
{
    //return PIND & (1<<7);
    return pio_input_get(PD7_PIO);
    /* TODO.  */
}


/** Initialise button1.  */
void button_init (void)
{
    //DDRD &= ~(1<<7);
    pio_config_set(PD7_PIO, PIO_INPUT);
    /* TODO.  */
}
