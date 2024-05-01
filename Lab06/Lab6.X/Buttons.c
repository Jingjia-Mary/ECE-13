/* 
 * This file contains the implement functions for buttons.
 * File:   Buttons.c
 * 
 * Author: Mary Xu
 *
 * Created on May 2, 2022, 2:40 PM
 */

#include <stdio.h>
#include <stdlib.h>

#include "BOARD.h"
#include "Buttons.h"

static uint8_t debounce = 0;
static uint8_t x = 0; //state the static 

/**
 * This function initializes the proper pins such that the buttons 1-4 may be used by modifying
 * the necessary bits in TRISD/TRISF. Only the bits necessary to enable the 1-4 buttons are
 * modified, so that this library does not interfere with other libraries.
 */
void ButtonsInit(void) {
    TRISD |= 0x00E0;
    TRISF |= 0x0002; // set the bits
}

/**
 * ButtonsCheckEvents function checks the current button states and returns
 *  any events that have occured since its last
 * call.  This function should be called repeatedly in a Timer ISR, though it can
 * be called in main() during testing.
 *
 * In normal use, this function should only be used after ButtonsInit().
 * 
 * This function should assume that the buttons start in an off state with 
 * value 0. Therefore if no buttons are
 * pressed when ButtonsCheckEvents() is first called, 
 * BUTTONS_EVENT_NONE should be returned. 
 * 
 * @return  Each bit of the return value corresponds to one ButtonEvent flag,
 *          as described in Buttons.h.  If no events are detected, BUTTONS_EVENT_NONE is returned.
 * 
 * Note that more than one event can occur simultaneously, though this situation is rare.
 * To handle this, the output should be a bitwise OR of all applicable event flags. 
 * For example, if button 1 was released at the same time that button 2 was pressed,
 * this function should return (BUTTON_EVENT_1UP | BUTTON_EVENT_2DOWN) 
 * 
 */
uint8_t ButtonsCheckEvents(void) {
    uint16_t y;
    uint8_t button;
    button = BUTTON_EVENT_NONE; //declare all of them and I think there is a typo
    y = BUTTON_STATES(); //above that is is BUTTON not BUTTONS :)

    if (y == x) {
        return BUTTON_EVENT_NONE;
    } //compare them to return it
    if (debounce > 0) {
        debounce--; // set the debounce to make sure
        return button;
    }
    // there are 4 type of state and 2 situation
    if (x == BUTTON_STATE_1) {//first set
        if ((y & BUTTON_STATE_1) == 0) {//check the button
            button |= BUTTON_EVENT_1UP;
        }
    } else if (y & BUTTON_STATE_1) {
        if ((x & BUTTON_STATE_1) == 0) {
            button |= BUTTON_EVENT_1DOWN;
        }
    }
    if (x == BUTTON_STATE_2) {//same thing as the 1 state
        if ((y & BUTTON_STATE_2) == 0) {
            button |= BUTTON_EVENT_2UP;
        }
    } else if (y & BUTTON_STATE_2) {
        if ((x & BUTTON_STATE_2) == 0) {
            button |= BUTTON_EVENT_2DOWN;
        }
    }
    if (x == BUTTON_STATE_3) {//same thing just change state
        if ((y & BUTTON_STATE_3) == 0) {
            button |= BUTTON_EVENT_3UP;
        }
    } else if (y & BUTTON_STATE_3) {
        if ((x & BUTTON_STATE_3) == 0) {
            button |= BUTTON_EVENT_3DOWN;
        }
    }
    if (x == BUTTON_STATE_4) {//same thing
        if ((y & BUTTON_STATE_4) == 0) {
            button |= BUTTON_EVENT_4UP;
        }
    } else if (y & BUTTON_STATE_4) {
        if ((x & BUTTON_STATE_4) == 0) {
            button |= BUTTON_EVENT_4DOWN;
        }
    }

    debounce = BUTTONS_DEBOUNCE_PERIOD; //declare the debounce variable linked it
    x = y;
    return button;
}