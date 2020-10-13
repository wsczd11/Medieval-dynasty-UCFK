#include "system.h"


/* Comparing the 2 different choices from each of the player
 * If Player1 win, it will return 0 and if Player1 lose
 * it will return 1 */
int compare_choice(uint8_t Player_1, uint8_t Player_2)
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
