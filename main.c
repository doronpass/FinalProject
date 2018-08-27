#include <stdio.h>
#include <stdlib.h>
#include "Game.h"
#include "Game_board.h"
#include "Functionality.h"

int main() {
    Game *new_game;
    Cell **arr;
    int i;
    arr = create_new_board(5,3);
    new_game = create_new_game(arr,arr,0,0,3,5);
    for ( i = 0; i < new_game->m_mult_n; ++i) {
        new_game->user_game_board[i][i].value = 9;

    }
    new_game->mode = 1;
    print_user_board(new_game);

     return 0;

}

