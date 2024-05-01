/* 
 * This file contains the implement functions for the lab7.
 * File:   Lab07_main.c
 * 
 * Author: Mary Xu
 *
 * Created on May 10, 2022, 2:40 PM
 */
//**** Include libraries here ****
//Standard libraries
#include <stdio.h>

//CSE13E Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>
#include <sys/attribs.h>

#include "BOARD.h"
#include <string.h>
#include "Leds.h"
#include "Adc.h"
#include "Ascii.h"
#include "Buttons.h"
#include "Oled.h"
#include "OledDriver.h"

#define SIZE 90
// **** Set any macros or preprocessor directives here ****
// Set a macro for resetting the timer, makes the code a little clearer.
#define TIMER_2HZ_RESET() (TMR1 = 0)

#define LongPress 5
#define DEFAULT_TEMP 350
#define OVEN_TOP_ON1 "\x01\x01\x01\x01\x01"
#define OVEN_TOP_OFF1 "\x02\x02\x02\x02\x02"
#define OVEN_BOTTOM_ON1 "\x03\x03\x03\x03\x03"
#define OVEN_BOTTOM_OFF1 "\x04\x04\x04\x04\x04"
#define ALL_LEDS_ON 0xFF
#define LEDS_OFF 0x00
#define SIXTY 60

// **** Set any local typedefs here ****

typedef enum {
    SETUP, SELECTOR_CHANGE_PENDING, COOKING, RESET_PENDING, ec
} OvenState;

typedef enum {
    BAKE, TOAST, BROIL
} CookingState;

typedef enum {
    time, temp
} Selector;

typedef enum {
    EVENT_NONE, ADC_CHANGED, BUTTON3FOWN, BUTTON3UP, BUTTON4DOWN, BUTTON4UP, TIMER_TICK_EVENT
} Event;

typedef struct {
    OvenState state;
    CookingState mode;
    //   SelectorState selector;
    Selector menu;
    uint16_t temperature;
    uint16_t setTime;
    uint16_t cookTimeStart;
    uint16_t cookTimeLeft;
    uint16_t buttonPressTime;
    uint16_t CookingTime;
    uint16_t total;
    //CookMode mode;

} OvenData;

// **** Declare any datatypes here ****

// **** Define any module-level, global, or external variables here ****
OvenData oven = {.state = SETUP, .cookTimeLeft = 1, .cookTimeStart = 0,
    .temperature = DEFAULT_TEMP, .buttonPressTime = 0, .mode = BAKE};

static char dataline1[100], dataline2[100], dataline3[100], dataline4[100];
static char output[120];
//static char *mode_string[3] = {"Bake", "Toast", "Broil"};
static uint8_t cookTimeMins;
static uint8_t cookTimeSecs;
static OvenData ovenSM;
static OvenData ovendata;
static uint16_t FRT = 0;
static uint16_t TIMER_TICK = FALSE;
static uint16_t ButtonEvent = BUTTON_EVENT_NONE;
static uint16_t adcChange = FALSE;
static uint8_t done = FALSE;
static int light;

// **** Put any helper functions here ****

void updateCookTime(void) {
    cookTimeMins = ((uint16_t) oven.cookTimeLeft / SIXTY);
    cookTimeSecs = oven.cookTimeLeft % SIXTY;
}

