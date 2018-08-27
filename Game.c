//
// Created by Doron_Passal on 24/08/2018.
//

#include <stdio.h>
#include <stdlib.h>
#include "Game.h"

Game *create_new_game (Cell **user_game_board,Cell **solved_game_board,int mark_error,int mode,int m_block_rows, int n_block_cols,Doubly_linked_list *dll){
    Game *new_game = (Game*)malloc(sizeof(Game));
    if (new_game == NULL){
        printf("Error: calloc has failed\n");
        exit(0);
    }
    new_game->n_block_cols = n_block_cols;
    new_game->m_block_rows = m_block_rows;
    new_game->m_mult_n= n_block_cols*m_block_rows;
    new_game->user_game_board = user_game_board;
    new_game->solved_game_board = solved_game_board;
    new_game->mode = mode;
    new_game->mark_error = mark_error;
    new_game->doubly_linked_list = create_new_dll();
    return new_game;
}

