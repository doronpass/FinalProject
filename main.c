#include <stdio.h>
#include <stdlib.h>
#include "Game.h"
#include "Game_board.h"

int main() {
    int value= 12;
    int is_fix = 1;
    int is_error =0;
    Cell *new_cell;
    new_cell = create_new_cell(value,is_fix,is_error);
//    print_cell(new_cell);
    printf("Hello, World!\n");

    return 0;
}