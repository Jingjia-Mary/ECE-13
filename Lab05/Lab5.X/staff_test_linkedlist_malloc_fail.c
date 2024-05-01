/*Because there are so many ways for students to crash the test code in this lab, 
 * the staff test harness is spread into several different harnesses.
 * 
 * This harness detects whether the student correctly handles malloc failures.
 * 
 * Note that this harness relies on some weird compiler magic, so its .o file
 * cannot be used directly to test student code.   See the comments for wipe_free() in LL_helpers.h.
 * (It works when compiled in this project configuration though)
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

void TestMallocFailure(void)
{
    ListItem * test = LL_CreateList(1, "DUMMY");
    LL_fillHeap();

    g_printf("New with malloc failure:\n");
    subtestResult(LinkedListNew("DUMMY") == NULL, "malloc_fail0");

    g_printf("CreateAfter with malloc failure\n:");
    subtestResult(LinkedListCreateAfter(test, "DUMMY") == NULL, "malloc_fail1");
    
    LL_clearHeap();
}

void TestFreeAccess(void){
    
    ListItem * test = LL_CreateList(1, "DUMMY");
    g_printf("Remove, with free()'d memory modified:\n");
    char * result = LinkedListRemove(test);
    subtestResult(!strcmp(result,"DUMMY"), "freed_access0");
}

int main()
{
    BOARD_Init();

    printHeader("Lab05 Malloc Failure Tests ");

    int max_heap = LL_measureHeap();
    printf("On initialization, heap has room for %d listitems.\n", max_heap);

    startSubtestRun("test_LinkedList_MallocFailureHandling");
    TestMallocFailure();
    endSubtestRun(1);

    startSubtestRun("test_LinkedList_NoFreedMemoryAccess");
    TestFreeAccess();
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
