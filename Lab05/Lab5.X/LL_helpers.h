
#ifndef LL_HELPERS_H
#define LL_HELPERS_H

/* LL_helpers are functions that are handy while doing auto-tests on Lab5.
 * No functions here rely on student code!
 *  
 * //<  BE SURE TO INCLUDE THIS BEFORE LinkedList.h!!!  >
 * 
 * Author:  Max L
 * Date:  8/5/2018
 */

#include "LinkedList.h"


/* fillHeap and clearHeap are used by measureHeap to measure how much space is left in
 * the heap, in terms of the number of LI's that can fit into the heap.  You can also
 * use fillHeap to indice malloc() failure.
 * 
 * fillHeap fills up the heap with ListItems until malloc fails, basically creating one
 * huge linked list.  It returns the number of LIs that it managed to occupy, and stores
 * the tail's address.
 * 
 * clearHeap undoes the action of fillHeap, freeing one item at a time until the list is free()d.
 * It returns the number of LIs that it free()d.
 *
 * measureHeap calls fillHeap and clearHeap, then returns the value that fillHeap measured.
 */
int LL_fillHeap(void);
int LL_clearHeap(void);
int LL_measureHeap(void);



/* Create a LL from a list of strings. Returns a pointer to the first one.
 * Since the whole thing is malloc'd at once, you can free it all at once.
 * (though maybe it would be better to use a static array, to isolate it
 * from student code errors?)
 * 
 * called like this: 
 * ListItem* newList = CreateList(2,"Hello","World");
 * free(newList);  
 * 
 * This list can be modified just like any other LL.  
 */
ListItem* LL_CreateList(int n_items, ...);

/* LL_FreeList (attempts to) clear out a whole list. */
void LL_FreeList(ListItem* li);

/* Validate a list (make sure it runs forward and backward, and get the head element). 
 * This is useful because it provides some protection against script crashes (you can 
 * check its output before a test, and skip the test if it fails)
 * 
 * It returns the head element if the list is valid, and NULL otherwise.
 */
ListItem* LL_VerifyList(ListItem* li);

/* Check two lists to see if they match.
 * Two lists are considered identical if:
 *      -they have the same number of items
 *      -they are both valid lists, as measured by VerifyList()
 *      -the i'th data * in each list is identical:  
 *          -Either both are NULL, or both point to identical strings as measured by strcmp()
 * 
 * Return TRUE if they match and FALSE otherwise.
 * */
int LL_CompareLists(ListItem* A, ListItem* B);


/* A correct implementation of LinkedListPrint()*/
int LL_Print(ListItem *list);

/* this function is meant to replace free() in some tests via #define.  
 * It forces free()d data to be unaccessable, and can be used by adding
 * "free=wipe_free" 
 * to the list of preprocessor macros
 */
void wipe_free(void*ptr);

/* Test code for the above helpers*/
void test_helpers(void);

#endif //LL_HELPERS_H