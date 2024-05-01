#include <stdlib.h>
#include <stdio.h>
#include "linkedlist.h"
#include "BOARD.h"


ListItem *LinkedListNew(char *data) {
    ListItem *item = malloc(sizeof(ListItem));
    if (item == NULL) {
        return NULL;
    }
    item->previousItem = NULL;
    item->nextItem = NULL;
    item->data = data;
    return item;
}

ListItem *LinkedListCreateAfter(ListItem *item, char *data) {
    ListItem *newItem = (ListItem*) malloc(sizeof(ListItem));
    if (newItem == NULL) {
        return NULL;
    }

    newItem->data = data;
    newItem->previousItem = item;
    if (item != NULL) {
        newItem->nextItem = item->nextItem;
        if (item->nextItem != NULL) {
            item->nextItem->previousItem = newItem;
        }
        item->nextItem = newItem;
    } else {
        newItem->nextItem = NULL;
    }

   return newItem;
}


char *LinkedListRemove(ListItem *item) {
    if (item == NULL) {
        return NULL;
    }
    
    char *data = item->data;
    ListItem *previousItem = item->previousItem;
    ListItem *nextItem = item->nextItem;
    
    if (previousItem != NULL) {
        previousItem->nextItem = nextItem;
    }
    
    if (nextItem != NULL) {
        nextItem->previousItem = previousItem;
    }
    
    free(item);
    
    return data;
}

int LinkedListSize(ListItem *list) {
    int count = 0;

    while (list != NULL) {
        count++;
        list = list->nextItem;
    }

    return count;
}

ListItem *LinkedListGetFirst(ListItem *list) {
    if (list == NULL) {  // Return NULL for invalid input
        return NULL;
    }

    // Traverse the list to find the head
    while (list->previousItem != NULL) {
        list = list->previousItem;
    }

    return list;
}

ListItem *LinkedListGetLast(ListItem *list) {
    if (list == NULL) {
        return NULL;
    }
    while (list->nextItem != NULL) {
        list = list->nextItem;
    }
    return list;
}

int LinkedListSwapData(ListItem *firstItem, ListItem *secondItem) {
    if (firstItem == NULL || secondItem == NULL) {
        return EXIT_FAILURE;
    }

    void *temp = firstItem->data;
    firstItem->data = secondItem->data;
    secondItem->data = temp;

    return SUCCESS;
}

int LinkedListPrint(ListItem *list) {
    // Check for NULL list
    if (list == NULL) {
        return STANDARD_ERROR;
    }

    // Traverse the list, printing each data item
    printf("[");
    while (list != NULL) {
        if (list->data != NULL) {
            printf("%s", list->data);
        }
        if (list->nextItem != NULL) {
            printf(", ");
        }
        list = list->nextItem;
    }
    printf("]\n");

    return SUCCESS;
}