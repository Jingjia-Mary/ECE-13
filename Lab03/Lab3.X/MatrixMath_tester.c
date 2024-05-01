// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

//CMPE13 Support Library
#include "BOARD.h"



// User libraries
#include "MatrixMath.h"
//#include "MatrixMath_correct.h"
#include "autotest_tools.h"

//module-level vars:
int pass = FALSE;
int undefined_fcns = FALSE;

// <editor-fold defaultstate="collapsed" desc="Test harness matrix prints">

//A function that prints out two matrices in grader-only lines

void g_MatrixPrint(float exp[3][3], float act[3][3])
{
    int r, c;
    g_printf("expected:                         actual:\n");
    for (r = 0; r < DIM; r++) {
        g_printf("");
        for (c = 0; c < DIM; c++) {
            g_printf("| % -7.2f ", (double) exp[r][c]);
        }
        g_printf("|   ");
        for (c = 0; c < DIM; c++) {
            g_printf("| % -7.2f ", (double) act[r][c]);
        }
        g_printf("|\n");
    }
};

void g_MatrixPrint2(float exp[2][2], float act[2][2])
{
    int r, c;
    g_printf("   expected:                actual:\n");
    for (r = 0; r < (DIM - 1); r++) {
        g_printf("   ");
        for (c = 0; c < (DIM - 1); c++) {
            g_printf("| % -7.2f ", (double) exp[r][c]);
        }
        g_printf("|    ");
        for (c = 0; c < (DIM - 1); c++) {
            g_printf("| % -7.2f ", (double) act[r][c]);
        }
        g_printf("|\n");
    }
};


// </editor-fold>

void _clearMatrix(float mat[3][3])
{
    int i;
    for (i = 0; i < 9; i++) mat[0][i] = 0;
}

int testMatrixEquals(void)
{
    int testsPassed = 0;
    g_printf("Testing two identical matrices:\n");
    {
        float m1[3][3] = {
            {1.1, 2.2, 3.3},
            {4.4, 5.5, 6.6},
            {7.7, 8.8, 9.9}
        };
        pass = subtestResult(MatrixEquals(m1, m1), "MatrixEquals0");
    }

    g_printf("Testing two NEARLY identical matrices:\n");
    {
        float m1[3][3] = {
            {1.1, 2.2, 3.3},
            {4.4, 5.5, 6.6},
            {7.7, 8.8, 9.9}
        };
        float m2[3][3] = {
            {1.1 + FP_DELTA * 0.9, 2.2 - FP_DELTA * 0.9, 3.3},
            {4.4, 5.5, 6.6},
            {7.7, 8.8, 9.9}
        };

        pass = subtestResult(MatrixEquals(m1, m2), "MatrixEquals1");
    }

    g_printf("Testing two NON identical matrices:\n");
    { //Note that the middle element is different, which many students do not catch
        float m1[3][3] = {
            {1.1, 2.2, 3.3},
            {4.4, -5.5, 6.6},
            {7.7, 8.8, 9.9}
        };
        float m2[3][3] = {
            {1.1, 2.2, 3.3},
            {4.4, 5.5, 6.6},
            {7.7, 8.8, 9.9}
        };
        pass = subtestResult(!MatrixEquals(m1, m2), "MatrixEquals2"); //this is a type II test, so invert the result
    }
    return testsPassed;
};

int testMatrixAdd(void)
{
    // <editor-fold defaultstate="collapsed" desc="MatrixAdd Tests">

    int testsPassed = 0;

    g_printf("Basic Addition Test:\n");
    float m1[3][3] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };
    float m2[3][3] = {
        {10, 20, 30},
        {40, 50, 60},
        {70, 80, 90}
    };
    float expected[3][3] = {
        {11, 22, 33},
        {44, 55, 66},
        {77, 88, 99}
    };
    float result[3][3] = {};
    MatrixAdd(m1, m2, result);
    pass = subtestResult(MatrixEquals(result, expected), "MatrixAdd0");
    if (!pass) g_MatrixPrint(expected, result);


    g_printf("Addition Commutativaty Test:\n");
    _clearMatrix(result);
    MatrixAdd(m2, m1, result);
    pass = subtestResult(MatrixEquals(result, expected), "MatrixAdd1");
    if (!pass) g_MatrixPrint(expected, result);

    return testsPassed;

    // </editor-fold>
};

