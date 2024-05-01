/* 
 * File:   LinkedListTest.c
 * Author: maryx
 *
 * Created on April 25, 2022, 6:06 PM
 */

#include <stdio.h>
#include <stdlib.h>

#include "BOARD.h"
#include "LinkedList.h"

/*
 * 
 */
int main() {
    BOARD_Init();
    ListItem *x = LinkedListNew("Y");
    LinkedListCreateAfter(x, "Y");
    printf("%s\n", x -> nextItem -> data);
    LinkedListCreateAfter(x, "Z");
    LinkedListPrint(x);
    BOARD_End();
}

