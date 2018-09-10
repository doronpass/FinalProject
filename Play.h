#ifndef FINALPROJECT_PLAY_H
#define FINALPROJECT_PLAY_H
#include "Game.h"

void undo_print(Data *data);
void redo_print(Data *data);

void do_solve (char *x);
void do_hint(Game *game,int row,int cols);


void do_edit(char *x);

void do_mark_errors (char *x);

void do_print_board (char *x);

void do_set (Game *game ,int x, int y , int value);

void do_generate (Game *game, int x,int y);

void do_undo (Game *game ,int x, int y , int value , int pre_value);

void do_redo (Game *game ,int x, int y , int value , int pre_value );

int num_of_empty_cells(Game *game);
int num_not_valid(int empty_cells,int x);

int get_leagel_random_val(Game *game,int row,int col);
void do_validate(Game *game);
int count_invalid_numbers(Game *game);

void copy_solve_2_user(Game *game);

#endif