/*This function will update your OLED to reflect the state .*/
void updateOvenOLED(OvenData ovenData) {
    //update OLED here
    sprintf(dataline1, "%s%s%s%s%s", OVEN_TOP_ON, OVEN_TOP_ON, OVEN_TOP_ON, OVEN_TOP_ON,
            OVEN_TOP_ON);
    sprintf(dataline2, "%s%s%s%s%s", OVEN_TOP_OFF, OVEN_TOP_OFF, OVEN_TOP_OFF, OVEN_TOP_OFF,
            OVEN_TOP_OFF);
    sprintf(dataline3, "%s%s%s%s%s", OVEN_BOTTOM_ON, OVEN_BOTTOM_ON, OVEN_BOTTOM_ON, OVEN_BOTTOM_ON,
            OVEN_BOTTOM_ON);
    sprintf(dataline4, "%s%s%s%s%s", OVEN_BOTTOM_OFF, OVEN_BOTTOM_OFF, OVEN_BOTTOM_OFF, OVEN_BOTTOM_OFF,
            OVEN_BOTTOM_OFF);
    // switch statement to determine what to print. The logic works as follows:
    switch (ovenData.mode) {
        case BAKE:
            if (!(ovenSM.state == COOKING || ovenSM.state == RESET_PENDING)) {
                if (ovenData.menu == time) {
                    sprintf(output, "|%s| Mode: Bake\n"
                            "|     | >Time: %d:%02d\n"
                            "|-----|  Temp: %d%sF\n"
                            "|%s|", dataline2, ovenData.total / 60, ovenData.total % 60,
                            ovenData.temperature, DEGREE_SYMBOL, dataline4);
                } else if(ovenData.menu == temp){
                    sprintf(output, "|%s| Mode: Bake\n"
                            "|     |  Time: %d:%02d\n"
                            "|-----| >Temp: %d%sF\n"
                            "|%s|", dataline2, ovenData.total / 60, ovenData.total % 60,
                            ovenData.temperature, DEGREE_SYMBOL, dataline4);
                }
            } else if (ovenSM.state == COOKING) {
                sprintf(output, "|%s| Mode: Bake\n"
                        "|     |  Time: %d:%02d\n"
                        "|-----|  Temp: %d%sF\n"
                        "|%s|", dataline1, ovenData.total / 60, ovenData.total % 60,
                        ovenData.temperature, DEGREE_SYMBOL, dataline3);
            }
            break;
        case TOAST:
            if (!(ovenSM.state == COOKING || ovenSM.state == RESET_PENDING)) {
                sprintf(output, "|%s| Mode: Toast\n"
                        "|     |  Time: %d:%02d\n"
                        "|-----|\n"
                        "|%s|", dataline2, ovenData.total / 60, ovenData.total % 60,
                        dataline4);
            } else {
                sprintf(output, "|%s| Mode: Toast\n|     |  Time: %d:%02d\n|-----|"
                        "\n|%s|", dataline1, ovenData.total / 60, ovenData.total % 60,
                        dataline3);
            }
            break;
        case BROIL:
            if (!(ovenSM.state == COOKING || ovenSM.state == RESET_PENDING)) {
                sprintf(output, "|%s| Mode: Broil\n"
                        "|     |  Time: %d:%02d\n"
                        "|-----|  Temp: 500%sF\n"
                        "|%s|", dataline2, ovenData.total / 60, ovenData.total % 60,
                        DEGREE_SYMBOL, dataline4);
            } else {
                sprintf(output, "|%s| Mode: Broil\n"
                        "|     |  Time: %d:%02d\n"
                        "|-----|  Temp: 500%sF\n"
                        "|%s|", dataline2, ovenData.total / 60, ovenData.total % 60,
                        DEGREE_SYMBOL, dataline3);
            }
            break;
    }
    OledClear(OLED_COLOR_BLACK);
    OledDrawString(output);
    if (ovenSM.state == ec) {
        if (done) {
            OledSetDisplayNormal();
        } else {
            OledSetDisplayInverted();
        }
    }
    OledUpdate();
}

/*This function will execute your state machine.  
 * It should ONLY run if an event flag has been set.*/
