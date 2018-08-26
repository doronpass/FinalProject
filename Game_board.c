//
// Created by Doron_Passal on 24/08/2018.
//
#include <stdio.h>
#include "Game_board.h"

/*tests whether calloc failed
 * if so, alert the user and terminate program*/
void check_memory(Cell* point){
    if (point == NULL) {
        printf("Error: calloc has failed\n");
        exit(0);
    }
}

/*tests whether calloc failed
 * if so, alert the user and terminate program*/
void check_memory2(Cell** point){
    if (point == NULL) {
        printf("Error: calloc has failed\n");
        exit(0);
    }


/*allocates dynamic memory space for a game board based on size N*N */
    Cell** create_new_board(Game *game){
        int i;
        Cell** arr =(Cell**)calloc(game->m_mult_n,sizeof(*Cell));
        check_memory2(arr);
        for (i=0; i<game->m_mult_n; i++){
            arr[i] = (Cell*) calloc (game.m_mult_n,sizeof(cell));
            check_memory(arr[i]);
        }
        return arr;
    }
    void print_board(Game game) {

    }
