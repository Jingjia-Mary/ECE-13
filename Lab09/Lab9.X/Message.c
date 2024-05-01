/* 
 * File:   Message.c
 * Author: maryx
 *
 * Created on June 1, 2022, 6:59 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "BOARD.h"
#include "Message.h"

#define ERROR 10

static char payload1[MESSAGE_MAX_PAYLOAD_LEN], checksum1[MESSAGE_CHECKSUM_LEN];
static int data = -1, record_pay = 0, record_che = 0;

int htoi(const char *string) {//call fuction
    int x = 0, y = 0;
    for (; (string[x] >= '0' && string[x] <= '9') || (string[x] >= 'A' && string[x] <= 'Z'); ++x) {
        if (string[x] > '9') {//columns and row only goes up to 9
            y = 16 * y + (10 + string[x] - 'A');
        } else {
            y = 16 * y + (string[x] - '0');
        }//set up a function that will help to project running and manipulate with
    }
    return y;
}

/**
 * Given a payload string, calculate its checksum
 * 
 * @param payload       //the string whose checksum we wish to calculate
 * @return   //The resulting 8-bit checksum 
 */
uint8_t Message_CalculateChecksum(const char* payload) {
    int l = 0, i = 0, j = strlen(payload);
    if (j < 5) {//to check and cal the number of payload
        return 0;
    } else {
        for (; i < j; i++) {
            l = l^payload[i];
        }
        return l;
    }
}

/**
 * ParseMessage() converts a message string into a BB_Event.  The payload and
 * checksum of a message are passed into ParseMessage(), and it modifies a
 * BB_Event struct in place to reflect the contents of the message.
 * 
 * @param payload       //the payload of a message
 * @param checksum      //the checksum (in string form) of  a message,
 *                          should be exactly 2 chars long, plus a null char
 * @param message_event //A BB_Event which will be modified by this function.
 *                      //If the message could be parsed successfully,
 *                          message_event's type will correspond to the message type and 
 *                          its parameters will match the message's data fields.
 *                      //If the message could not be parsed,
 *                          message_events type will be BB_EVENT_ERROR
 * 
 * @return STANDARD_ERROR if:
 *              the payload does not match the checksum
 *              the checksum string is not two characters long
 *              the message does not match any message template
 *          SUCCESS otherwise
 * 
 * Please note!  sscanf() has a couple compiler bugs that make it a very
 * unreliable tool for implementing this function. * 
 */
int Message_ParseMessage(const char* payload,
        const char* checksum_string, BB_Event * message_event) {
    char i[3], j[] = "";
    int a = 0;
    if (strlen(checksum_string) == 2) {
        if (htoi(checksum_string) == Message_CalculateChecksum(payload)) {
            for (a = 0; a < 3; a++) {
                i[a] = payload[a];
            }
            i[a] = '\0';
            //now right out all the possibility for res and out with number 0-10
            //for different ways the load will go in for
            //using the strcmp to call all the different case and strncpy for the number
            if (strcmp(i, "ACC") == 0) {
                message_event->type = BB_EVENT_ACC_RECEIVED;
                message_event->param0 = atoi(strncpy(j, payload + 4, 5));
                return SUCCESS;
            } else if (strcmp(i, "CHA") == 0) {
                message_event -> type = BB_EVENT_CHA_RECEIVED;
                message_event -> param0 = atoi(strncpy(j, payload + 4, 5));
                return SUCCESS;
            } else if (strcmp(i, "REV") == 0) {
                message_event->type = BB_EVENT_REV_RECEIVED;
                message_event->param0 = atoi(strncpy(j, payload + 4, 5));
                return SUCCESS;
            } else if (strcmp(i, "SHO") == 0) {// put in for sho for all the param
                message_event->type = BB_EVENT_SHO_RECEIVED;
                if (payload[5] == ',') {
                    message_event->param0 = atoi(strncpy(j, payload + 4, 1));
                    if (payload[7] == 0) {
                        message_event->param1 = atoi(strncpy(j, payload + 6, 1));
                    } else {
                        message_event->param1 = atoi(strncpy(j, payload + 6, 2));
                    }
                } else {
                    message_event->param0 = atoi(strncpy(j, payload + 4, 2));
                    if (payload[8] == 0) {
                        message_event->param1 = atoi(strncpy(j, payload + 7, 1));
                    } else {
                        message_event->param1 = atoi(strncpy(j, payload + 7, 2));
                    }
                }
                return SUCCESS;
            } else if (strcmp(i, "RES") == 0) {
                message_event->type = BB_EVENT_RES_RECEIVED;
                if (payload[5] == ',') {
                    message_event->param0 = atoi(strncpy(j, payload + 4, 1));
                    if (payload[7] == ',') {
                        message_event->param0 = atoi(strncpy(j, payload + 6, 1));
                        if (payload[9] == 0) {
                            message_event->param0 = atoi(strncpy(j, payload + 8, 1));
                        } else {
                            message_event->param0 = atoi(strncpy(j, payload + 8, 2));
                        }
                    } else {
                        message_event->param0 = atoi(strncpy(j, payload + 6, 2));
                        if (payload[10] == 0) {
                            message_event->param0 = atoi(strncpy(j, payload + 9, 1));
                        } else {
                            message_event->param0 = atoi(strncpy(j, payload + 9, 2));
                        }
                    }
                } else {
                    message_event->param0 = atoi(strncpy(j, payload + 4, 2));
                    if (payload[8] == ',') {
                        message_event->param0 = atoi(strncpy(j, payload + 7, 1));
                        if (payload[10] == 0) {
                            message_event->param0 = atoi(strncpy(j, payload + 9, 1));
                        } else {
                            message_event->param0 = atoi(strncpy(j, payload + 9, 2));
                        }
                    } else {
                        message_event->param0 = atoi(strncpy(j, payload + 7, 2));
                        if (payload[11] == 0) {
                            message_event->param0 = atoi(strncpy(j, payload + 10, 1));
                        } else {
                            message_event->param0 = atoi(strncpy(j, payload + 10, 2));
                        }
                    }
                }
                return SUCCESS;
            } else {//for it to occur with error
                message_event->type = BB_EVENT_ERROR;
                return STANDARD_ERROR;
            }
        } else {
            message_event->type = BB_EVENT_ERROR;
            return STANDARD_ERROR;
        }
    } else {
        message_event->type = BB_EVENT_ERROR;
        return STANDARD_ERROR;
    }
}

