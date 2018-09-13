#ifndef FINALPROJECT_PARSER_H
#define FINALPROJECT_PARSER_H
#include "Game_board.h"

int user_turn(Game *my_game);
int is_number(char *str);
int execute_function(Game *my_game, char *command_name, int x, int y, int z);
int init_user_turn(Game *my_game,int is_there_old_game);
int is_game_over(Game *my_game);
#endif