void runOvenSM(void) {
    switch (ovenSM.state) {
        case SETUP:
            if (ButtonEvent & BUTTON_EVENT_3DOWN) {
                ovendata.buttonPressTime = FRT;
                ovenSM.state = SELECTOR_CHANGE_PENDING;
            }
            if (ButtonEvent & BUTTON_EVENT_4DOWN) {
                ovendata.cookTimeLeft = ovendata.total;
                updateOvenOLED(ovendata);
                LEDS_SET(ALL_LEDS_ON);
                ovenSM.state = COOKING;
            }
            if (adcChange) {
                if (ovendata.menu == temp) {
                    ovendata.temperature = (AdcRead() >> 2);
                    ovendata.temperature += 300;
                } else if (ovendata.menu == time) {
                    ovendata.total = (AdcRead() >> 2) + 1;
                    ovendata.cookTimeStart = ovendata.total;
                }
                updateOvenOLED(ovendata);
            }
            break;

        case SELECTOR_CHANGE_PENDING:
            if (ButtonEvent & BUTTON_EVENT_3UP) {
                uint16_t elapsed_Time = FRT - ovendata.buttonPressTime;
                if (elapsed_Time < LongPress) {
                    if (ovendata.mode == BAKE) {
                        ovendata.mode = TOAST;
                        ovendata.menu = time;
                    } else if (ovendata.mode == TOAST) {
                        ovendata.mode = BROIL;
                        ovendata.menu = time;
                    } else if (ovendata.mode == BROIL) {
                        ovendata.mode = BAKE;
                    }
                    updateOvenOLED(ovendata);
                    ovenSM.state = SETUP;
                } else {
                    if (ovendata.menu == time) {
                        ovendata.menu = temp;
                    } else if (ovendata.menu == time) {
                        ovendata.menu = time;
                    }
                    updateOvenOLED(ovendata);
                    ovenSM.state = SETUP;
                }
            }
            break;

        case COOKING:
            if (ButtonEvent & BUTTON_EVENT_4DOWN) {
                ovenSM.state = RESET_PENDING;
                ovenSM.buttonPressTime = FRT;
            }
            if (TIMER_TICK) {
                if ((FRT - ovendata.cookTimeStart) % 5 == 0) {
                    ovendata.cookTimeLeft = ovendata.total;
                    ovendata.total--;
                    updateOvenOLED(ovendata);
                    Led_update(ovendata);
                    break;
                }
                if (ovendata.total == 0) {
                    ovendata.total = ovendata.cookTimeStart;
                    ovenSM.state = ec;
                    updateOvenOLED(ovendata);
                    break;
                }
            }
            break;

        case RESET_PENDING:
            if ((ButtonEvent & BUTTON_EVENT_4UP) && ((FRT - ovenSM.buttonPressTime) / 5) < LongPress) {
                ovenSM.state = COOKING;
            }
            if (TIMER_TICK){
                if (((FRT - ovenSM.buttonPressTime) / 5) >= LongPress) {
                    ovendata.total = ovendata.cookTimeStart;
                    ovenSM.state = SETUP;
                    ovendata.total = ovendata.cookTimeStart;
                    updateOvenOLED(ovendata);
                    LEDS_SET(LEDS_OFF);
                    break;
                } else {
                    if (((FRT - ovenSM.cookTimeStart) % 5) == 0) {
                        ovendata.total--;
                        updateOvenOLED(ovendata);
                        Led_update(ovendata);
                    }
                    if (ovendata.cookTimeLeft == 0) {
                        ovendata.cookTimeLeft = ovendata.total;
                        ovenSM.state = SETUP;
                        updateOvenOLED(ovendata);
                        break;
                    }
                }
            }
            break;

        case ec:
            if (TIMER_TICK) {
                if (done) {
                    done = FALSE;
                } else {
                    done = TRUE;
                }
                updateOvenOLED(ovendata);
            }
            if (ButtonEvent & BUTTON_EVENT_4UP) {
                done = TRUE;
                updateOvenOLED(ovendata);
                ovenSM.state = SETUP;
                updateOvenOLED(ovendata);
            }
            break;
    }
}

void Led_update(OvenData ovendata);
void Led_update(OvenData ovendata) {
    light = ((ovendata.cookTimeStart * 1) / 8);
    if (light == ovendata.total) {
        LEDS_SET(LEDS_GET() << 1);
    }
    light = ((ovendata.cookTimeStart * 2) / 8);
    if (light == ovendata.total) {
        LEDS_SET(LEDS_GET() << 1);
    }
    light = ((ovendata.cookTimeStart * 3) / 8);
    if (light == ovendata.total) {
        LEDS_SET(LEDS_GET() << 1);
    }
    light = ((ovendata.cookTimeStart * 4) / 8);
    if (light == ovendata.total) {
        LEDS_SET(LEDS_GET() << 1);
    }
    light = ((ovendata.cookTimeStart * 5) / 8);
    if (light == ovendata.total) {
        LEDS_SET(LEDS_GET() << 1);
    }
    light = ((ovendata.cookTimeStart * 6) / 8);
    if (light == ovendata.total) {
        LEDS_SET(LEDS_GET() << 1);
    }
    light = ((ovendata.cookTimeStart * 7) / 8);
    if (light == ovendata.total) {
        LEDS_SET(LEDS_GET() << 1);
    }
    light = 0;
    if (light == ovendata.total) {
        LEDS_SET(LEDS_GET() << 1);
    }
}

