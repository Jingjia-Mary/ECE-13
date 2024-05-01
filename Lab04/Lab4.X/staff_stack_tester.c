// **** Include libraries here ****
// Standard libraries

//CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>
#include <stdio.h>
//#include <plib.h>

// User libraries
#include "stack.h"
#include "_cruzID.h"
#include "autotest_tools.h"


// This code will unit test the Stack.c implementation. You should run at least two different tests
// on each function to be able to determine if it is working correctly. You will have to write these
// tests yourself.

/* Tests StackInit(), and returns the number of tests passed.*/
void Test_StackInit(void)
{
    g_printf("StackInit() sets initialized member to TRUE\n");
    {
        struct Stack stack = {};
        StackInit(&stack);
        subtestResult(stack.initialized, "StackInit0");
    }

    g_printf(": StackInit() sets currentItemIndex to -1\n");
    {
        struct Stack stack = {};
        StackInit(&stack);
        subtestResult(stack.currentItemIndex == -1, "StackInit1");
    }
};

void Test_StackIsEmpty(void)
{


    g_printf("StackIsEmpty() is correct on empty stacks\n");
    {
        struct Stack stack = {
            {}, -1, TRUE
        };
        subtestResult(StackIsEmpty(&stack), "StackIsEmpty0");
    }

    g_printf("StackIsEmpty() is correct on non-empty stacks\n");
    {
        struct Stack stack = {
            {}, 0, TRUE
        };
        subtestResult(!StackIsEmpty(&stack), "StackIsEmpty1");
    }
}

void Test_StackIsFull(void)
{

    g_printf("StackIsFull() is correct on full stacks\n");
    {
        struct Stack stack = {
            {}, STACK_SIZE - 1, TRUE
        };
        subtestResult(StackIsFull(&stack), "StackIsFull10\n");
    }

    g_printf("StackIsFull() is correct on non-full stacks");
    {
        struct Stack stack1 = {
            {}, -1, TRUE
        };
        struct Stack stack2 = {
            {}, STACK_SIZE / 2, TRUE
        };
        subtestResult(!StackIsFull(&stack1) && !StackIsFull(&stack2), "StackIsFull1\n");
    }
}

void Test_StackGetSize(void)
{


    g_printf("StackGetSize() returns 0 for empty stacks\n");
    {
        struct Stack stack = {
            {}, -1, TRUE
        };
        subtestResult(StackGetSize(&stack) == 0, "StackGetSize0");
    }

    g_printf("StackGetSize() is correct on non-empty stacks\n");
    {
        struct Stack stack = {
            {}, -1, TRUE
        };
        uint8_t stackGetSizePassed = TRUE;
        int i;
        for (i = 0; i < STACK_SIZE; ++i) {
            stack.currentItemIndex = i;

            if (StackGetSize(&stack) != i + 1) {
                stackGetSizePassed = FALSE;
            }
        }
        subtestResult(stackGetSizePassed, "StackGetSize1");
    }
}


void Test_StackPush(void)
{
    g_printf("StackPush() is correct on non-empty stacks\n");
    {
        struct Stack stack;
        StackInit(&stack);
        uint8_t stackPushPassed = TRUE;
        
        int i;
        for (i = 0; i < STACK_SIZE; ++i) {
            if (!StackPush(&stack, i)) {
                stackPushPassed = FALSE;
                break;
            }
            if (stack.stackItems[stack.currentItemIndex]!=i){
                stackPushPassed = FALSE;
                break;                
            }
        }
        subtestResult(stackPushPassed, "StackPush0");
    }
}

void Test_StackPop(void)
{
    g_printf("StackPop() is correct on non-empty stacks\n");
    {
        struct Stack stack;
        StackInit(&stack);
        uint8_t stackPopPassed = TRUE;
        
        int i;
        double result;
        for (i = 0; i < STACK_SIZE; ++i) StackPush(&stack, i);
        for (i = STACK_SIZE-1; i >=0; --i){
            if(!StackPop(&stack, &result)){
                stackPopPassed = FALSE;
                break;
            }
            if (result!=i){
                stackPopPassed = FALSE;
                break;                
            }
        }
        subtestResult(stackPopPassed, "StackPop0");
    }
}

