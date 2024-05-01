
// **** Include libraries here ****
// old bounce
// Standard libraries
#include <stdio.h>



//CMPE13 Support Library
#include "BOARD.h"
#include "Leds_Lab06.h"

// Microchip libraries
#include <xc.h>
#include <sys/attribs.h>

// **** Declare any datatypes here ****
//static uint8_t table;
//static uint8_t switchState = 0x0;
static struct Timer Timer1;
// **** Define global, module-level, or external variables here ****
#define the_LEFT 0x80
#define the_RIGHT 0x01

struct Timer {
    uint8_t event;
    int16_t timeRemaining;
};

// **** Declare function prototypes ****

enum table{
    LEFT,
    RIGHT
};

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
    printf("Welcome to CRUZID's lab6 part2 (bounce_switch).  Compiled on %s %s.\n", __TIME__, __DATE__);
    int state = -1; //state variable
    Timer1.event = FALSE;
    Timer1.timeRemaining = 1;
    uint8_t Pattern = 0x01;
    LEDS_INIT();

    while (1) {
        //poll timer events and react if any occur
        LEDS_SET(Pattern);
        if (Timer1.event) {
            if (Pattern == the_RIGHT) {
                state = LEFT;
            } else if (Pattern == the_LEFT) {
                state = RIGHT;
            }
            if (state == RIGHT) {
                Pattern = Pattern >> 1;
            } else {
                if (Pattern == the_RIGHT) {
                    Pattern = Pattern << 1;
                } else {
                    Pattern = Pattern << 1;
                }
            }
            Timer1.event = FALSE;
        }
    }


    /***************************************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     **************************************************************************************************/

}

/**
 * This is the interrupt for the Timer1 peripheral. It will trigger at the frequency of the peripheral
 * clock, divided by the timer 1 prescaler and the interrupt interval.
 * 
 * It should not be called, and should communicate with main code only by using module-level variables.
 */
void __ISR(_TIMER_1_VECTOR, ipl4auto) Timer1Handler(void) {
    // Clear the interrupt flag.
    IFS0bits.T1IF = 0;

    /***************************************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     **************************************************************************************************/
    Timer1.timeRemaining--;
    if (Timer1.timeRemaining <= 0) {
        Timer1.event = TRUE;
        Timer1.timeRemaining = SWITCH_STATES();
    }
    /***************************************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     **************************************************************************************************/

}