#ifndef FINALPROJECT_GAME_BOARD_H
#define FINALPROJECT_GAME_BOARD_H
#include "Dll.h"

typedef struct {
    int value;
    int is_fix;
    int is_error;
} Cell;

typedef struct Game
{
    Cell **user_game_board;
    Cell **solved_game_board;
    int mark_error;/* 0 do not mark, 1 do mark */
    int mode; /* 0 if edit , 1 if solve */
    int m_block_rows;
    int n_block_cols;
    int m_mult_n;
    Doubly_linked_list *doubly_linked_list;
} Game;


Cell** create_new_board(int rows_size , int cols_size);
void print_cell(Cell *cell,int mode, int mark_error);
void insert_zero_cells(Cell **arr,int size);
void print_separator_row(int size,int m);
void print_user_board(Game *game);
void free_boards(Game *my_game);
int** create_matrix(int N);
void copy_board_to_game(int **result_arr,int N,Game *game);
void copy_sol_to_board(double *sol,int **result_arr, int N ) ;

void start_game();

#endif
