/* 
 * File:   Agent.c
 * Author: maryx
 *
 * Created on May 29, 2022, 6:59 PM
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "BOARD.h"
#include "Oled.h"
#include "Agent.h"
#include "Message.h"
#include "BattleBoats.h"
#include "Field.h"
#include "FieldOled.h"
#include "Negotiation.h"

typedef enum {
    win,
    lost
} GameState;

static AgentState agentState;
static int turnCounter;
static Message message;
static NegotiationData a, b, c, d, e;
static Field gamerField, AIField;
static GuessData guessOut, guessRes;
static GameState gameState;
static FieldOledTurn Turn;
static char output[200] = "";
//I give up just writing message for this one, only message had drive me
//crazy so add field and negotiate in

NegotiationData generateHash(void);

/**
 * The Init() function for an Agent sets up everything necessary for an agent before the game
 * starts.  At a minimum, this requires:
 *   -setting the start state of the Agent SM.
 *   -setting turn counter to 0
 * If you are using any other persistent data in Agent SM, that should be reset as well.
 * 
 * It is not advised to call srand() inside of AgentInit.  
 *  */
void AgentInit(void) {
    agentState = AGENT_STATE_START;
    turnCounter = 0;
    Turn = FIELD_OLED_TURN_NONE;
    message.type = MESSAGE_NONE;
    a = 0; //set the original start number
    c = 0;
    b = 0;
}

/**
 * AgentRun evolves the Agent state machine in response to an event.
 * 
 * @param  The most recently detected event
 * @return Message, a Message struct to send to the opponent. 
 * 
 * If the returned Message struct is a valid message
 * (that is, not of type MESSAGE_NONE), then it will be
 * passed to the transmission module and sent via UART.
 * This is handled at the top level! AgentRun is ONLY responsible 
 * for generating the Message struct, not for encoding or sending it.
 */

