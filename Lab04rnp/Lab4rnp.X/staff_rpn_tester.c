// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//CMPE13 Support Library
#include "BOARD.h"
#include "_cruzID.h"
#include "autotest_tools.h"

// Microchip libraries
#include <xc.h>

// User libraries
#include "rpn.h"

// A useful struct for managing string tests:

typedef struct {
    double result;
    char RPN_string[80];
    int RPN_error;
} rpn_test;


//module-level vars:
int pass = FALSE;
int undefined_fcns = FALSE;

//helper functions for test harness:

int RPN_ValueTest(rpn_test test)
{
    double student_result;
    g_printf("   RPN STR: \"%s\" :\n", test.RPN_string);
    int student_error = RPN_Evaluate(test.RPN_string, &student_result);
    g_printf("   Result (%d | %d) (%f | %f)\n",
            student_error, test.RPN_error,
            student_result, test.result);
    student_error++; //this is a dumb way to suppress an "unused variable" warning when not in GRADER_MODE
    return (equal_within_fp_delta(student_result, test.result));
}

int RPN_ErrorTest(rpn_test test)
{
    double student_result;
    g_printf("   RPN STR: \"%s\" :\n", test.RPN_string);
    int student_error = RPN_Evaluate(test.RPN_string, &student_result);
    g_printf("   Result error (%d | %d) \n ", student_error, test.RPN_error);
    return (student_error == test.RPN_error);
}

//rubric item testers:

void Test_SimpleStrings(void)
{
    rpn_test test0 = {2, "1 1 +", RPN_NO_ERROR};
    subtestResult(RPN_ValueTest(test0), "TestSimple0");

    rpn_test test1 = {-1, "1 1 1 + -", RPN_NO_ERROR};
    subtestResult(RPN_ValueTest(test1), "TestSimple1");

    rpn_test test2 = {-4, "6 10 - 1 1 * /", RPN_NO_ERROR};
    subtestResult(RPN_ValueTest(test2), "TestSimple2");
}

void Test_ComplexStrings(void)
{
    g_printf("Testing floats:\n");
    rpn_test test0 = {2.43, "1.0 1.43 +", RPN_NO_ERROR};
    subtestResult(RPN_ValueTest(test0), "TestComplex0");

    g_printf("Testing negatives:\n");
    rpn_test test1 = {-0.0302, "-3.02 100 /", RPN_NO_ERROR};
    subtestResult(RPN_ValueTest(test1), "TestComplex1");

    g_printf("Testing zero floats:\n");
    rpn_test test2 = {1.86, "0.93 -0.93 -", RPN_NO_ERROR};
    subtestResult(RPN_ValueTest(test2), "TestComplex2");

    g_printf("Testing a long string:\n");
    rpn_test test3 = {-7.127272, "0.6 3.1 * 5 * -2.2 / 10 + 12 3 1.3 0 - * + - 3 +", RPN_NO_ERROR};
    subtestResult(RPN_ValueTest(test3), "TestComplex3");
}

void Test_RPN_Errors(void)
{
    g_printf("Testing NO_ERROR:\n");
    {
        rpn_test test = {0, "1 1 +", RPN_NO_ERROR};
        subtestResult(RPN_ErrorTest(test), "TestErrors0");
    }

    g_printf("Testing stack underflow:\n");
    {
        rpn_test test = {0, "+", RPN_ERROR_STACK_UNDERFLOW};
        subtestResult(RPN_ErrorTest(test), "TestErrors1");
    }
    {
        rpn_test test = {0, "1 1 + +", RPN_ERROR_STACK_UNDERFLOW};
        subtestResult(RPN_ErrorTest(test), "TestErrors2");
    }

    g_printf("Testing stack overflow:\n");
    {
        rpn_test test = {0, "0 1 2 3 4 5 6 7 8 9 10"
            "11 12 13 14 15 16 17 18 19 20 21", RPN_ERROR_STACK_OVERFLOW};
        subtestResult(RPN_ErrorTest(test), "TestErrors3");
    }

    g_printf("Testing divide by zero:\n");
    {
        rpn_test test = {0, "1 0 /", RPN_ERROR_DIVIDE_BY_ZERO};
        subtestResult(RPN_ErrorTest(test), "TestErrors4");
    }
    {
        rpn_test test = {0, "1 8 3 3 - / +", RPN_ERROR_DIVIDE_BY_ZERO};
        subtestResult(RPN_ErrorTest(test), "TestErrors5");
    }

    g_printf("Testing invalid tokens:\n");
    {
        rpn_test test = {0, "a", RPN_ERROR_INVALID_TOKEN};
        subtestResult(RPN_ErrorTest(test), "TestErrors6");
    }
    {
        rpn_test test = {0, "1 a +", RPN_ERROR_INVALID_TOKEN};
        subtestResult(RPN_ErrorTest(test), "TestErrors7");
    }
}

