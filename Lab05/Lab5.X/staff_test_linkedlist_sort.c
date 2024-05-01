/*Because there are so many ways for students to crash the test code in this lab, 
 * the staff test harness is spread into several different harnesses.
 * 
 * This harness tests whether the student's insertion and selection sorts produce correct
 * behavior.
 * 
 * Note that this harness relies on some weird compiler magic, so its .o file
 * cannot be used directly to test student code.   
 * See the comments for main() below.
 * (It works when compiled in this project configuration though)
 * 
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

ListItem *InsertionSort(ListItem* list);
ListItem *SelectionSort(ListItem* list);

//local list macros:
#define N_UNSORTED_WORDS 22

#define UNSORTED_WORDS "bored", \
"royal", \
"lunchroom", \
"yoke", \
"weak", \
"ghost", \
"aloof", \
"godly", \
"puffy", \
"shut", \
"account", \
"cook", \
"devilish", \
"tart", \
"annoyed", \
"obeisant", \
"broad", \
"tendency", \
"expand", \
"cherry", \
"dogs", \
"hole"

#define SORTED_WORDS "account",\
"aloof", \
"annoyed", \
"bored", \
"broad", \
"cherry", \
"cook", \
"devilish", \
"dogs", \
"expand", \
"ghost", \
"godly", \
"hole", \
"lunchroom", \
"obeisant", \
"puffy", \
"royal", \
"shut", \
"tart", \
"tendency", \
"weak", \
"yoke"

//prototypes to be included from sort.c

void TestLLSelectionSort(void)
{

    g_printf("   Sorting items with selection sort:\n");
    {
        //this is weird, but make sure you create "resultBase" AFTER "expected"
        //otherwise, the "resultBase = Sort(resultBase)" line can assign "resultBase "
        // to "expected" in the event that the student's Sort() doesn't return anything.
        ListItem * expected = LL_CreateList(N_UNSORTED_WORDS, SORTED_WORDS);
        ListItem * resultBase = LL_CreateList(N_UNSORTED_WORDS, UNSORTED_WORDS);
//        LinkedListPrint(resultBase);
//        LinkedListPrint(expected);
        resultBase = SelectionSort(resultBase);
//        LinkedListPrint(resultBase);
        subtestResult(LL_CompareLists(resultBase, expected), "selectionSort");

        LL_FreeList(expected);
        LL_FreeList(resultBase);

    }
}

void TestLLInsertionSort(void)
{

    g_printf("   Sorting items with insertion sort:\n");
    {
        //this is weird, but make sure you create "resultBase" AFTER "expected"
        //otherwise, the "resultBase = Sort(resultBase)" line can assign "resultBase "
        // to "expected" in the event that the student's Sort() doesn't return anything.
        ListItem * expected = LL_CreateList(N_UNSORTED_WORDS, SORTED_WORDS);
        ListItem * resultBase = LL_CreateList(N_UNSORTED_WORDS, UNSORTED_WORDS);
        resultBase = InsertionSort(resultBase);
        subtestResult(LL_CompareLists(resultBase, expected), "insertionSort");

        LL_FreeList(expected);
        LL_FreeList(resultBase);

    }
}


/*Since sort.c is a main file, as the students turn it in, we have to use a sneaky
 * macro technique to "deselect" their main.  In the configuration staff_LL_test_sort,
 * a compiler-flag level #define redefines "main" as STUDENT_MAIN.  This is set in 
 * project properties -> xc32-gcc -> preprocessing and messages -> preprocessor macros -> 
 *  "main=STUDENT_MAIN"
 *  Locally, in this file, that flag is undefined just long enough
 *  to give this file the ability to contain
 * the unmodified name "main".
 * 
 * NOTE: This means that you can't use the .o of this file as a test harness for 
 * a student's sort functions, since the linker can't do this "deselection"
 */

#undef main
int main()
#define main STUDENT_MAIN
{
    BOARD_Init();

    printHeader("Lab05 Sort tests ");

    int max_heap = LL_measureHeap();
    printf("On initialization, heap has room for %d listitems.\n", max_heap);


    startSubtestRun("test_Sort_SelectionSort");
    TestLLSelectionSort();
    endSubtestRun(1);

    startSubtestRun("test_Sort_InsertionSort");
    TestLLInsertionSort();
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