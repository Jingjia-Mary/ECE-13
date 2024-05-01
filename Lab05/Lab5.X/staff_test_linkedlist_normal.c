/*Because there are so many ways for students to crash the test code in this lab, 
 * the staff test harness is spread into several different harnesses.
 * 
 * This harness tests linked list behavior under "normal" conditions - ie, when nothing goes wrong.
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

void TestLLNew(void)
{
    g_printf("Create single item:\n");
    ListItem* result = LinkedListNew("pig");
    ListItem* expected = LL_CreateList(1, "pig");
    subtestResult(LL_CompareLists(expected, result), "New0");

    LL_FreeList(result);
    LL_FreeList(expected);
}

void TestLLCreateAfter(void)
{
    g_printf("Adding to solo item:\n");
    {
        ListItem * resultBase = LL_CreateList(1, "ONE");
        ListItem* expected = LL_CreateList(2, "ONE", "TWO");
        ListItem * result = LinkedListCreateAfter(resultBase, "TWO");
        subtestResult(LL_CompareLists(expected, result), "CreateAfter0");

        LL_FreeList(result);
        LL_FreeList(expected);
        LL_FreeList(resultBase);

    }

    g_printf("Creating new item at tail:\n");
    {
        ListItem * resultBase = LL_CreateList(2, "ONE", "TWO");
        ListItem* expected = LL_CreateList(3, "ONE", "TWO", "THREE");
        ListItem * result = LinkedListCreateAfter(resultBase->nextItem, "THREE");
        subtestResult(LL_CompareLists(expected, result), "CreateAfter1");

        LL_FreeList(result);
        LL_FreeList(expected);
        LL_FreeList(resultBase);

    }

    g_printf("Creating new item in center:\n");
    {
        ListItem * resultBase = LL_CreateList(2, "ONE", "THREE");
        ListItem* expected = LL_CreateList(3, "ONE", "TWO", "THREE");
        ListItem * result = LinkedListCreateAfter(resultBase, "TWO");
        subtestResult(LL_CompareLists(expected, result), "CreateAfter2");

        LL_FreeList(result);
        LL_FreeList(expected);
        LL_FreeList(resultBase);
    }
}

void TestLLSize(void)
{
    g_printf("One item:\n");
    {
        ListItem * test = LL_CreateList(1, "ONE");
        int size = LinkedListSize(test);
        subtestResult(size == 1, "Size0");
        LL_FreeList(test);
    }

    g_printf("Three items, starting at head:\n");
    {
        ListItem * test = LL_CreateList(3, "ONE", "TWO", "THREE");
        int size = LinkedListSize(test);
        subtestResult(size == 3, "Size1");
        LL_FreeList(test);
    }

    g_printf("Three items, starting at tail:\n");
    {
        ListItem * test = LL_CreateList(3, "ONE", "TWO", "THREE");
        int size = LinkedListSize(test->nextItem->nextItem);
        subtestResult(size == 3, "Size2");
        LL_FreeList(test);
    }
}

void TestLLRemove(void)
{
    g_printf("Removing Middle of List:\n");
    {
        ListItem * resultBase = LL_CreateList(3, "ALPHA", "BRAVO", "CHARLIE");
        char * result = LinkedListRemove(resultBase->nextItem);
        subtestResult(!strcmp(result, "BRAVO"), "ret_Remove0");

        ListItem* expected = LL_CreateList(2, "ALPHA", "CHARLIE");
        subtestResult(LL_CompareLists(expected, resultBase), "list_Remove0");

        LL_FreeList(expected);
        LL_FreeList(resultBase);
    }

    g_printf("Removing head of List:\n");
    {
        ListItem * resultBase = LL_CreateList(3, "ALPHA", "BRAVO", "CHARLIE");
        ListItem * new_head = resultBase->nextItem;
        char * result = LinkedListRemove(resultBase);
        subtestResult(!strcmp(result, "ALPHA"), "ret_Remove1");

        ListItem* expected = LL_CreateList(2, "BRAVO", "CHARLIE");
        subtestResult(LL_CompareLists(expected, new_head), "list_Remove1");

        LL_FreeList(expected);
        LL_FreeList(new_head);
    }

    g_printf("Removing tail of List:\n");
    {
        ListItem * resultBase = LL_CreateList(3, "ALPHA", "BRAVO", "CHARLIE");
        char * result = LinkedListRemove(resultBase->nextItem->nextItem);
        subtestResult(!strcmp(result, "CHARLIE"), "ret_Remove2");

        ListItem* expected = LL_CreateList(2, "ALPHA", "BRAVO");
        subtestResult(LL_CompareLists(expected, resultBase), "list_Remove2");

        LL_FreeList(expected);
        LL_FreeList(resultBase);
    }

    g_printf("Removing only item in List:\n");
    {
        ListItem * resultBase = LL_CreateList(1, "SOLO");
        char * result = LinkedListRemove(resultBase);
        subtestResult(!strcmp(result, "SOLO"), "ret_Remove3");

        LL_FreeList(resultBase);
    }
}

void TestLLGetFirst(void)
{
    ListItem * listHead = LL_CreateList(3, "Apple", "Banana", "Cookie");

    g_printf("Testing on tail of list:\n");
    ListItem * result = LinkedListGetFirst(listHead->nextItem->nextItem);
    subtestResult(listHead == result, "getFirst0");

    g_printf("Testing on head of list:\n");
    result = LinkedListGetFirst(listHead);
    subtestResult(listHead == result, "getFirst1");

    LL_FreeList(listHead);
}

void TestLLGetLast(void)
{
    ListItem * listHead = LL_CreateList(3, "Apple", "Banana", "Cookie");

    g_printf("Testing on tail of list:\n");
    ListItem * result = LinkedListGetLast(listHead->nextItem->nextItem);
    subtestResult(listHead->nextItem->nextItem == result, "getLast0");

    g_printf("Testing on head of list:\n");
    result = LinkedListGetLast(listHead);
    subtestResult(listHead->nextItem->nextItem == result, "getLast1");

    LL_FreeList(listHead);
}

void TestLLSwapData(void)
{
    g_printf("Swapping first and last:\n");
    {
        ListItem * resultBase = LL_CreateList(3, "A", "B", "C");
        ListItem * expected = LL_CreateList(3, "C", "B", "A");
        LinkedListSwapData(resultBase, resultBase->nextItem->nextItem);
        subtestResult(LL_CompareLists(resultBase, expected), "swap0");

        LL_FreeList(expected);
        LL_FreeList(resultBase);
    }
    g_printf("Swapping middle and last:\n");
    {
        ListItem * resultBase = LL_CreateList(3, "A", "B", "C");
        ListItem * expected = LL_CreateList(3, "A", "C", "B");
        LinkedListSwapData(resultBase->nextItem, resultBase->nextItem->nextItem);
        subtestResult(LL_CompareLists(resultBase, expected), "swap1");

        LL_FreeList(expected);
        LL_FreeList(resultBase);
    }
    g_printf("Swapping first and middle:\n");
    {
        ListItem * resultBase = LL_CreateList(3, "A", "B", "C");
        ListItem * expected = LL_CreateList(3, "B", "A", "C");
        LinkedListSwapData(resultBase->nextItem, resultBase);
        subtestResult(LL_CompareLists(resultBase, expected), "swap2");

        LL_FreeList(expected);
        LL_FreeList(resultBase);
    }
}


void TestLLPrint(void)
{
    printf("Printing one item:\n");
    {
        ListItem * TestOne = LL_CreateList(1, "ONE");
        LinkedListPrint(TestOne);
        LL_FreeList(TestOne);
    }

    printf("Printing three items, starting at head:\n");
    {
        ListItem * TestOne = LL_CreateList(3, "ONE", "TWO", "THREE");
        LinkedListPrint(TestOne);
        LL_FreeList(TestOne);
    }

    printf("Printing three items, starting from tail:\n");
    {
        ListItem * TestOne = LL_CreateList(3, "ONE", "TWO", "THREE");
        LinkedListPrint(TestOne->nextItem->nextItem);
        LL_FreeList(TestOne);
    }

    printf("Printing including NULL data:\n");
    {
        ListItem * TestOne = LL_CreateList(3, "ONE", NULL, "THREE");
        LinkedListPrint(TestOne->nextItem->nextItem);
        LL_FreeList(TestOne);
    }
}

int main()
{
    BOARD_Init();

    printHeader("Lab05 Linked List Tests under normal conditions ");

    int max_heap = LL_measureHeap();
    printf("On initialization, heap has room for %d listitems.\n", max_heap);

    startSubtestRun("test_LinkedListNew");
    TestLLNew();
    endSubtestRun(0.5);

    startSubtestRun("test_LinkedListCreateAfter");
    TestLLCreateAfter();
    endSubtestRun(0.5);

    startSubtestRun("test_LinkedListSize");
    TestLLSize();
    endSubtestRun(0.5);

    startSubtestRun("test_LinkedListGetFirst");
    TestLLGetFirst();
    endSubtestRun(0.5);
    
    startSubtestRun("test_LinkedListGetLast");
    TestLLGetLast();
    endSubtestRun(0.5);

    startSubtestRun("test_LinkedListSwapData");
    TestLLSwapData();
    endSubtestRun(0.5);

    startSubtestRun("test_LinkedListRemove");
    TestLLRemove();
    endSubtestRun(0.5);
    
    printf("Testing LinkedListPrint:\n");
    TestLLPrint();
//    endSubtestRun(0.5);

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
