/* 
 * File:   Game.c
 * Author: Mary Xu
 *
 * Created on May 17, 2022, 6:34 PM
 */

// **** Include libraries here ****
// Standard libraries
#include <string.h>
#include <math.h>
#include <stdio.h>

//CSE013E Support Library
#include "UNIXBOARD.h"
#include "Game.h"
#include "Player.h"

// User libraries


// **** Set any macros or preprocessor directives here ****

// **** Declare any data types here ****

// **** Define any global or external variables here ****
#define NO_EXITS 0b0000
// **** Declare any function prototypes here ****

int main()
{
    /******************************** Your custom code goes below here ********************************/
    printf("Welcome to Mary's RPG Game!\n");
    
    char title[GAME_MAX_ROOM_DESC_LENGTH + 1];
    char descript[GAME_MAX_ROOM_DESC_LENGTH + 1];
    char input;
    int check;
    uint8_t Exit;
    
    int result = GameInit();
    if (result == STANDARD_ERROR){ //set the result
        FATAL_ERROR();
    }
    
    while (1){ //print out the result outline and the library
        GameGetCurrentRoomTitle(title);
        printf("\n%s\n", title);
        GameGetCurrentRoomDescription(descript);
        printf("\n%s\n", descript);
        Exit = GameGetCurrentRoomExits();
        printf("The possible exits are: ");
        if (Exit & GAME_ROOM_EXIT_NORTH_EXISTS) {
            printf("East");
        }
        if (Exit & GAME_ROOM_EXIT_EAST_EXISTS) {
            printf("South");
        }
        if (Exit & GAME_ROOM_EXIT_SOUTH_EXISTS) {
            printf("West");
        }
        if (Exit & GAME_ROOM_EXIT_WEST_EXISTS) {
            printf("North");
        }
        if (Exit == NO_EXITS) {
            printf("None. Press q to quit.");
        }
        
        while(1){
            printf("\nWhere would you like to go? (e for East, s for South, w for West, n for North. q to quit game): ");
            input = getchar(); //set input
            if (getchar() != '\n'){ // invalid number
                printf("Invalid input, please try again.\n");
                continue;
            }
            //all the enter character for the direction
            if (input == 'q'){ // the quit command
            exit(SUCCESS);
            }
            if (input == 'e'){
                check = GameGoEast();
                if (check == STANDARD_ERROR){
                    printf("There is no East exit!\n");
                }
            }else if (input == 's'){
                check = GameGoSouth();
                if(check == STANDARD_ERROR){
                    printf("There is no South exit!\n");
                }
            }else if (input == 'w') {
                check = GameGoWest();
                if (check == STANDARD_ERROR) {
                    printf("There is no West exit!\n");
                }
            }else if(input == 'n') {
                check = GameGoNorth();
                if (check == STANDARD_ERROR) {
                    printf("There is no North exit!\n");
                }
            }else if (input == 'q'){
                printf("\nYou have exit the game!:)\n");
            }
            else{
                printf("Invalid number, please try again.");
            }
            if (input == 'q' || check == SUCCESS){
                break;
            }
            check = 0;
        }
    }
    /**************************************************************************************************/
}

