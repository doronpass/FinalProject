//
// Created by Doron_Passal on 27/08/2018.
//

#ifndef FINALPROJECT_PLAY_H
#define FINALPROJECT_PLAY_H
#include "Game.h"

void undo_print(int x,int y,int pre_value,int value);

void do_solve (char *x);

void do_edit(char *x);

void do_mark_errors (char *x);

void do_print_board (char *x);

void do_set (Game *game ,int x, int y , int value);

void do_validate (char *x);

void do_generate (char *x);

void do_undo (Game *game ,int x, int y , int value , int pre_value);

void do_redo (char *x){}

void do_save (char *x){}

void do_hint (char *x){}

void do_num_soulutions (char *x){}

void do_autofill (char *x){}

void do_reset (char *x){}

void do_exit (char *x){}










#endif //FINALPROJECT_PLAY_H
