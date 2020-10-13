#include "system.h"
#include "pacer.h"
#include "tinygl.h"
#include "../fonts/font5x7_1.h"
#include "navswitch.h"


/* Display a start screen.  */
int words_display (char* words)
{
    system_init();
    navswitch_init ();

    tinygl_init (500);
    tinygl_font_set (&font5x7_1);
    tinygl_text_speed_set (20);
    tinygl_text_mode_set (TINYGL_TEXT_MODE_SCROLL);
    tinygl_text(words);

    pacer_init (500);

    while(1) {
        pacer_wait();
        navswitch_update ();
        tinygl_update();
        if (navswitch_push_event_p (NAVSWITCH_NORTH) || navswitch_push_event_p (NAVSWITCH_EAST)
        || navswitch_push_event_p (NAVSWITCH_WEST) || navswitch_push_event_p (NAVSWITCH_SOUTH)
        || navswitch_push_event_p (NAVSWITCH_PUSH)){
            return 0;
        }
    }
}