int main() {
    ovendata.mode = BAKE;
    ovenSM.state = SETUP;
    BOARD_Init();
    // <editor-fold defaultstate="collapsed" desc="TIMER SETUP">

    // Configure Timer 2 using PBCLK as input. We configure it using a 1:16 prescalar, so each timer
    // tick is actually at F_PB / 16 Hz, so setting PR2 to F_PB / 16 / 100 yields a .01s timer.

    T2CON = 0; // everything should be off
    T2CONbits.TCKPS = 0b100; // 1:16 prescaler
    PR2 = BOARD_GetPBClock() / 16 / 100; // interrupt at .5s intervals
    T2CONbits.ON = 1; // turn the timer on

    // Set up the timer interrupt with a priority of 4.
    IFS0bits.T2IF = 0; //clear the interrupt flag before configuring
    IPC2bits.T2IP = 4; // priority of  4
    IPC2bits.T2IS = 0; // subpriority of 0 arbitrarily 
    IEC0bits.T2IE = 1; // turn the interrupt on

    // Configure Timer 3 using PBCLK as input. We configure it using a 1:256 prescaler, so each timer
    // tick is actually at F_PB / 256 Hz, so setting PR3 to F_PB / 256 / 5 yields a .2s timer.

    T3CON = 0; // everything should be off
    T3CONbits.TCKPS = 0b111; // 1:256 prescaler
    PR3 = BOARD_GetPBClock() / 256 / 5; // interrupt at .5s intervals
    T3CONbits.ON = 1; // turn the timer on

    // Set up the timer interrupt with a priority of 4.
    IFS0bits.T3IF = 0; //clear the interrupt flag before configuring
    IPC3bits.T3IP = 4; // priority of  4
    IPC3bits.T3IS = 0; // subpriority of 0 arbitrarily 
    IEC0bits.T3IE = 1; // turn the interrupt on;

    // </editor-fold>
    OledInit();
    LEDS_INIT();
    ButtonsInit();
    AdcInit();
    ovendata.temperature = 350;
    //oven.mode =
    updateOvenOLED(ovendata);

    printf("Welcome to mary's Lab07 (Toaster Oven).  Compiled on %s %s.", __TIME__, __DATE__);
    //initialize state machine (and anything else you need to init) here
    ovendata.temperature = DEFAULT_TEMP;
    while (1) {
        // Add main loop code here:
        // check for events
        // on event, run runOvenSM()
        // clear event flags
        if (ButtonEvent != BUTTON_EVENT_NONE || adcChange || TIMER_TICK) {
            runOvenSM();
            ButtonEvent = BUTTON_EVENT_NONE;
            adcChange = FALSE;
            TIMER_TICK = FALSE;
        }
    }
}

/*The 5hz timer is used to update the free-running timer and to generate TIMER_TICK events*/
void __ISR(_TIMER_3_VECTOR, ipl4auto) TimerInterrupt5Hz(void) {
    // Clear the interrupt flag.
    IFS0CLR = 1 << 12;
    TIMER_TICK = TRUE;
    FRT++;
}
//add event-checking code here

/*The 100hz timer is used to check for button and ADC events*/
void __ISR(_TIMER_2_VECTOR, ipl4auto) TimerInterrupt100Hz(void) {
    // Clear the interrupt flag.
    IFS0CLR = 1 << 8;
    //add event-checking code here
    adcChange = AdcChanged();
    ButtonEvent = ButtonsCheckEvents();
}