int testMatrixMultiply(void)
{
    int testsPassed = 0;

    float m1[3][3] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };
    float m2[3][3] = {
        {1, 4, 7},
        {2, 5, 8},
        {3, 6, 9}
    };
    float I[3][3] = {
        {1, 0, 0},
        {0, 1, 0},
        {0, 0, 1}
    };
    float result[3][3];
    float expected[3][3];

    g_printf("MatrixMultiply Identity Test:\n");
    MatrixMultiply(m1, I, result);
    pass = subtestResult(MatrixEquals(m1, result), "MatrixMultiply0");
    if (!pass) g_MatrixPrint(expected, result);


    g_printf("MatrixMultiply Basic Test:\n");
    _clearMatrix(result);
    MatrixMultiply(m1, m2, result);
    MatrixMultiply(m1, m2, expected);
    pass = subtestResult(MatrixEquals(expected, result), "Matrix");
    if (!pass) g_MatrixPrint(expected, result);

    g_printf("MatrixMultiply Anti-commutativity Test:\n");
    _clearMatrix(result);
    MatrixMultiply(m2, m1, result);
    MatrixMultiply(m2, m1, expected);
    pass = subtestResult(MatrixEquals(expected, result), "MatrixMultiply1");
    if (!pass) g_MatrixPrint(expected, result);

    return testsPassed;
};

int testMatrixScalarAdd(void)
{
    int testsPassed = 0;

    float m1[3][3] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };
    float result[3][3];
    float expected[3][3];

    g_printf("MatrixScalarAdd Zero Test:\n");
    _clearMatrix(result);
    MatrixScalarAdd(0, m1, result);
    MatrixScalarAdd(0, m1, expected);
    pass = subtestResult(MatrixEquals(expected, result), "MatrixScalarAdd0");
    if (!pass) g_MatrixPrint(expected, result);

    g_printf("MatrixScalarAdd Non-zero Test:\n");
    _clearMatrix(result);
    MatrixScalarAdd(-M_PI, m1, result);
    MatrixScalarAdd(-M_PI, m1, expected);
    pass = subtestResult(MatrixEquals(expected, result), "MatrixScalarAdd1");
    if (!pass) g_MatrixPrint(expected, result);

    return testsPassed;
};

int testMatrixScalarMultiply(void)
{
    int testsPassed = 0;

    float m1[3][3] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };
    float result[3][3];
    float expected[3][3];

    g_printf("MatrixScalarMultiply Unity Test:\n");
    _clearMatrix(result);
    MatrixScalarMultiply(1, m1, result);
    MatrixScalarMultiply(1, m1, expected);
    pass = subtestResult(MatrixEquals(expected, result), "MatrixScalarMultiply0");
    if (!pass) g_MatrixPrint(expected, result);

    g_printf("MatrixScalarMultiply Non-unity Test:\n");
    _clearMatrix(result);
    MatrixScalarMultiply(-M_PI, m1, result);
    MatrixScalarMultiply(-M_PI, m1, expected);
    pass = subtestResult(MatrixEquals(expected, result), "MatrixScalarMultiply1");
    if (!pass) g_MatrixPrint(expected, result);

    return testsPassed;
};

int testMatrixTrace(void)
{

    int testsPassed = 0;

    float m1[3][3] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };

    float result;
    float expected;

    g_printf("MatrixTrace Simple Test:\n");
    result = MatrixTrace(m1);
    expected = MatrixTrace(m1);
    pass = subtestResult(equal_within_fp_delta(expected, result), "MatrixTrace0");
    if (!pass) printf("expected | result = (%f | %f)\n", (double) expected, (double) result);

    float m2[3][3] = {
        {0.0271828183, 0, 0},
        {0, 1.48413159, 0},
        {0, 0, 81.0308393}
    };

    g_printf("MatrixTrace Diagonal Test:\n");
    result = MatrixTrace(m2);
    expected = MatrixTrace(m2);
    pass = subtestResult(equal_within_fp_delta(expected, result), "MatrixTrace1");
    if (!pass) printf("expected | result = (%f | %f)\n", (double) expected, (double) result);

    return testsPassed;
};

int testMatrixTranspose(void)
{
    int testsPassed = 0;

    float symm[3][3] = {
        {1, 0, -1},
        {0, 4, 2},
        {-1, 2, -4}
    };
    float m1[3][3] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };
    float m1_transpose[3][3] = {
        {1, 4, 7},
        {2, 5, 8},
        {3, 6, 9}
    };

    float result[3][3];

    g_printf("MatrixTranspose Symmetric Test:\n");
    _clearMatrix(result);
    MatrixTranspose(symm, result);
    pass = subtestResult(MatrixEquals(symm, result), "MatrixTranspose0");
    if (!pass) g_MatrixPrint(symm, result);

    g_printf("MatrixTranspose A-symmetric Test:\n");
    _clearMatrix(result);
    MatrixTranspose(m1, result);
    pass = subtestResult(MatrixEquals(result, m1_transpose), "MatrixTranspose1");
    if (!pass) g_MatrixPrint(m1_transpose, result);

    return testsPassed;
};

