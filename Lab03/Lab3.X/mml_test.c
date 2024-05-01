/* 
 * This file contains the test of matrixmath
 * File:   mml_test.c
 * 
 * Author: Mary Xu
 *
 * Created on April 13, 2022, 7:11 PM
 */

// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//CMPE13 Support Library
#include "BOARD.h"




// User libraries:
#include "MatrixMath.h"


// Module-level variables:
float zero_matrix[3][3] = {
    {},
    {},
    {}};

int main() {
    BOARD_Init();

    printf("Beginning Mary's mml test harness, compiled on %s %s\n", __DATE__, __TIME__);

    printf("Demonstrating MatrixPrint():\n");
    MatrixPrint(zero_matrix);


    printf("Now testing MatrixTrace\n");
//    printf("Now testing MatrixTrace\n");
//    printf("Now testing MatrixTrace\n");
//    printf("Now testing MatrixTrace\n");
//    printf("Now testing MatrixTrace\n");
//    printf("Now testing MatrixTrace\n");
//    printf("Now testing MatrixTrace\n");
//    printf("Now testing MatrixTrace\n");
//    printf("Now testing MatrixTrace\n");
    
    //change the test name every time test a new calculation
    float TraceTest1[3][3] = {
        {-2, 5, 7},
        {4, 6, 1},
        {8, -9, 5}
    };
    /* if (MatrixEquals(equal_matrix, equal_matrix) == 1) {
        passed += 1;
    }
    */
    /*if (MatrixEquals(resultAdd, anstest) == 1) {
        passAdd++;
    }s
     */
    
    int TracePass = 0;
    printf("%f\n",MatrixTrace(TraceTest1));
    
    if(MatrixTrace(TraceTest1)-9 < FP_DELTA){
        printf("Trace Test 1 Passed!\n");
    };
    
    // Add more tests here!
    BOARD_End();
    while (1);
}

