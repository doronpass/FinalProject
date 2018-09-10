#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Game.h"
#include "Game_board.h"
#include "Functionality.h"

int main() {
    srand(time(NULL));
    start_game();
    return 0;
}

//    printf("node_data = (%d,%d) val - %d\n", node_to_undo->node_data[0]->row, node_to_undo->node_data[0]->col, node_to_undo->node_data[0]->prev_value); /* ---------------------- for testing!! */