/**
 * Encodes the coordinate data for a guess into the string `message`. This string must be big
 * enough to contain all of the necessary data. The format is specified in PAYLOAD_TEMPLATE_COO,
 * which is then wrapped within the message as defined by MESSAGE_TEMPLATE. 
 * 
 * The final length of this
 * message is then returned. There is no failure mode for this function as there is no checking
 * for NULL pointers.
 * 
 * @param message            The character array used for storing the output. 
 *                              Must be long enough to store the entire string,
 *                              see MESSAGE_MAX_LEN.
 * @param message_to_encode  A message to encode
 * @return                   The length of the string stored into 'message_string'.
                             Return 0 if message type is MESSAGE_NONE.
 */
int Message_Encode(char *message_string, Message message_to_encode) {
    char x[MESSAGE_MAX_LEN];
    switch (message_to_encode.type) {//use the switch case to change the encode state
        case MESSAGE_ACC:
            sprintf(x, PAYLOAD_TEMPLATE_ACC, message_to_encode.param0);
            return sprintf(message_string, MESSAGE_TEMPLATE, x, Message_CalculateChecksum(x));
            break;
        case MESSAGE_CHA:
            sprintf(x, PAYLOAD_TEMPLATE_CHA, message_to_encode.param0);
            return sprintf(message_string, MESSAGE_TEMPLATE, x, Message_CalculateChecksum(x));
            break;
        case MESSAGE_REV:
            sprintf(x, PAYLOAD_TEMPLATE_REV, message_to_encode.param0);
            return sprintf(message_string, MESSAGE_TEMPLATE, x, Message_CalculateChecksum(x));
            break;
        case MESSAGE_SHO:
            sprintf(x, PAYLOAD_TEMPLATE_SHO, message_to_encode.param0, message_to_encode.param1);
            return sprintf(message_string, MESSAGE_TEMPLATE, x, Message_CalculateChecksum(x));
            break;
        case MESSAGE_RES:
            sprintf(x, PAYLOAD_TEMPLATE_RES, message_to_encode.param0,
                    message_to_encode.param1, message_to_encode.param2);
            return sprintf(message_string, MESSAGE_TEMPLATE, x, Message_CalculateChecksum(x));
            break;
        case MESSAGE_NONE:
            return 0;
    }
    return 0;
}

/**
 * Message_Decode reads one character at a time.  If it detects a full NMEA message,
 * it translates that message into a BB_Event struct, which can be passed to other 
 * services.
 * 
 * @param char_in - The next character in the NMEA0183 message to be decoded.
 * @param decoded_message - a pointer to a message struct, used to "return" a message
 *                          if char_in is the last character of a valid message, 
 *                              then decoded_message
 *                              should have the appropriate message type.
 *                          if char_in is the last character of an invalid message,
 *                              then decoded_message should have an ERROR type.
 *                          otherwise, it should have type NO_EVENT.
 * @return SUCCESS if no error was detected
 *         STANDARD_ERROR if an error was detected
 * 
 * note that ANY call to Message_Decode may modify decoded_message.
 */
int Message_Decode(unsigned char char_in, BB_Event * decoded_message_event) {
    //to decode all of the message break it up with it
    //encode both data and the character number in the game
    if ((data == 0) && (char_in != '*')) {
        if ((record_pay == MESSAGE_MAX_PAYLOAD_LEN) || (char_in == '$')) {
            data = data + ERROR;
            printf("Here");
            return STANDARD_ERROR;
        } else {
            payload1[record_pay] = char_in;
            return SUCCESS;
        }
        record_pay++;
    } else if ((data == 1) && (char_in != '\n')) {
        if ((record_che == MESSAGE_CHECKSUM_LEN) || ((char_in < '0' || char_in > '9') && (char_in < 'A' || char_in > 'Z'))) {
            data = data + ERROR;
            return STANDARD_ERROR;
        } else {
            checksum1[record_che] = char_in;
            record_che++;
            return SUCCESS;
        }
    }
    if (char_in == '$') {
        data++;
        return SUCCESS;
    } else if (char_in == '*') {
        data++;
        return SUCCESS;
    } else if ((char_in == '\n') && data < ERROR) {
        if (Message_ParseMessage(payload1, checksum1, decoded_message_event)) {
            return SUCCESS;
        } else {
            return STANDARD_ERROR;
        }
        memset(payload1, 0, MESSAGE_MAX_PAYLOAD_LEN);
        memset(checksum1, 0, MESSAGE_CHECKSUM_LEN);
    } else if ((char_in == '\n') && data >= ERROR) {
        memset(payload1, 0, MESSAGE_MAX_PAYLOAD_LEN);
        memset(checksum1, 0, MESSAGE_CHECKSUM_LEN);
        return STANDARD_ERROR;
    }
    return 0;
}