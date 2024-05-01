///* 
// * This file contains the calculation of varies method
// * 
// * File:   Calculator.c
// * Author: Mary Xu (jxu228@ucsc.edu)
// *
// * Created on April 3, 2022, 12:38 PM
// */
//
//// Standard libraries
//#include <stdio.h>
//#include <math.h>
//
////User libraries:
//#include "BOARD.h"
//
//#ifdef PIC32
//// Microchip libraries
//#include <xc.h>
//#endif
//
//// **** Declare function prototypes ****
//double Add(double operand1, double operand2);
//double Multiply(double operand1, double operand2);
//double AbsoluteValue(double operand);
//double FahrenheitToCelsius(double operand);
//double Tangent(double operand);
////add more prototypes here
//double Subtract(double operand1, double operand2);
//double Divide(double operand1, double operand2);
//double Average(double operand1, double operand2);
//double CelsiusToFahrenheit(double operand);
//double Round(double operand);
//
//#define opt 1
//
//void CalculatorRun(void) {
//    printf("\n\nWelcome to Mary's calculator program! Compiled at %s %s\n", __DATE__, __TIME__);
//
//    // Your code here
//    while (opt) {
//        char oper;
//        //operand and clean up memory
//        float operand, operand1, operand2;
//        double value;
//
//        printf("\n\nEnter a mathematical operation to perform (*,/,+,-,m,a,c,f,t,q,r):");
//        scanf("%c", &oper);
//        //the beginning of choosing the prototype to calculate
//
//        if (oper == '*') {
//            printf("Enter the first operand:");
//            scanf("%f", &operand1);
//            while (getchar() != '\n');
//            printf("Enter the second operand:");
//            scanf("%f", &operand2);
//            //while (getchar() != '\n');
//            printf("Result of (%f %c %f):", operand1, oper, operand2);
//            value = Multiply(operand1, operand2);
//            printf("%f", value);
//            //multiplication operator print
//        } else if (oper == '/') {
//            printf("Enter the first operand:");
//            scanf("%f", &operand1);
//            while (getchar() != '\n');
//            printf("Enter the second operand:");
//            scanf("%f", &operand2);
//            //            while (getchar() != '\n');
//            printf("Result of (%f %c %f):", operand1, oper, operand2);
//            value = Divide(operand1, operand2);
//            if (value == HUGE_VAL) {
//                printf("Divide by zero error!");
//            } else {
//                printf("%f", value);
//            }
//            //division operator
//        } else if (oper == '+') {
//            printf("Enter the first operand:");
//            scanf("%f", &operand1);
//            while (getchar() != '\n');
//            printf("Enter the second operand:");
//            scanf("%f", &operand2);
//            //            while (getchar() != '\n');
//            printf("Result of (%f %c %f):", operand1, oper, operand2);
//            value = Add(operand1, operand2);
//            printf("%f", value);
//            //addition operator
//        } else if (oper == '-') {
//            printf("Enter the first operand:");
//            scanf("%f", &operand1);
//            while (getchar() != '\n');
//            printf("Enter the second operand:");
//            scanf("%f", &operand2);
//            //            while (getchar() != '\n');
//            printf("Result of (%f %c %f):", operand1, oper, operand2);
//            value = Subtract(operand1, operand2);
//            printf("%f", value);
//            //Subtract operator
//        } else if (oper == 'm') {
//            printf("Enter the first operand:");
//            scanf("%f", &operand1);
//            while (getchar() != '\n');
//            printf("Enter the second operand:");
//            scanf("%f", &operand2);
//            //            while (getchar() != '\n');
//            printf("Result of (%f %c %f):", operand1, oper, operand2);
//            value = Average(operand1, operand2);
//            printf("%f", value);
//            //calculation of average operator
//        } else if (oper == 'a') {
//            printf("Enter the first operand:");
//            scanf("%f", &operand);
//            while (getchar() != '\n');
//            printf("Result of |%f|:", operand);
//            value = AbsoluteValue(operand);
//            printf("%f", value);
//            //absolute value operator only one number needed so one operand
//        } else if (oper == 'c') {
//            printf("Enter the first operand:");
//            scanf("%f", &operand);
//            while (getchar() != '\n');
//            printf("Result of (%f deg->C):", operand);
//            value = FahrenheitToCelsius(operand);
//            printf("%f", value);
//            //calculate temperature only one operand needed
//            //fahr to Celsius
//        } else if (oper == 'f') {
//            printf("Enter the first operand:");
//            scanf("%f", &operand);
//            while (getchar() != '\n');
//            printf("Result of (%f deg->F):", operand);
//            value = CelsiusToFahrenheit(operand);
//            printf("%f", value);
//            //temperature only need one operand
//            //Celsius to Fahr
//        } else if (oper == 't') {
//            printf("Enter the first operand:");
//            scanf("%f", &operand);
//            while (getchar() != '\n');
//            printf("Result of tan(%f):", operand);
//            value = Tangent(operand);
//            printf("%f", value);
//            //calculate tangent value need one operand
//        } else if (oper == 'r') {
//            printf("Enter the first operand:");
//            scanf("%f", &operand);
//            while (getchar() != '\n');
//            printf("Result of round(%f):", operand);
//            value = Round(operand);
//            printf("%f", value);
//            //just round a number
//        } else if (oper == 'q') {
//            return;
//        } else {
//            printf("Invalid Operator.");
//            //quit the function
//        }
//        while (getchar() != '\n');
//    }
//
//    BOARD_End();
//    while (1);
//}
//
///********************************************************************************
// * Define the Add function here.
// ********************************************************************************/
//double Add(double operand1, double operand2) {
//    return operand1 + operand2;
//}
//
///********************************************************************************
// * Define the Subtract function here.
// ********************************************************************************/
//double Subtract(double operand1, double operand2) {
//    return operand1 - operand2;
//}
//
///********************************************************************************
// * Define the Multiply function here.
// ********************************************************************************/
//double Multiply(double operand1, double operand2) {
//    return operand1 * operand2;
//}
//
///********************************************************************************
// * Define the Divide function here.
// ********************************************************************************/
//double Divide(double operand1, double operand2) {
//    double value;
//    if (operand2 != 0) {
//        value = operand1 / operand2;
//        return value;
//    } else {
//        value = HUGE_VAL;
//        return value;
//    }
//}
//
///********************************************************************************
// * Define the Absolute Value function here.
// ********************************************************************************/
//double AbsoluteValue(double operand) {
//    double value;
//    if (operand < 0) {
//        value = (-1) * operand;
//        return value;
//    } else {
//        value = operand;
//        return value;
//    }
//}
//
///*********************************************************************************
// * Define the Fahrenheit to Celsius function here.
// ********************************************************************************/
//double FahrenheitToCelsius(double operand) {
//    return (5.0 / 9.0) * (operand - 32.0);
//}
//
///*********************************************************************************
// * Define the Celsius to Fahrenheit function here.
// ********************************************************************************/
//double CelsiusToFahrenheit(double operand) {
//    return (operand * (9.0 / 5.0)) + 32.0;
//}
//
///********************************************************************************
// * Define the Average function here.
// *******************************************************************************/
//double Average(double operand1, double operand2) {
//    return (operand1 + operand2) / 2;
//}
//
///*********************************************************************************
// * Define the Tangent function that takes input in degrees.
// ********************************************************************************/
//double Tangent(double operand) {
//    return tan(operand * (M_PI / 180));
//}
//
///*********************************************************************************
// * Define the Round function here.
// * In order to receive the extra credit your calculator MUST ALSO CORRECTLY utilize
// * this function.
// ********************************************************************************/
//double Round(double operand) {
//    return (operand > 0.0) ? floor(operand + 0.5) : ceil(operand - 0.5);
//}


