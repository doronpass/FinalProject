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

