#include <stdio.h>
#include <stdlib.h>
#include "Game.h"
#include "Parser.h"

Game *create_new_game (Cell **user_game_board,Cell **solved_game_board,int mark_error,int mode,int m_block_rows, int n_block_cols){
    Game *new_game = (Game*)malloc(sizeof(Game));
    if (new_game == NULL){
        printf("Error: malloc has failed\n");
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

/*starts the game and let user type commands
 * first loop takes care if "init mode", inner loop takes care of the rest of the game
 * game_status:
 * 0 during the game
 * 1 when the game is over (so the user will return to "init mode" = in the first loop)
 * 2 when the user enters "exit" command to exit program */
void start_game(){
    int game_status = 0;
    Game *my_game = (Game *) malloc(sizeof(Game));
    if (my_game == NULL) {
        printf("Error: malloc has failed\n");
        exit(0);
    }
    while (1){
        if (game_status==2) {
            break;
        }
        game_status = init_user_turn(my_game);
        if (game_status==2) {
            break;
        }
        while(1){
            game_status = user_turn(my_game);
            if (game_status==1 || game_status == 2){
                break;
            }
        }
    }
}
