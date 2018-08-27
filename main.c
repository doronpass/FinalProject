#include <stdio.h>
#include <stdlib.h>
#include "Game.h"
#include "Game_board.h"
#include "Functionality.h"

int main() {
    Game *new_game;
    new_game = init_game("solve", NULL);

    printf("errors: %d, mode %d, m=%d, n=%d, N=%d\n",new_game->mark_error, new_game->mode , new_game->m_block_rows , new_game->n_block_cols, new_game->m_mult_n);
    new_game->user_game_board[0][0].value = 1;
    new_game->user_game_board[1][1].value = 2;
    new_game->user_game_board[1][2].value = 3;
    new_game->user_game_board[2][2].value = 4;
    new_game->user_game_board[2][2].is_fix = 1;
    save_game(new_game, "C:\\Users\\itai zur\\Desktop\\project3\\test.txt");
    /*
    printf("errors: %d, mode %d, m=%d, n=%d, N=%d\n",new_game->mark_error, new_game->mode , new_game->m_block_rows , new_game->n_block_cols, new_game->m_mult_n);
    printf("%d, %d, %d, %d\n", new_game->user_game_board[0][0].value,  new_game->user_game_board[1][1].value,  new_game->user_game_board[2][1].value,  new_game->user_game_board[2][2].value);
    printf("2,2: %d, other: %d\n",new_game->user_game_board[2][2].is_fix, new_game->user_game_board[0][0].is_fix);
    */
     return 0;

}

