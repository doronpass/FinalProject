#include "Dll.h"
#include "Game_board.h"

#ifndef FINALPROJECT_FUNCTIONALITY_H
#define FINALPROJECT_FUNCTIONALITY_H

int save_to_file(Game *my_game, char *path);
int has_erroneous_values(Game *my_game);
int load_from_file(Game *my_game, char *path);
int is_valid(Game *my_game,int x, int y, int z);
void mark_error_cells(Game *my_game);
void mark_erroneous_after_change(Game *my_game, int x, int y, int z);
void unmark_erroneous_before_change(Game *my_game, int x, int y, int z);
Game * clone_game(Game *my_game);
void set_without_dll(Game *my_game, int x, int y, int z);
void undo_without_output(Game *my_game);
void free_all_mem(Game *my_game);
void undo_print(Data *data);
void redo_print(Data *data);
void clear_board(Game *game);
int num_of_empty_cells(Game *game);
int num_not_valid(int empty_cells,int x);
int get_legal_random_val(Game *game,int row,int col);
int create_valid_arr(Game *game,int *valid_arr, int row , int col,int N) ;
int get_the_order_number_from_arr(int order, int *valid_arr);
int count_invalid_numbers(Game *game);
void copy_solve_2_user(Game *game);
int generate_x(Game *game,int x);

#endif