void Test_EdgeCases(void)
{
    g_printf("Testing strange characters that aren't actually invalid:\n");
    {
        rpn_test test = {0, "0 0.0 -0.0 .0 + + +", RPN_NO_ERROR};
        subtestResult(RPN_ErrorTest(test), "TestEdgeCases2");
    }
    g_printf("Testing characters that look valid but aren't:)\n");
    {
        rpn_test test = {1.1, ". 9 +", RPN_ERROR_INVALID_TOKEN};
        subtestResult(RPN_ErrorTest(test), "TestEdgeCases3");
    }
    {
        rpn_test test = {1.1, "0.00notAtoken 1 +", RPN_ERROR_INVALID_TOKEN};
        subtestResult(RPN_ErrorTest(test), "TestEdgeCases4");
    }
    g_printf("Testing a single token (should be valid!)\n");
    {
        rpn_test test = {1.1, "1.1", RPN_NO_ERROR};
        subtestResult(RPN_ErrorTest(test), "TestEdgeCases5");
    }
    g_printf("Testing a very long string (RPN_Evaluate has no size limit!:\n");
    {
        char * long_string = "1.001 3.003 + 5 + 7 3.33 * + 18 + 3 - 7.5 5 / 3 + 2.222 2.222 / * +";
        rpn_test test = {51.813999, "", RPN_NO_ERROR};

        strcpy(test.RPN_string, long_string); //we get warnings if we try to assign a very long string on initialization
        subtestResult(RPN_ErrorTest(test), "TestEdgeCases0");
        strcpy(test.RPN_string, long_string);
        subtestResult(RPN_ValueTest(test), "TestEdgeCases1");
    }

    //    g_printf("Testing an empty string.  Behavior undefined, but shouldn't crash their code\n");
    //    {
    //        char test_str[60] = "";
    //        double dummy;
    //        RPN_Evaluate(test_str, &dummy);
    //    }
};

void Test_ProcessBackspaces(void)
{ // Test extra credit
    char testStrings[][13] = {
        "1 2\b3 +",
        "10.0\b\b\b3 4 +",
        "1 2 +\b\b\b\b\b",
        "1.0\b\b\b\b"
    };
    //    const char *testBaseStrings[] = {
    //        "1 2\b3 +",
    //        "10.0\b\b\b3 4 +",
    //        "1 2 +\b\b\b\b\b",
    //        "1.0\b\b\b\b"
    //    };
    const char *testResults[] = {
        "1 3 +",
        "13 4 +",
        "",
        ""
    };
    int i;
    char * testnames[4] = {"TestProcessBackspaces0",
        "TestProcessBackspaces1",
        "TestProcessBackspaces2",
        "TestProcessBackspaces3"};
    for (i = 0; i < 4; ++i) {
        ProcessBackspaces(testStrings[i]);
        int passed = !strcmp(testStrings[i], testResults[i]);
        if (!passed) {
            g_printf("failed to convert '%s' to '%s', was '%s' instead\n",
                    testBaseStrings[i], testResults[i], testStrings[i]);
        }
        subtestResult(passed, testnames[i]);
    }
}

int main(void)
{
    BOARD_Init();

    printHeader("Lab04: RPN.c");
    g_printf("Results reported as: (expected err | student err) "
            "expected result | student result)\n");


    startSubtestRun("RPN_SimpleStrings");
    Test_SimpleStrings();
    endSubtestRun(1.0);

    startSubtestRun("RPN_ComplexStrings");
    Test_ComplexStrings();
    endSubtestRun(1.0);

    startSubtestRun("RPN_Errors");
    Test_RPN_Errors();
    endSubtestRun(1.0);

    startSubtestRun("RPN_EdgeCases");
    Test_EdgeCases();
    endSubtestRun(1.0);

    startSubtestRun("RPN_ProcessBackspaces");
    Test_ProcessBackspaces();
    endSubtestRun(1.0);

    printTotalScore();

    if (undefined_fcns) {
        printf("\nWarning:  Student code did not define some functions!");
    }

    printFooter();

    return 0;
}




/* Many students in this lab fail to define all their functions.  The following 
 * weak function definitions will define their functions if their code does not.
 *  */

// <editor-fold defaultstate="collapsed" desc="weak function defs:">

__attribute__((weak)) int ProcessBackspaces(char *rpn_sentence)
{
    undefined_fcns = TRUE;
    return 0;
};


// </editor-fold>