void Test_ErrorChecking(void)
{
    // 3 tests for StackIsEmpty()
    g_printf("StackIsEmpty() fails on non-initialized stacks\n");
    {
        struct Stack stack = {
            {}, -1, FALSE
        };
        subtestResult(!StackIsEmpty(&stack), "StackError0");
    }
    // 3 tests for StackIsFull() 
    g_printf("StackIsFull() fails on non-initialized stacks\n");
    {
        struct Stack stack = {};
        subtestResult(!StackIsFull(&stack), "StackError1");
    }
    // 3 tests for StackGetSize()
    g_printf("StackGetSize() fails on non-initialized stacks\n");
    {
        struct Stack stack = {};
        subtestResult(StackGetSize(&stack) == SIZE_ERROR, "StackError2");
    }
    g_printf("StackPush() fails on non-initialized stacks\n");
    {
        struct Stack stack = {};
        double rv = 1.1;
        subtestResult(!StackPush(&stack, rv), "StackError3");
    }

    g_printf("StackPush() fails on full stacks\n");
    {
        struct Stack stack = {
            {}, STACK_SIZE - 1, TRUE
        };
        double rv = 2.2;
        subtestResult(!StackPush(&stack, rv), "StackError4");
    }

    // 3 tests for StackPop()
    g_printf("StackPop() fails on non-initialized stacks\n");
    {
        struct Stack stack = {};
        double rv;
        subtestResult(!StackPop(&stack, &rv), "StackError5");
    }

    g_printf("StackPop() fails on empty stacks\n");
    {
        struct Stack stack = {
            {}, -1, TRUE
        };
        double rv;
        subtestResult(!StackPop(&stack, &rv), "StackError6");
    }
}

void TestSilentOperation(void)
{
    printf("NOTHING should appear between (vvv) and (^^^) below:\n");
    printf("  vvvvvvvvvvv  \n");
    double dummyval;
    struct Stack stack;
    int i, j;
    stack.currentItemIndex = 3;
    stack.initialized = 0;
    //run through everything, once initialized and once not
    for (i = 0; i < 2; i++) {
        StackGetSize(&stack);
        StackIsEmpty(&stack);
        StackIsFull(&stack);
        for (j = 0; j < (STACK_SIZE + 2); j++) {
            StackPush(&stack, (double) rand());
        }
        for (j = 0; j < (STACK_SIZE + 2); j++) {
            StackPop(&stack, &dummyval);
        }
        StackInit(&stack);
    };
    printf("  ^^^^^^^^^^^  \n");
}

int main()
{
    BOARD_Init();
    printHeader("Lab04: Stack.c");
    int randSeed = rand();
    printf("This test used a random seed = %d\n", randSeed);
    srand(randSeed);


    /******************************************************************************
     * Perform our tests:
     *****************************************************************************/

    startSubtestRun("stack_StackInit");
    Test_StackInit();
    endSubtestRun(0.5);


    startSubtestRun("stack_StackIsEmpty");
    Test_StackIsEmpty();
    endSubtestRun(0.5);

    startSubtestRun("stack_StackIsFull");
    Test_StackIsFull();
    endSubtestRun(0.5);

    startSubtestRun("stack_StackGetSize");
    Test_StackGetSize();
    endSubtestRun(0.5);

    startSubtestRun("stack_ErrorChecking");
    Test_ErrorChecking();
    endSubtestRun(1);

    startSubtestRun("stack_StackPush");
    Test_StackPush();
    endSubtestRun(0.5);
    
    startSubtestRun("stack_StackPop");
    Test_StackPop();
    endSubtestRun(0.5);
    
    printTotalScore();

    printf("\nTesting for silence:\n");
    TestSilentOperation();

    printFooter();

    return 0;

    //    puts("[CHECK]: The STACK_SIZE constant was used.");  //should be in a separate grading section now

#if 0

#endif


    /*************************************************************************************************/

    // You can never return from main() in an embedded system (one that lacks an operating system).
    // This will result in the processor restarting, which is almost certainly not what you want!
}


