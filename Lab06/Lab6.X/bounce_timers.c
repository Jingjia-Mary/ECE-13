/* 
 * This file contains the implement functions for bounce_timers.
 * File:   bounce_timers.c
 * 
 * Author: Mary Xu
 *
 * Edit on May3, 2022, 8:40 PM
 */
// **** Include libraries here ****
// old bounce
// Standard libraries
#include <stdio.h>
#include <GenericTypeDefs.h>

//CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>
#include <sys/attribs.h>

// User libraries
#include "Leds_Lab06.h"


// **** Declare any datatypes here ****
struct Timer{
    uint8_t event;
    int16_t timeRemaining;
};
// **** Define global, module-level, or external variables here ****
#define TIMER_A_REMAIN 76
#define TIMER_B_REMAIN 114
#define TIMER_C_REMAIN 190
// **** Declare function prototypes ****
static struct Timer TimerA, TimerB, TimerC;

int main(void)
{
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
    printf("Welcome to Mary's lab6 part1 (timers).  Compiled on %s %s.\n", __TIME__, __DATE__);
    
    LEDS_INIT();
    TimerA.event = FALSE;
    TimerB.event = FALSE;
    TimerC.event = FALSE;
    TimerA.timeRemaining = TIMER_A_REMAIN;
    TimerB.timeRemaining = TIMER_B_REMAIN;
    TimerC.timeRemaining = TIMER_C_REMAIN;
    
    while (1) {
        //poll timer A
            //react to timer A events
            //clear timer A event flag
        if (TimerA.event == TRUE){
            printf("A;");
            TimerA.event = FALSE;
            char led1 = 0x01;
            led1 = LEDS_GET()^led1;
            LEDS_SET(led1);
        }
        if (TimerB.event == TRUE){
            printf("B");
            TimerB.event = FALSE;
            char led2 = 0x02;
            led2 = LEDS_GET()^led2;
            LEDS_SET(led2);
        }
        if (TimerC.event == TRUE){
            printf("C;");
            TimerC.event = FALSE;
            char led3 = 0x04;
            led3 = LEDS_GET()^led3;
            LEDS_SET(led3);
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
void __ISR(_TIMER_1_VECTOR, ipl4auto) Timer1Handler(void)
{
    // Clear the interrupt flag.
    IFS0bits.T1IF = 0;
    /***************************************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     **************************************************************************************************/
    
    //update timerA
    //if timerA has counted down,
        //generate timerA event
        //reset timerA
    TimerA.timeRemaining--;//state it 
    TimerB.timeRemaining--;
    TimerC.timeRemaining--;
    if(TimerA.timeRemaining == 0){
        TimerA.timeRemaining = TIMER_A_REMAIN;
        TimerA.event = TRUE;
    }
    if(TimerB.timeRemaining == 0){
        TimerB.timeRemaining = TIMER_B_REMAIN;
        TimerB.event = TRUE;
    }
    if(TimerC.timeRemaining == 0){
        TimerC.timeRemaining = TIMER_C_REMAIN;
        TimerC.event = TRUE;
    }
    /***************************************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     **************************************************************************************************/

}