#include <stdio.h>
#include "Play.h"
#include "Game.h"
#include "Error_handler.h"
#include <string.h>

void undo_print(Data data) {
    int x = data.row;
    int y = data.row;
    int value = data.value;
    int prev_value = data.prev_value;
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

void redo_print(Data data) {
    int x = data.row;
    int y = data.row;
    int value = data.value;
    int prev_value = data.prev_value;
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

void do_generate(Game *game,int x, int y){
    int empty_cells = 0;
    empty_cells = num_of_empty_cells(game);
    if (game->mode != 0) /* if the mode is not edit print invalid command */{
        invalid_command();
        /* should we exit after invalid command?! */
    }
    else if (num_not_valid(empty_cells,x) || num_not_valid(empty_cells,y){
        not_in_range(empty_cells);
    }



}