int testMatrixSubmatrix(void)
{
#define MatrixEquals2x2(m1, m2) ( \
        m1[0][0]==m2[0][0] && \
        m1[0][1]==m2[0][1] && \
        m1[1][0]==m2[1][0] && \
        m1[1][1]==m2[1][1])

    int testsPassed = 0;

    float m1[3][3] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };
    float result[2][2] = {};


    g_printf("MatrixSubmatrix Upper-Left Test:\n");
    {
        float expected[2][2] = {
            {5, 6},
            {8, 9}
        };
        MatrixSubmatrix(0, 0, m1, result);
        pass = subtestResult(MatrixEquals2x2(expected, result), "MatrixSubmatrix0");
        if (!pass) g_MatrixPrint2(expected, result);
    }

    g_printf("MatrixSubmatrix Center Test:\n");
    {
        float expected[2][2] = {
            {1, 3},
            {7, 9}
        };
        MatrixSubmatrix(1, 1, m1, result);
        pass = subtestResult(MatrixEquals2x2(expected, result), "MatrixSubmatrix1");
        if (!pass) g_MatrixPrint2(expected, result);
    }

    g_printf("MatrixSubmatrix Lower-Center Test:\n");
    {
        float expected[2][2] = {
            {1, 3},
            {4, 6}
        };
        MatrixSubmatrix(2, 1, m1, result);
        pass = subtestResult(MatrixEquals2x2(expected, result), "MatrixSubmatrix2");
        if (!pass) g_MatrixPrint2(expected, result);
    }
    return testsPassed;

    g_printf("MatrixSubmatrix Lower-Right Test:\n");
    {
        float expected[2][2] = {
            {1, 2},
            {4, 5}
        };
        MatrixSubmatrix(2, 2, m1, result);
        pass = subtestResult(MatrixEquals2x2(expected, result), "MatrixSubmatrix3");
        if (!pass) g_MatrixPrint2(expected, result);
    }
    return testsPassed;
};

int testMatrixDeterminant(void)
{
    int testsPassed = 0;
    float result;

    g_printf("   MatrixDeterminant Identity test:\n");
    {
        float I[3][3] = {
            {1, 0, 0},
            {0, 1, 0},
            {0, 0, 1}
        };
        float expected = 1;
        result = MatrixDeterminant(I);
        pass = subtestResult(equal_within_fp_delta(expected, result), "MatrixDeterminant0");
        if (!pass) printf("expected | result = (%f | %f)\n", (double) expected, (double) result);
    }

    g_printf("   MatrixDeterminant Basic test:\n");
    {
        float m1[3][3] = {
            {2, 3, 2},
            {4, 2, 2},
            {3, 6, 5}
        };
        float expected = -10;
        result = MatrixDeterminant(m1);
        pass = subtestResult(equal_within_fp_delta(expected, result), "MatrixDeterminant0");
        if (!pass) printf("expected | result = (%f | %f)\n", (double) expected, (double) result);
    }
    return testsPassed;
};

int testMatrixInverse(void)
{
    int testsPassed = 0;
    float result[3][3];

    g_printf("MatrixInverse Identity test:\n");
    {
        float I[3][3] = {
            {1, 0, 0},
            {0, 1, 0},
            {0, 0, 1}
        };
        _clearMatrix(result);
        MatrixInverse(I, result);
        pass = subtestResult(MatrixEquals(result, I), "MatrixInverse0");
        if (!pass) g_MatrixPrint(I, result);
    }

    g_printf("MatrixInverse Full test:\n");
    {
        float m1[3][3] = {
            {2, 3, 2},
            {4, 2, 2},
            {3, 6, 5}
        };
        float expected[3][3] = {
            {0.2, 0.3, -0.2},
            {1.4, -0.4, -0.4},
            {-1.8, 0.3, 0.8}
        };
        _clearMatrix(result);
        MatrixInverse(m1, result);
        pass = subtestResult(MatrixEquals(expected, result), "MatrixInverse1");
        if (!pass) g_MatrixPrint(expected, result);
    }
    return testsPassed;
};

/*Test that ensures (hopefully) that functions operate silently*/
void testSilentOperation(void)
{
    printf("NOTHING should appear between (vvv) and (^^^) below:\n");
    printf("  vvvvvvvvvvv  \n");

    float m1[3][3] = {
        {0, 1, 2},
        {3, 4, 5},
        {6, 7, 8}};
    float m2[3][3] = {
        {0, 1, 2},
        {3, -4, 5},
        {6, 7, 8}};
    float m3[2][2];
    ;
    MatrixEquals(m1, m2);
    MatrixAdd(m1, m2, m2);
    MatrixMultiply(m1, m2, m1);
    MatrixScalarAdd(5, m1, m2);
    MatrixScalarMultiply(5, m2, m1);
    MatrixTrace(m2);
    MatrixTranspose(m1, m2);
    MatrixDeterminant(m2);
    MatrixInverse(m2, m1);
    MatrixSubmatrix(1, 1, m1, m3);
    printf("  ^^^^^^^^^^^  \n");
};