// Standard libraries
#include <stdio.h>
#include <math.h>

//User libraries:
#include "BOARD.h"

// **** Declare function prototypes ****
double Add(double operand1, double operand2);
double Multiply(double operand1, double operand2);
double AbsoluteValue(double operand1);
double FahrenheitToCelsius(double operand1);
double CelsiusToFahrenheit(double operand1);
double Tangent(double operand1);
double Average(double operand1, double operand2);
double Divide(double operand1, double operand2);
double Subtract(double operand1, double operand2);
double Round(double operand);

void CalculatorRun(void)
{
    char operation;
    double operand1;
    double operand2;
    int loop = 1;
    
    printf("Welcome to Ayush's calculator program! Compiled at %s %s\n", __DATE__, __TIME__);
    
    // Your code here
    printf("Enter a mathematical operation to perform (*, /, +, -, v, a, c, f, t, r): ");
    scanf(" %c", &operation);
    
    printf("Enter the first operand: ");
    scanf("%lf", &operand1);
    
    if (operation != 'a' && operation != 'm' && operation != 'c' && operation != 'f' && operation != 't' && operation != 'r') {
        printf("Enter the second operand: ");
        scanf("%lf", &operand2);
    } else {
        if (operation == 'a'){
            double result = AbsoluteValue(operand1);
            printf("Result of |%.2lf|: %.2lf\n", operand1, result);
        }
        if (operation == 'm'){
            double result = Average(operand1, operand2);
            printf("Result of avg(%.2lf): %.2lf\n", operand1, result);
        }
        if (operation == 'c'){
            double result = FahrenheitToCelsius(operand1);
            printf("Result of (%.2lf deg->C): %.2lf\n", operand1, result);
        }
        if (operation == 'f'){
            double result = CelsiusToFahrenheit(operand1);
            printf("Result of (%.2lf deg->F): %.2lf\n", operand1, result);
        }
        if (operation == 't'){
            double result = Tangent(operand1);
            printf("Result of tan(%.2lf): %.2lf\n", operand1, result);
        }
        if (operation == 'q'){
           int loop;
           loop = 0;
        }
    }
    
    if (operation == '+'){
            int result = Add(operand1, operand2);
            printf("result of %i + %i:", operand1, operand2);
            printf(result);
    }
    if (operation == '*'){
            int result = Multiply(operand1, operand2);
            printf("result of %i * %i is:", operand1, operand2);
            printf(result);
    }
    if (operation == '-'){
            int result = Subtract(operand1, operand2);
            printf("result of %i - %i: ", operand1, operand2);
            printf(result);
    }
    if (operation == '/'){
            int result = Divide(operand1, operand2);
            printf("result of %i / %i:", operand1, operand2);
            printf(result);
    }
    
    
    while (loop);// this while(1) can be removed as needed.
}



