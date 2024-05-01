/* 
 * File:   Game.c
 * Author: Mary Xu
 *
 * Created on May 17, 2022, 6:34 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Game.h"
#include "Player.h"

#define NUM_EXITS 4
#define ONE 1

struct currentRoom {
    char title [GAME_MAX_ROOM_TITLE_LENGTH];
    char desc [GAME_MAX_ROOM_DESC_LENGTH];
    uint8_t East;
    uint8_t South;
    uint8_t West;
    uint8_t North;
    uint16_t Exit[NUM_EXITS];
} currentRoom;

static char path[30];
static uint16_t point = FALSE;

int RoomData(uint8_t Room); //give a place to store data

int RoomData(uint8_t RoomNum) {
    FILE *Path;
    sprintf(path, "RoomFiles/Room%d.txt", RoomNum);
    Path = fopen(path, "rb");
    if (Path == NULL) { // set the error message
        return STANDARD_ERROR;
    }
    fseek(Path, 3, SEEK_SET);
    uint8_t TitleSize = fgetc(Path); //title size
    fread(currentRoom.title, TitleSize, ONE, Path); //read out
    currentRoom.title[TitleSize] = '\0'; //original title size

    while (1) {
        uint16_t Length = fgetc(Path); //length
        if (Length == 0) { // set the length length
            point = TRUE;
        }
        for (int i = 0; i < Length; i++) { //give the length to find
            if (FindInInventory(fgetc(Path)) == 0) {
                continue;
            }
        }

        uint8_t Contain = fgetc(Path); //if it already have length
        for (int i = 0; i < Contain; i++) {
            if (AddToInventory(fgetc(Path)) == 0) {
                return STANDARD_ERROR;
            }
        }

        currentRoom.East = fgetc(Path);
        currentRoom.South = fgetc(Path);
        currentRoom.West = fgetc(Path);
        currentRoom.North = fgetc(Path);
        if (point == TRUE) {//close it
            fclose(Path);
            break;
        }

        uint8_t Desc = fgetc(Path);
        fread(currentRoom.desc, Desc, ONE, Path);
        currentRoom.desc[Desc] = '\0';
    }
    return SUCCESS;
}

/**
 * These function transitions between rooms. Each call should return SUCCESS if the current room has
 * an exit in the correct direction and the new room was able to be loaded, and STANDARD_ERROR
 * otherwise.
 * @return SUCCESS if the room CAN be navigated to and changing the current room to that new room
 *         succeeded.
 */
int GameGoNorth(void) {
    if (currentRoom.North != 0x00 && RoomData(currentRoom.North) == SUCCESS) {
        return SUCCESS;
    }
    return STANDARD_ERROR;
}

/**
 * @see GameGoNorth
 */
int GameGoEast(void) {
    if (currentRoom.East != 0x00 && RoomData(currentRoom.East) == SUCCESS) {
        return SUCCESS;
    }
    return STANDARD_ERROR;
}

/**
 * @see GameGoNorth
 */
int GameGoSouth(void) {
    if (currentRoom.South != 0x00 && RoomData(currentRoom.South) == SUCCESS) {
        return SUCCESS;
    }
    return STANDARD_ERROR;
}

/**
 * @see GameGoNorth
 */
int GameGoWest(void) {
    if (currentRoom.West != 0x00 && RoomData(currentRoom.West) == SUCCESS) {
        return SUCCESS;
    }
    return STANDARD_ERROR;
}

/**
 * This function sets up anything that needs to happen at the start of the game. This is just
 * setting the current room to STARTING_ROOM and loading it. It should return SUCCESS if it succeeds
 * and STANDARD_ERROR if it doesn't.
 * @return SUCCESS or STANDARD_ERROR
 */
int GameInit(void) {
    uint8_t check = RoomData(STARTING_ROOM);
    if (check == SUCCESS) {
        return SUCCESS;
    }
    return STANDARD_ERROR;
}

/**
 * Copies the current room title as a NULL-terminated string into the provided character array.
 * Only a NULL-character is copied if there was an error so that the resultant output string
 * length is 0.
 * @param title A character array to copy the room title into. Should be GAME_MAX_ROOM_TITLE_LENGTH+1
 *             in length in order to allow for all possible titles to be copied into it.
 * @return The length of the string stored into `title`. Note that the actual number of chars
 *         written into `title` will be this value + 1 to account for the NULL terminating
 *         character.
 */
int GameGetCurrentRoomTitle(char *title) {
    strcpy(title, currentRoom.title);
    return strlen(title);
}

/**
 * GetCurrentRoomDescription() copies the description of the current room into the argument desc as
 * a C-style string with a NULL-terminating character. The room description is guaranteed to be less
 * -than-or-equal to GAME_MAX_ROOM_DESC_LENGTH characters, so the provided argument must be at least
 * GAME_MAX_ROOM_DESC_LENGTH + 1 characters long. Only a NULL-character is copied if there was an
 * error so that the resultant output string length is 0.
 * @param desc A character array to copy the room description into.
 * @return The length of the string stored into `desc`. Note that the actual number of chars
 *          written into `desc` will be this value + 1 to account for the NULL terminating
 *          character.
 */
int GameGetCurrentRoomDescription(char *desc) {
    strcpy(desc, currentRoom.desc);
    return strlen(desc);
}

/**
 * This function returns the exits from the current room in the lowest-four bits of the returned
 * uint8 in the order of NORTH, EAST, SOUTH, and WEST such that NORTH is in the MSB and WEST is in
 * the LSB. A bit value of 1 corresponds to there being a valid exit in that direction and a bit
 * value of 0 corresponds to there being no exit in that direction. The GameRoomExitFlags enum
 * provides bit-flags for checking the return value.
 *
 * @see GameRoomExitFlags
 *
 * @return a 4-bit bitfield signifying which exits are available to this room.
 */
uint8_t GameGetCurrentRoomExits(void) {
    uint8_t exit = '\0';
    if (currentRoom.East) {
        exit |= GAME_ROOM_EXIT_EAST_EXISTS;
    }
    if (currentRoom.South) {
        exit |= GAME_ROOM_EXIT_SOUTH_EXISTS;
    }
    if (currentRoom.West) {
        exit |= GAME_ROOM_EXIT_WEST_EXISTS;
    }
    if (currentRoom.North) {
        exit |= GAME_ROOM_EXIT_NORTH_EXISTS;
    }
    return exit;
}
