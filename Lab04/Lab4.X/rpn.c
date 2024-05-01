/* 
 * This file contain the Reverse Polish Notation to evaluate numbers.
 * 
 * File:   rpn.c
 * 
 * Author: Mary Xu
 *
 * Created on April 18, 2022, 12:07 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "BOARD.h"
#include "rpn.h"
#include "stack.h"

/* RPN_Evaluate() parses and evaluates a string that contains 
 * a valid Reverse Polish Notation string (no newlines!)  
 * @param:  rpn_string - a string in polish notation.  Tokens must be either 
 *          arithmetic operators or numbers.
 * @param:  result - a pointer to a double that will be modified to contain
 *          the return value of the rpn expression.
 * @return: error - if the rpn expression is invalid, 
 *          an appropriate rpn_error value is returned.
 * 
 * RPN_Evaluate supports the following basic arithmetic operations:
 *   + : addition
 *   - : subtraction
 *   * : multiplication
 *   / : division
 * Numeric tokens can be positive or negative, and can be integers or 
 * decimal floats.  RPN_Evaluate should be able to handle strings of 
 * at least 255 length.
 * */
int RPN_Evaluate(char * rpn_string, double * result){
    struct Stack value;
    StackInit(&value);
    char *token;
    token = strtok(rpn_string, " ");
    
    double rpn_num;
    double input_num;
    double num1;
    double num2;
    //all the basic declare value needed
    
    while (token != NULL){
        switch (*token){
            default:
                rpn_num = atof(token);
                if(atof(token) == 0){
                    return RPN_ERROR_INVALID_TOKEN;
                }else{
                    StackPush(&value, rpn_num);
                }
            case '+':
                if(StackGetSize(&value) >= 2){
                    StackPop(&value, &num1);
                    StackPop(&value, &num2);
                    StackPush(&value, num2 + num1);
                    //pop the number then push the calculations
                break;
            case'-':
                if(StackGetSize(&value) >= 2){
                    StackPop(&value, &num1);
                    StackPop(&value, &num2);
                    StackPush(&value, num2 - num1);
                    //pop the number then push the calculations
                break;
            case'*':
                if(StackGetSize(&value) >= 2){
                    StackPop(&value, &num1);
                    StackPop(&value, &num2);
                    StackPush(&value, num2 * num1);
            case'/':
                if(StackGetSize(&value) >= 2){
                    StackPop(&value, &num1);
                    StackPop(&value, &num2);
                    StackPush(&value, num2 / num1);
                }else if (num1 == 0){
                    return RPN_ERROR_DIVIDE_BY_ZERO;
                }else{
                    return RPN_ERROR_INVALID_TOKEN;
                }
        }
        token = strtok(NULL, "");
    }
    if(StackGetSize(&value) > 3){
        return RPN_ERROR_TOO_MANY_ITEMS_REMAIN;
    }else if (StackGetSize(&value) < 3){
        return RPN_ERROR_TOO_FEW_ITEMS_REMAIN;
    }
    StackPop(&value, result);
    return 1;
}


/**
 * This function should read through an array of characters, checking for backspace characters. 
 * When it encounters a backspace character, it eliminates the backspace, the preceeding character
 * (if such a character exists), and shifts all subsequent characters as appropriate.
 * @param string_to_modify The string that will be processed for backspaces. 
 *        This string is modified "in place", so it is both an input and an output to the function.
 * @return Returns the size of the resulting string in "string_to_modify".
 *
 * ProcessBackspaces() should be able to handle multiple repeated backspaces and also
 * strings with more backspaces than characters. It should be able to handle strings of at least 255 length.
 * 
 * */
int ProcessBackspaces(char *rpn_sentence){
    char *token = strtok(rpn_sentence, " ");
    int number = 0;
    
    while(token != NULL){
        rpn_sentence[number] = *token;
        number ++;
    }
    return number;
}
