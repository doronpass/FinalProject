#ifndef FINALPROJECT_PLAY_H
#define FINALPROJECT_PLAY_H
#include "Game_board.h"

void undo_print(Data *data);
void redo_print(Data *data);

void hint(Game *game,int row,int cols);
void generate (Game *game,Node *node, int x,int y);
void validate(Game *game);

void clear_board(Game *game);
int num_of_empty_cells(Game *game);
int num_not_valid(int empty_cells,int x);
int get_legal_random_val(Game *game,int row,int col);
int create_valid_arr(Game *game,int *valid_arr, int row , int col,int N) ;
int get_the_order_number_from_arr(int order, int *valid_arr);


int count_invalid_numbers(Game *game);
void copy_solve_2_user(Game *game);

#endif

