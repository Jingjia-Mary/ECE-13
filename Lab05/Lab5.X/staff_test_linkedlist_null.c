/*Because there are so many ways for students to crash the test code in this lab, 
 * the staff test harness is spread into several different harnesses.
 * 
 * This harness tests whether the student's linked list functions work when null pointers
 * are passed in.
 */

// Heap size 240 required!

// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <string.h>
#include <ctype.h>

//CMPE13 Support Library
#include "BOARD.h"
#include "autotest_tools.h"
#include "LL_helpers.h"

// Microchip libraries
#include <xc.h>


#include "LinkedList.h"

void TestNullInputs(void)
{
    g_printf("New with NULL:\n");
    {
        ListItem * expected = LL_CreateList(1, NULL);
        ListItem * result = LinkedListNew(NULL);
        subtestResult(LL_CompareLists(result, expected), "Null0");

        LL_FreeList(expected);
        LL_FreeList(result);
    }
    g_printf("CreateAfter with NULL ListItem:\n");
    {
        ListItem * expected = LL_CreateList(1, "Hello");
        ListItem * result = LinkedListCreateAfter(NULL, "Hello");
        subtestResult(LL_CompareLists(result, expected), "Null1");

        LL_FreeList(expected);
        LL_FreeList(result);
    }
    g_printf("CreateAfter with NULL data:\n");
    {
        ListItem * expected = LL_CreateList(2, "Hello", NULL);
        ListItem * resultBase = LL_CreateList(1, "Hello");
        ListItem * result = LinkedListCreateAfter(resultBase, NULL);
        subtestResult(LL_CompareLists(result, expected), "Null2");

        LL_FreeList(expected);
        LL_FreeList(result);
        LL_FreeList(resultBase);
    }
    g_printf("Size on NULL:\n");
    {
        subtestResult(0 == LinkedListSize(NULL), "Null3");
    }
    g_printf("Remove on NULL:\n");
    {
        subtestResult(0 == LinkedListRemove(NULL), "Null4");
    }
    g_printf("GetFirst on NULL:\n");
    {
        subtestResult(0 == LinkedListGetFirst(NULL), "Null5");
    }
    g_printf("SwapData with NULL ListItem pointer:\n");
    {
        ListItem* test = LL_CreateList(1, "TEST");
        subtestResult(0 == LinkedListSwapData(test, NULL), "Null6");
        LL_FreeList(test);
    }
    
    g_printf("Print with a NULL ListItem pointer:\n");
    {
        subtestResult(LinkedListPrint(NULL) == STANDARD_ERROR, "Null9");
    }

}

int main()
{
    BOARD_Init();

    printHeader("Lab05 Linked List NULL Input");

    int max_heap = LL_measureHeap();
    printf("On initialization, heap has room for %d listitems.\n", max_heap);

    startSubtestRun("test_linkedList_NullInputHandling");
    TestNullInputs();
    endSubtestRun(1);

    printTotalScore();


    //Final check to ensure that heap didn't fill up:
    //    printf("\n\n--------------Final heap space remaining (in sizeof(ListItems)): %d\n", measureHeap());
    void* mallocTest = malloc(sizeof (ListItem)*10);
    if (mallocTest == NULL) {
        puts("WARNING: heap filled up during testing\n");
    }

    printFooter();
    return 0;
}
