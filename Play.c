#include <stdio.h>
#include "Play.h"
#include "Game.h"
#include "Error_handler.h"
#include <string.h>

void undo_print(Data *data) {
    int x = data->row;
    int y = data->row;
    int value = data->value;
    int prev_value = data->prev_value;
    if (value != 0 && prev_value != 0) {
        printf("Undo %d,%d :from %d to %d\n", x, y, prev_value, value);
    } else if (value != 0 && prev_value == 0) {
        printf("Undo %d,%d :from _ to %d\n", x, y, value);
    } else if (value == 0 && prev_value == 0) {
        printf("Undo %d,%d :from _ to _\n", x, y);
    } else {
        printf("Undo %d,%d :from %d to _\n", x, y, prev_value);
    }
}

void redo_print(Data *data) {
    int x = data->row;
    int y = data->row;
    int value = data->value;
    int prev_value = data->prev_value;
    if (value != 0 && prev_value != 0) {
        printf("Redo %d,%d :from %d to %d\n", x, y, prev_value, value);
    } else{
        if (value != 0 && prev_value == 0) {
            printf("Redo %d,%d :from _ to %d\n", x, y, value);
        }
        else{
            if (value == 0 && prev_value == 0) {
                printf("Redo %d,%d :from _ to _\n", x, y);
            }
            else {
                printf("Redo %d,%d :from %d to _\n", x, y,prev_value);
            }
        }
    }
}