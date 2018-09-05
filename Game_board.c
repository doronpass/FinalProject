#include <stdio.h>
#include <stdlib.h>
#include "Game_board.h"
#include "Error_handler.h"
#include "Game.h"
#define PIPE "|"
Cell* create_new_cell(int value ,int is_fix, int is_error){
    Cell *new_cell;
    new_cell = (Cell*)malloc(sizeof(Cell));
    new_cell->is_error=is_error;
    new_cell->is_fix=is_fix;
    new_cell->value=value;
    return new_cell;
}

void print_cell(Cell *cell,int mode, int mark_error) {
    printf("%s", " ");
    if ((cell->value) == 0){
        printf("%s","  ");
    }
    else{
    printf("%2d",cell->value);}
    if (cell->is_fix && cell->value != 0) {
        printf("%s","."); }
    else if (cell->value!=0 && cell->is_error==1 && ((mode == 0) || (mark_error==1))){ /* mode 0 is edit mode*/
        printf("%s", "*");
    }
    else {
        printf(" "); /* mabye would be better do check if the value is 0 at start and then pring 4 spaces, to check if leagal*/
    }
}

/*allocates dynamic memory space for a game board based on size N*N */
Cell **create_new_board(int rows_size, int cols_size) {
    int size, i;
    size = rows_size * cols_size;
    Cell **arr = (Cell **) malloc(sizeof(Cell *) * size);
    check_memory2(arr);
    for (i = 0; i < size; ++i) {
        arr[i] = (Cell *) malloc(sizeof(Cell) * size);
        check_memory(arr[i]);
    }
    insert_zero_cells(arr, size);
    return arr;
}

void insert_zero_cells(Cell **arr, int size){
    int i, j;
    for (i = 0; i < size; ++i) {
        for (j = 0; j < size; ++j) {
            arr[i][j].is_fix = 0;
            arr[i][j].is_error = 0;
            arr[i][j].value = 0;
        }
    }
}

void print_separator_row(int size,int m) {
    int i;
    int num_of_dash=(size*4)+m+1;
    for (i = 0; i <num_of_dash ; ++i) {
        printf("-");
    }
    printf("\n");
}
void print_user_board(Game *game){
    int i, j, size;
    if (game == NULL){
        exit(1);
    }
    size = game->m_mult_n;
    for (i = 0; i < size; ++i) {
        if ((i % (game->n_block_cols)) == 0) {
            print_separator_row(size,game->n_block_cols);
        }
        for ( j = 0; j < size; ++j) {
            if ((j % (game->m_block_rows)) == 0) {
                printf(PIPE);
            }
            print_cell(&(game->user_game_board[i][j]), game->mode, game->mark_error);
        }
        printf("%s\n",PIPE);
    }
    print_separator_row(size,game->n_block_cols); /* there is an empty line after! check if it's fine */
}



void free_boards(Game *my_game){
    int i;
    for (i=0;i<my_game->m_mult_n;i++){
        free(my_game->user_game_board[i]);
        free(my_game->solved_game_board[i]);
    }
    free(my_game->user_game_board);
    free(my_game->solved_game_board);
}


