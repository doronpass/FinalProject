//
// Created by Doron_Passal on 27/08/2018.
//

#ifndef FINALPROJECT_PLAY_H
#define FINALPROJECT_PLAY_H
#include "Game.h"

void undo_print(Data *data);

void do_solve (char *x);

void do_edit(char *x);

void do_mark_errors (char *x);

void do_print_board (char *x);

void do_set (Game *game ,int x, int y , int value);

void do_validate (char *x);

void do_generate (char *x);

void do_undo (Game *game ,int x, int y , int value , int pre_value);

void do_redo (Game *game ,int x, int y , int value , int pre_value );








#endif //FINALPROJECT_PLAY_H
