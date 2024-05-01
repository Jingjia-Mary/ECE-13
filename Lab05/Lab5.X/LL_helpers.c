/* LL_helpers are functions that are handy while doing auto-tests on Lab5.
 * No functions here rely on student code!
 *  
 * Author:  Max L
 * Date:  8/5/2018
 */

//**** Module-level variables *************************


/*Note: this test suite uses a sneaky trick to detect when students 
 * attempt to access free()d memory.  The project properties for 
 * staff_test_malloc_fail redefines "free" as "wipe_free", which you can
 * see is defined below.  Of course, for this to work, we have to call 
 * the REAL free() inside this file...so, here, we undo the project-level macro. */
#undef free

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
//#include <ctype.h>

//CMPE13 Support Library
#include "BOARD.h"

// User libraries
#include "LL_helpers.h"  //<< BE SURE TO INCLUDE THIS BEFORE LinkedList.h!!!
#include "LinkedList.h"

// chain_tail is passed from fillHeap to clearHeap so clearHeap knows where to begin free()ing
ListItem * chain_tail;

int LL_fillHeap(void)
{
    ListItem * chain_temp;
    int i = 0;
    chain_tail = malloc(sizeof (ListItem));
    if (chain_tail != NULL) chain_tail->previousItem = NULL;
    while (1) {
        i++;
        chain_temp = malloc(sizeof (ListItem));
        if (chain_temp == NULL) return i;
        chain_temp->previousItem = chain_tail;
        chain_temp->data = (char *) 0xDEADBEEF;
        chain_tail = chain_temp;
    }
}

int LL_clearHeap(void)
{
    int i = 0;
    ListItem * chain_temp;
    while (chain_tail->previousItem != NULL) {
        i++;
        chain_temp = chain_tail->previousItem;
        free(chain_tail);
        chain_tail = chain_temp;
    }
    if (chain_tail != NULL) {
        i++;
        free(chain_tail);
    }
    return i;
}

int LL_measureHeap(void)
{
    LL_fillHeap();
    return LL_clearHeap();
}

//basically just a helper function for LL_CreateList

ListItem* LL_CreateAfter(ListItem *list, char *data)
{
    ListItem *new_li = calloc(1, sizeof (ListItem));
    if (new_li == NULL) {
        return NULL;
    }
    // Set up the new list item
    new_li->previousItem = list;
    new_li->data = data;
    new_li->nextItem = NULL;
    if (list) {
        if (list->nextItem) {
            new_li->nextItem = list->nextItem;
            new_li->nextItem->previousItem = new_li;
        }
        list->nextItem = new_li;
    }
    return new_li;
}

ListItem* LL_CreateList(int n_items, ...)
{
    int orig_heap = LL_measureHeap();
    int i;

    //start vararg reader:
    va_list arg_ptr;
    va_start(arg_ptr, n_items);

    //add in items:
    ListItem* tail_ptr = NULL;
    for (i = 0; i < n_items; i++) {
        tail_ptr = LL_CreateAfter(tail_ptr, va_arg(arg_ptr, char*));
        if (!tail_ptr) {
            printf("Error:  Test script ran out of memory!!!\n");
            while (1);
        }
    }
    //close out VA macro:
    va_end(arg_ptr);

    //do some error checking:
    if (!(LL_measureHeap() == orig_heap - n_items)) {
        printf("Error:  LL_CreateList used too much memory");
        while (1);
    }
    tail_ptr = LL_VerifyList(tail_ptr);
    if (tail_ptr == NULL) {
        printf("Error:  LL_CreateList generated a faulty list");
        while (1);
    }

    return tail_ptr;
}

//basically just a helper for LL_FreeList

char *LL_Remove(ListItem *item)
{
    if (item == NULL) {
        return NULL;
    }
    char *returnData = item->data;
    if (item->previousItem) {
        item->previousItem->nextItem = item->nextItem;
    }
    if (item->nextItem) {
        item->nextItem->previousItem = item->previousItem;
    }
    free(item);
    return returnData;
}

void LL_FreeList(ListItem* li)
{
    li = LL_VerifyList(li);
    while(li){
        ListItem* next = li->nextItem;
        LL_Remove(li);
        li = next;
    }
}

ListItem* LL_VerifyList(ListItem* li)
{
    if (!li) return NULL;

    //first, get the first item:
    while (li->previousItem) {
        li = li->previousItem;
    }
    //now, count its length
    int len = 0;
    while (li->nextItem) {
        li = li->nextItem;
        len++;
    }
    //come back:
    while (li->previousItem) {
        li = li->previousItem;
        len--;
    }
    if (len != 0) return NULL;
    return li;
}

int LL_CompareLists(ListItem* A, ListItem* B)
{
    A = LL_VerifyList(A);
    B = LL_VerifyList(B); //presumably this works, if not there are much bigger issues!
    if (!(A && B)) return FALSE;

    //Now, iterate over the string:
    while (1) {
        //first, check data elements for each LI
        //        if( (A->data == NULL) && (B->data == NULL) ); do nothing, this is fine
        if ((A->data == NULL) && (B->data != NULL)) return FALSE;
        if ((A->data != NULL) && (B->data == NULL)) return FALSE;
        if ((A->data != NULL) && (B->data != NULL)) {
            if (strcmp(A->data, B->data)) return FALSE; //strings don't match
        }
        //next, we advance both pointers:
        A = A->nextItem;
        B = B->nextItem;
        if ((A == NULL) && (B == NULL)) return TRUE;
        if ((A == NULL) && (B != NULL)) return FALSE;
        if ((A != NULL) && (B == NULL)) return FALSE;
        //        if( (A != NULL) && (B != NULL));  do nothing, this is fine
    }
}



void wipe_free(void* ptr)
{
//BADDADD is just a bad address that is human readable for certain error messages
    //memset(ptr, 0xBADADD, sizeof(ListItem)); //this doesn't play well with our macro trickery
    ListItem * LI = (ListItem*) ptr;
    LI->previousItem = (ListItem*) 0xBADADD;
    LI->nextItem = (ListItem*) 0xBADADD;
    LI->data = (char*) 0xBADADD;
    free(ptr);
}

int LL_Print(ListItem *list)
{
    if (list) {
        printf("      ~~{");
        for (list = LinkedListGetFirst(list); list; list = list->nextItem) {
            if (list->nextItem) {
                printf("%s, ", list->data);
            } else {
                printf("%s", list->data);
            }
        }
        printf("}~~\n");
        return SUCCESS;
    } else {
        return STANDARD_ERROR;
    }
}

/* Test code for the above helpers*/
void test_helpers(void)
{

    ListItem* newList1 = LL_CreateList(2, "Hello", "World");
    ListItem* newList2 = LL_CreateList(2, "Hello", "World");
    ListItem* newList3 = LL_CreateList(2, "Hello", "World2");
    ListItem* newList4 = LL_CreateList(3, "Hello", "World", "etc");
    LL_Print(newList1);
    LL_Print(newList2);
    LL_Print(newList3);
    LL_Print(newList4);
    printf("1 == 2 (should be TRUE) : %x\n", LL_CompareLists(newList1, newList2));
    printf("1 == 3 (should be FALSE): %x\n", LL_CompareLists(newList1, newList3));
    printf("1 == 4 (should be FALSE): %x\n", LL_CompareLists(newList1, newList4));

}

