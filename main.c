#include <stdio.h>
#include <stdlib.h>
#include "Game.h"
#include "Game_board.h"

int main() {
    Cell *new_cell;
    new_cell = create_new_cell(23,1,0);
    print_cell(new_cell);
    printf("\nHello, World!\n");

    return 0;
}