///*
// * *This file is about temperature Conversion
// * 
// * File:   part2.c
// * Author: Mary Xu (jxu228@ucsc.edu)
// *
// * Created on March 30, 2022, 2:05 PM
// */
//
//// **** Include libraries here ****
//// Standard libraries
//#include <stdio.h>
//
////Class specific libraries
//#include "BOARD.h"
//
//// User libraries
//#include <stdio.h>
//#include <stdlib.h>
//
//int main(void) {
//    BOARD_Init();
//    /***************************************************************************************************
//     * Your code goes in between this comment and the following one with asterisks.
//     **************************************************************************************************/
//    // Declare Variables
//    float x, y, z;
//    int lower, upper, step;
//
//    // Initialize Variables
//    lower = 0; //lower limit of temperature
//    upper = 300; //upper limit of temperature
//    step = 20; //step size
//    x = lower;
//    z = lower;
//
//    // Print out F to C table
//    printf("    F   C\n");
//    while (x <= upper) {
//        y = (5.0 / 9.0) * (x - 32.0);
//        printf("%7.1f %04.0f\n", (double) x, (double) y);
//        x = x + step;
//    }
//    
//    // Print out K to F table
//    printf("    K      F\n");
//    while (z <= upper) {
//        x = (9.0 / 5.0) * (z - 273.15) + (32);
//        printf("%03.3f %5f\n", (double) z, (double) x);
//        z = z + step;
//    }
//
//
//
//    /***************************************************************************************************
//     * Your code goes in between this comment and the preceding one with asterisks.
//     **************************************************************************************************/
//
//    // Returning from main() is bad form in embedded environments. So we sit and spin.
//    while (1);
//}

/* 
 * File:   part2.c
 * Author: Kai Juarez-Jimenez
 *
 * Created on April 5, 2023, 7:17 PM
 */
// **** Include libraries here ****
// Standard libraries
#include <stdio.h>

//Class specific libraries
#include "BOARD.h"


// User libraries
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    BOARD_Init();
    /***************************************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     **************************************************************************************************/
    //declaring variables
    float fahr, celsius;
    int lower, upper, step;
    // initialize variables
    lower = 0; // lower limit of temperature
    upper = 300; // upper limit
    step = 20; // step size
    fahr = lower;
    float kelv = lower;
    // printing out tables
    printf("    F     C\n");
    while (fahr <= upper) {
        celsius = (5.0 / 9.0)*(fahr - 32.0);
        printf("%7.1f %04.0f\n", (double) fahr, (double) celsius);
        fahr = fahr + step;
    }
    // printing out newline and headers
    printf("\n");
    printf("     K      F\n");
    // printing out kelvin/fahr tables
    while (kelv <= upper) {
        fahr = (9.0 / 5.0)*(kelv - 273.15) + 32;
        printf("%03.3f %7f\n", (double) kelv, (double) fahr); // new formula
        kelv = kelv + step;

    }



    /***************************************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks.
     **************************************************************************************************/

    // Returning from main() is bad form in embedded environments. So we sit and spin.
    BOARD_End();
    while (1);
}
