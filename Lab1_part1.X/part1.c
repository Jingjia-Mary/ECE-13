/*
 * This file is for Lab1 part 1
 * 
 * File:   part1.c
 * Author: Mary Xu (jxu228@ucsc.edu)
 * 
 * created on March 30, 2022, 11:56 Am
 */

// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <stdlib.h>

//Class specific libraries
#include "BOARD.h"


// User libraries

int main(void) {
    BOARD_Init();
    unsigned char vals[] = {rand(), rand(), rand(), rand(), rand()};
    int valsToBeSorted[] = {vals[0], vals[1], vals[2], vals[3], vals[4]};

    // Sort the array in place.
    //this will sort the value in order
    //| valsToBeSorted | Values1|Values2|Values3|Values4|Values5|
    //| ------         | ------ |------ |------ |------ |------ |
    //| 0              | 231    |  149  |  97   |  97   |   97  |
    //| 1              | 248    |  97   |  224  |  149  |  149  |
    //| 2              | 149    |  231  |  149  |  231  |  224  |
    //| 3              | 97     |  248  |  231  |  224  |  231  |
    //| 4              | 224    |  224  |  248  |  248  |  248  |

    int i, j;
    for (i = 0; i < 5; i++) {
        int aTemp = valsToBeSorted[i];
        for (j = i - 1; j >= 0; j--) {
            if (valsToBeSorted[j] <= aTemp)
                break;
            valsToBeSorted[j + 1] = valsToBeSorted[j];
        }
        valsToBeSorted [j + 1] = aTemp;
    }

    // Print out the array
    printf("[");
    for (i = 0; i < 4; i++) {
        printf("%d, ", valsToBeSorted[i]);
    }
    printf("%d]\n", valsToBeSorted[i]);

    /*
     * Returning from main() is bad form in embedded environments. So we
     * sit and spin.
     */
    while (1);
}