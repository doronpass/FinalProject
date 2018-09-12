#ifndef FINALPROJECT_GAME_BOARD_H
#define FINALPROJECT_GAME_BOARD_H

typedef struct {
    int value;
    int is_fix;
    int is_error;
} Cell;

typedef struct Game Game;

Cell** create_new_board(int rows_size , int cols_size);
Cell* create_new_cell(int value ,int is_fix, int is_error);
void print_cell(Cell *cell,int mode, int mark_error);
void insert_zero_cells(Cell **arr,int size);
void print_separator_row(int size,int m);
void print_user_board(Game *game);
void free_boards(Game *my_game);
int** create_matrix(int N);
void copy_board_to_game(int **result_arr,int N,Game *game);
void copy_sol_to_board(double *sol,int **result_arr, int N ) ;

#endif