/********************************************************************************
 * Define the Add function here.
 ********************************************************************************/
double Add(double operand1, double operand2)
{
    return operand1 + operand2;
}
/********************************************************************************
 * Define the Subtract function here.
 ********************************************************************************/
double Subtract(double operand1, double operand2)
{
	return operand1 - operand2;
}

/********************************************************************************
 * Define the Multiply function here.
 ********************************************************************************/
double Multiply(double operand1, double operand2)
{
    return operand1 * operand2;
}

/********************************************************************************
 * Define the Divide function here.
 ********************************************************************************/
double Divide(double operand1, double operand2)
{
	return operand1 / operand2;
}

/********************************************************************************
 * Define the Absolute Value function here.
 ********************************************************************************/
double AbsoluteValue(double operand1)
{
    return fabs(operand1);
}

/*********************************************************************************
 * Define the Fahrenheit to Celsius function here.
 ********************************************************************************/
double FahrenheitToCelsius(double operand1)
{
   return (operand1 - 32) * 5 / 9;
}

/*********************************************************************************
 * Define the Celsius to Fahrenheit function here.
 ********************************************************************************/
double CelsiusToFahrenheit(double operand1)
{
   return (operand1 * 9 / 5) + 32;
}
/********************************************************************************
 * Define the Average function here.
 *******************************************************************************/
double Average(double operand1, double operand2){
    int avg;
    avg = ((operand1 + operand2) / 2);
    return avg;
}

/*********************************************************************************
 * Define the Tangent function that takes input in degrees.
 ********************************************************************************/
double Tangent(double operand1){
    
    return tan(operand1);
}
/*********************************************************************************
 * Define the Round function here.
 * In order to receive the extra credit your calculator MUST ALSO CORRECTLY utilize
 * this function.
 ********************************************************************************/
double Round(double operand)
{
    return 1;
}



