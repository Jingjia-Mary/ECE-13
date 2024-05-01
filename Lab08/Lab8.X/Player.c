/* 
 * File:   Player.c
 * Author: Mary Xu
 *
 * Created on May 17, 2022, 6:35 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "UNIXBOARD.h"
#include "Player.h"

static int inventory[INVENTORY_SIZE];
static int count = 0;

/**
 * Adds the specified item to the player's inventory if the inventory isn't full.
 * @param item The item number to be stored: valid values are 0-255.
 * @return SUCCESS if the item was added, STANDARD_ERRROR if the item couldn't be added.
 */
int AddToInventory(uint8_t item) {// add the item to it if the text is in the array
    if (count < INVENTORY_SIZE) {
        inventory[count] = item;
        return SUCCESS;
    }
    return STANDARD_ERROR;
}

/**
 * Check if the given item exists in the player's inventory.
 * @param item The number of the item to be searched for: valid values are 0-255.
 * @return SUCCESS if it was found or STANDARD_ERROR if it wasn't.
 */
int FindInInventory(uint8_t item) {
    if (count == 0) { //set the count error
        return STANDARD_ERROR;
    }
    if (item > 255 || item < 0) { //give the space
        return STANDARD_ERROR;
    }
    for (int i = 0; i < count; i++) {
        if (inventory[i] == item) {
            return SUCCESS;
        }
    }
    return STANDARD_ERROR;
}