int main()
{
    BOARD_Init();
    printHeader("Lab03: MatrixMath");

#ifdef AUTOGRADER
    printf("This script was auto-compiled and auto-run.\n");
#endif
    //    printf("Please note this autochecker is unfinished!\n");

    /******************************************************************************
     * Perform our tests:
     *****************************************************************************/
    int points_awarded = 0;

    startSubtestRun("MatrixEquals");
    testMatrixEquals();
    points_awarded += endSubtestRun(2);

    startSubtestRun("MatrixAdd");
    testMatrixAdd();
    points_awarded += endSubtestRun(1);

    startSubtestRun("MatrixMultiply");
    testMatrixMultiply();
    points_awarded += endSubtestRun(1);


    startSubtestRun("MatrixScalarAdd");
    testMatrixScalarAdd();
    points_awarded += endSubtestRun(0.5);


    startSubtestRun("MatrixScalarMultiply");
    testMatrixScalarMultiply();
    points_awarded += endSubtestRun(0.5);


    startSubtestRun("MatrixTrace");
    testMatrixTrace();
    points_awarded += endSubtestRun(1);


    startSubtestRun("MatrixTranspose");
    testMatrixTranspose();
    points_awarded += endSubtestRun(1);

    startSubtestRun("MatrixDeterminant");
    testMatrixDeterminant();
    points_awarded += endSubtestRun(0.5);

    startSubtestRun("MatrixInverse");
    testMatrixInverse();
    points_awarded += endSubtestRun(0.5);

    //For some reason MatrixSubmatrix() crashes in a lot of student code, so keep it at the end:
    startSubtestRun("MatrixSubmatrix");
    testMatrixSubmatrix();
    points_awarded += endSubtestRun(1);

    printf("\nMatrixPrint() testing (should be a pretty, readable grid):\n");
    {
        float TestMat[3][3];
        int i;
        for (i = 0; i < 9; i++) {
            //this line generates 0s, or + or - numbers between -999.0 and 999.0
            //Nearly all bad MatrixPrints will look bad after this!
            TestMat[0][i] = (rand() % 3 - 1) * ((float) rand() / (float) RAND_MAX) * 999;
        }
        MatrixPrint(TestMat);
    }

    printf("\n\n");
    printTotalScore();

    printf("\nTesting for silence:\n");
    testSilentOperation();

    printFooter();
    printf("DONE\n");

    if (undefined_fcns) {
        printf("\nWarning:  Student code did not define some functions!");
    }



    //while (1); you can turn this on to assist with debugging, but note that it makes the python autochecker run slower
    return 0;
}




/* Many students in this lab fail to define all their functions.  The following 
 * weak function definitions will define their functions if their code does not.
 *  */

// <editor-fold defaultstate="collapsed" desc="weak function defs:">

__attribute__((weak)) void MatrixPrint(float mat[3][3])
{
    undefined_fcns = TRUE;
};

__attribute__((weak)) int MatrixEquals(float mat1[3][3], float mat2[3][3])
{
    undefined_fcns = TRUE;
    return -999;
};

__attribute__((weak)) void MatrixAdd(float mat1[3][3], float mat2[3][3], float result[3][3])
{
    undefined_fcns = TRUE;
};

__attribute__((weak)) void MatrixMultiply(float mat1[3][3], float mat2[3][3], float result[3][3])
{
    undefined_fcns = TRUE;
};

__attribute__((weak)) void MatrixScalarAdd(float x, float mat[3][3], float result[3][3])
{
    undefined_fcns = TRUE;
};

__attribute__((weak)) void MatrixScalarMultiply(float x, float mat[3][3], float result[3][3])
{
    undefined_fcns = TRUE;
};

__attribute__((weak)) float MatrixTrace(float mat[3][3])
{
    undefined_fcns = TRUE;
    return -999;
};

__attribute__((weak)) void MatrixTranspose(float mat[3][3], float result[3][3])
{
    undefined_fcns = TRUE;
};

__attribute__((weak)) void MatrixSubmatrix(int i, int j, float mat[3][3], float result[2][2])
{
    undefined_fcns = TRUE;
};

__attribute__((weak)) float MatrixDeterminant(float mat[3][3])
{
    undefined_fcns = TRUE;
    return -999;
};

__attribute__((weak)) void MatrixInverse(float mat[3][3], float result[3][3])
{
    undefined_fcns = TRUE;
};

// </editor-fold>
