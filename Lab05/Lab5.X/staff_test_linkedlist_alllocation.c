/*Because there are so many ways for students to crash the test code in this lab, 
 * the staff test harness is spread into several different harnesses.
 * 
 * This harness tests whether the student's create and delete functions allocate and deallocate
 * the correct amount of memory.
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

void TestMemoryAllocation(void)
{
    g_printf("Checking memory allocation in call to New:\n");
    {
        int orig_mem = LL_measureHeap();
        ListItem* dummy = LinkedListNew("This long string shouldn't end up on the heap!");
        subtestResult(orig_mem - LL_measureHeap() == 1, "allocation0");
        LL_FreeList(dummy);
    }
    g_printf("Checking memory allocation in call to CreateAfter:\n");
    {
        ListItem* dummy = LL_CreateList(1, "dummy");
        int orig_mem = LL_measureHeap();
        LinkedListCreateAfter(
                dummy, "Also, this string shouldn't get heap space either!\n");
        subtestResult(orig_mem - LL_measureHeap() == 1, "allocation1");
        LL_FreeList(dummy);
    }

    g_printf("Checking memory allocation in call to Remove:");
    {
        ListItem* dummy = LL_CreateList(3, "A", "B", "C");
        int orig_mem = LL_measureHeap();
        LinkedListRemove(dummy->nextItem);
        subtestResult(orig_mem - LL_measureHeap() == -1, "allocation2");
        LL_FreeList(dummy);
    }
}

int main()
{
    BOARD_Init();

    printHeader("Lab05 Allocation Tests ");

    int max_heap = LL_measureHeap();
    printf("On initialization, heap has room for %d listitems.\n", max_heap);


    startSubtestRun("test_LinkedList_MemoryAllocation");
    TestMemoryAllocation();
    endSubtestRun(0.5);

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
