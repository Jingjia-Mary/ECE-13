/* 
 * This file contains the implement functions for bounce_buttons.
 * File:   bounce_buttons.c
 * 
 * Author: Mary Xu
 *
 * Edit on May3, 2022, 2:40 PM
 */
// **** Include libraries here ****
// Standard libraries
#include <stdio.h>

//CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>
#include <sys/attribs.h>

// User libraries
#include "Buttons.h"
#include "Leds_Lab06.h"
// **** Set macros and preprocessor directives ****

// **** Declare any datatypes here ****
static uint8_t button;
// **** Define global, module-level, or external variables here ****

// **** Declare function prototypes ****

int main(void) {
    BOARD_Init();

    // Configure Timer 1 using PBCLK as input. This default period will make the LEDs blink at a
    // pretty reasonable rate to start.
    T1CON = 0; // everything should be off
    T1CONbits.TCKPS = 1; // 1:8 prescaler
    PR1 = 0xFFFF; // interrupt at max interval
    T1CONbits.ON = 1; // turn the timer on

    // Set up the timer interrupt with a priority of 4.
    IFS0bits.T1IF = 0; //clear the interrupt flag before configuring
    IPC1bits.T1IP = 4; // priority of  4
    IPC1bits.T1IS = 0; // subpriority of 0 arbitrarily 
    IEC0bits.T1IE = 1; // turn the interrupt on

    /***************************************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     **************************************************************************************************/
    printf("Welcome to Mary's lab6 part5 (bounce_buttons).  Compiled on %s %s.\n", __TIME__, __DATE__);

    LEDS_INIT();
    ButtonsInit();
    while (1) {
        if (button != BUTTON_EVENT_NONE) {//all 8 of them needed switch
            if (SWITCH_STATES() & SWITCH_STATE_SW1) {
                if (button & BUTTON_EVENT_1UP) {
                    LEDS_SET(LEDS_GET() ^ 0x01 ^ 0x02);
                }
            } else if (!(SWITCH_STATES() & SWITCH_STATE_SW1)) { //reverse
                if (button & BUTTON_EVENT_1DOWN) {
                    LEDS_SET(LEDS_GET() ^0x01 ^ 0x02);
                }
            }

            if (SWITCH_STATES() & SWITCH_STATE_SW2) {
                if (button & BUTTON_EVENT_2UP) {
                    LEDS_SET(LEDS_GET() ^ 0x04 ^ 0x08);
                }
            } else if (!(SWITCH_STATES() & SWITCH_STATE_SW2)) {
                if (button & BUTTON_EVENT_2DOWN) {
                    LEDS_SET(LEDS_GET()^ 0x04 ^0x08);
                }
            }

            if (SWITCH_STATES() & SWITCH_STATE_SW3) {
                if (button & BUTTON_EVENT_3UP) {
                    LEDS_SET(LEDS_GET() ^ 0x10 ^ 0x20);
                }
            } else if (!(SWITCH_STATES() & SWITCH_STATE_SW3)) {
                if (button & BUTTON_EVENT_3DOWN) {
                    LEDS_SET(LEDS_GET() ^ 0x10 ^0x20);
                }
            }

            if (SWITCH_STATES() & SWITCH_STATE_SW4) {
                if (button & BUTTON_EVENT_4UP) {
                    LEDS_SET(LEDS_GET() ^ 0x40 ^ 0x80);
                }
            } else if (!(SWITCH_STATES() & SWITCH_STATE_SW4)) {
                if (button & BUTTON_EVENT_4DOWN) {
                    LEDS_SET(LEDS_GET()^ 0x40 ^0x80);
                }
            }

            button = BUTTON_EVENT_NONE;

        }

    }
    /***************************************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     **************************************************************************************************/

    while (1);
}

/**
 * This is the interrupt for the Timer1 peripheral. It should check for button events and stores them in a
 * module-level variable.
 * 
 * You should not modify this function for ButtonsTest.c or bounce_buttons.c!
 */
void __ISR(_TIMER_1_VECTOR, ipl4auto) Timer1Handler(void) {
    // Clear the interrupt flag.
    IFS0bits.T1IF = 0;

    /***************************************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     **************************************************************************************************/
    button = ButtonsCheckEvents();

    /***************************************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     **************************************************************************************************/
}