Message AgentRun(BB_Event event) {
    switch (agentState) {//use switch gate to change the game step
        case AGENT_STATE_START:// start case
            if (event.type == BB_EVENT_RESET_BUTTON) {
                AgentInit();
                OledClear(0);
                OledDrawString("START");
                OledDrawString("\n\nReady to play a new game?");
                OledDrawString("\n\n\nPress BTN4 to Start");
                OledUpdate();
                message.type = MESSAGE_NONE;
                message.param0 = 0; //set everything to original
                message.param1 = 0;
                message.param2 = 0;
                turnCounter = 0;
                guessOut.row = 0;
                guessOut.col = 0;
                guessOut.result = 0;
                guessRes.row = 0;
                guessRes.col = 0;
                guessRes.result = 0;
                a = 0;
                b = 0;
                c = 0;
                agentState = AGENT_STATE_START;
                Turn = FIELD_OLED_TURN_NONE;
            } else if (event.type == BB_EVENT_START_BUTTON) {
                agentState = AGENT_STATE_CHALLENGING;
                srand((unsigned int) time(0)*6);
                a = rand();
                if (d == a || e == a) {
                    a = rand();
                }
                e = d;
                d = a;
                b = NegotiationHash(a);
                message.type = MESSAGE_CHA;
                message.param0 = b;
                message.param1 = 0;
                message.param2 = 0;
                FieldInit(&gamerField, &AIField); //boat position
                FieldAIPlaceAllBoats(&gamerField);
                OledClear(0);
                sprintf(output, "Challenging!\nb:%d\nA:%d", b, a);
                OledDrawString(output);
                OledUpdate();
            } else if (event.type == BB_EVENT_CHA_RECEIVED) {
                srand((unsigned int) time(0)*2);
                c = rand(); // for the order to be received
                if (d == c || e == c) {
                    c = rand();
                }
                e = d;
                d = c;
                message.param0 = c;
                message.param1 = 0;
                message.param2 = 0;
                b = event.param0;
                message.type = MESSAGE_ACC;
                FieldInit(&gamerField, &AIField); // original field
                FieldOledDrawScreen(&gamerField, &AIField, FIELD_OLED_TURN_MINE, turnCounter);
                FieldAIPlaceAllBoats(&gamerField); //PLACE BOAT
                OledClear(0);
                sprintf(output, "ACCEPTING\n"
                        "b:%d\n"
                        "B:%d", b, c);
                OledDrawString(output);
                OledUpdate();
                agentState = AGENT_STATE_ACCEPTING;
            } else if (event.type == BB_EVENT_ERROR) {
                OledClear(0); //give out for the error state where it will all conclude to 0
                sprintf(output, "ERROR IN STATE START");
                OledDrawString(output);
                OledUpdate();
                message.type = MESSAGE_NONE;
                message.param0 = 0;
                message.param1 = 0;
                message.param2 = 0;
            } else {
                message.type = MESSAGE_NONE;
                message.param0 = 0;
                message.param1 = 0;
                message.param2 = 0;
            }
            break;
        case AGENT_STATE_CHALLENGING://challenge case
            if (event.type == BB_EVENT_RESET_BUTTON) {
                OledClear(0);
                OledDrawString("START");
                OledDrawString("\n\nReady to play a new game?");
                OledDrawString("\n\n\nPress BTN4 to Start");
                OledUpdate();
                message.type = MESSAGE_NONE;
                message.param0 = 0; //start again
                message.param1 = 0;
                message.param2 = 0;
                turnCounter = 0;
                a = 0;
                c = 0;
                b = 0;
                agentState = AGENT_STATE_START;
            } else if (event.type == BB_EVENT_ACC_RECEIVED) {
                message.type = MESSAGE_REV;
                message.param0 = a;
                message.param1 = 0;
                message.param2 = 0;
                if (NegotiateCoinFlip(a, b) == HEADS) {//you are gussing hit or miss
                    agentState = AGENT_STATE_WAITING_TO_SEND;
                } else {
                    agentState = AGENT_STATE_DEFENDING;
                    FieldOledDrawScreen(&gamerField, &AIField, FIELD_OLED_TURN_MINE, turnCounter);
                }
            } else if (event.type == BB_EVENT_ERROR) {
                OledClear(0); //give out the error state for this case
                sprintf(output, "ERROR IN STATE CHALLENGING");
                OledDrawString(output);
                OledUpdate();
                message.type = MESSAGE_NONE;
                message.param0 = 0;
                message.param1 = 0;
                message.param2 = 0;
            } else {
                message.type = MESSAGE_NONE;
                message.param0 = 0;
                message.param1 = 0;
                message.param2 = 0;
            }
            break;
        case AGENT_STATE_ACCEPTING: // accept case
            if (event.type == BB_EVENT_RESET_BUTTON) {
                OledClear(0); //everything reset again
                OledDrawString("START");
                OledDrawString("\n\nReady to play a new game?");
                OledDrawString("\n\n\nPress BTN4 to Start");
                OledUpdate();
                message.type = MESSAGE_NONE;
                message.param0 = 0;
                message.param1 = 0;
                message.param2 = 0;
                turnCounter = 0;
                a = 0;
                c = 0;
                b = 0;
                agentState = AGENT_STATE_START;
            } else if (event.type == BB_EVENT_REV_RECEIVED) {
                agentState = AGENT_STATE_END_SCREEN;
                message.type = MESSAGE_NONE;
                a = event.param0;
                message.param0 = 0; //recevie the information but start with none
                message.param1 = 0;
                message.param2 = 0;
                if (NegotiationVerify(a, b) == TRUE) {// hit or miss
                    if (NegotiateCoinFlip(a, c) == HEADS) {
                        OledClear(0);
                        sprintf(output, "Heading to defense\n");
                        OledDrawString(output);
                        OledUpdate();
                        agentState = AGENT_STATE_DEFENDING;
                    } else {//message sho
                        message.type = MESSAGE_SHO;
                        guessOut = FieldAIDecideGuess(&AIField);
                        message.param0 = guessOut.row;
                        message.param1 = guessOut.col;
                        agentState = AGENT_STATE_ATTACKING;
                    }
                    FieldOledDrawScreen(&gamerField, &AIField, Turn, turnCounter);
                }
            } else if (event.type == BB_EVENT_ERROR) {
                OledClear(0);
                sprintf(output, "ERROR IN STATE ACCEPTING");
                OledDrawString(output);
                OledUpdate(); //wrong state
                message.type = MESSAGE_NONE;
                message.param0 = 0;
                message.param1 = 0;
                message.param2 = 0;
            } else {
                message.type = MESSAGE_NONE;
                message.param0 = 0;
                message.param1 = 0;
                message.param2 = 0;
            }
            break;
        case AGENT_STATE_ATTACKING:// attack case
            if (event.type == BB_EVENT_RES_RECEIVED) {
                guessRes.row = event.param0; // this is when gamer or AI attacks
                guessRes.col = event.param1; //so the recevie is the guess place
                guessRes.result = event.param2; //compare with the boat place
                message.type = MESSAGE_NONE;
                message.param0 = guessRes.row;
                message.param1 = guessRes.col;
                FieldUpdateKnowledge(&AIField, &guessRes);
                message.param2 = guessRes.result;
                uint8_t boatsAlive = FieldGetBoatStates(&AIField);
                if (boatsAlive == 0x00) {//the boat survived  from attacking
                    agentState = AGENT_STATE_END_SCREEN;
                } else {
                    agentState = AGENT_STATE_DEFENDING; //boat drown, so sad
                    FieldOledDrawScreen(&gamerField, &AIField, FIELD_OLED_TURN_MINE, turnCounter);
                }
            }
            if (event.type == BB_EVENT_ERROR) {
                OledClear(OLED_COLOR_BLACK); //recevied wrong information
                char errorString[100];
                sprintf(errorString, "error in attack! \n");
                OledDrawString(errorString);
                OledUpdate();
            }
            break;
        case AGENT_STATE_DEFENDING: //defending case
            if (event.type == BB_EVENT_SHO_RECEIVED) {
                guessOut.row = event.param0; //you are trying to help your boat from getting
                guessOut.col = event.param1; //hit, the information recived is
                message.type = MESSAGE_RES; //the guess compare with boat place
                message.param0 = guessOut.row;
                message.param1 = guessOut.col;
                FieldRegisterEnemyAttack(&gamerField, &guessOut);
                message.param2 = guessOut.result;
                uint8_t boatsAlive = FieldGetBoatStates(&gamerField);
                if (boatsAlive == 0x00) {//haha to bad
                    agentState = AGENT_STATE_END_SCREEN;
                } else {
                    agentState = AGENT_STATE_WAITING_TO_SEND;
                    FieldOledDrawScreen(&gamerField, &AIField, FIELD_OLED_TURN_MINE, turnCounter);
                }
            } else if (event.type == BB_EVENT_ERROR) {
                OledClear(OLED_COLOR_BLACK);
                char errorString[100];
                sprintf(errorString, "error in defending \n");
                OledDrawString(errorString);
                OledUpdate();
            }
            break;
        case AGENT_STATE_WAITING_TO_SEND: // send signal
            if (event.type == BB_EVENT_RESET_BUTTON) {
                OledClear(0);
                OledDrawString("START");
                OledDrawString("\n\nReady to play a new game?");
                OledDrawString("\n\n\nPress BTN4 to Start");
                OledUpdate(); // start again
                message.type = MESSAGE_NONE;
                message.param0 = 0;
                message.param1 = 0;
                message.param2 = 0;
                turnCounter = 0;
                a = 0;
                b = 0;
                c = 0;
                agentState = AGENT_STATE_START;
            } else if (event.type == BB_EVENT_MESSAGE_SENT) {
                turnCounter++; // message had been sent 
                message.type = MESSAGE_SHO; //message to sho
                guessRes = FieldAIDecideGuess(&AIField);
                message.param0 = guessOut.row;
                message.param1 = guessOut.col;
                agentState = AGENT_STATE_ATTACKING;
            } else if (event.type == BB_EVENT_ERROR) {
                OledClear(0); //an error occured
                sprintf(output, "ERROR IN WAITING TO SENT");
                OledDrawString(output);
                OledUpdate();
                message.type = MESSAGE_NONE;
                message.param0 = 0;
                message.param1 = 0;
                message.param2 = 0;
            } else {
                message.type = MESSAGE_NONE;
                message.param0 = 0;
                message.param1 = 0;
                message.param2 = 0;
            }
            break;
        case AGENT_STATE_END_SCREEN://end game
            if (gameState == win) {
                OledDrawString("You WON!");
            } else if (gameState == lost) {
                OledDrawString("You LOST!");
            } else {
                OledDrawString("ERROR\n");
            }
            OledUpdate();
            break;
    }
    return message;
}

/** * 
 * @return Returns the current state that AgentGetState is in.  
 * 
 * This function is very useful for testing AgentRun.
 */
AgentState AgentGetState(void) {
    return agentState;
}

/** * 
 * @param Force the agent into the state given by AgentState
 * 
 * This function is very useful for testing AgentRun.
 */
void AgentSetState(AgentState newState) {
    agentState = newState;
}