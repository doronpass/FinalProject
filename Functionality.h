#include "Dll.h"
#include "Game_board.h"
#include "Game.h"

#ifndef FINALPROJECT_FUNCTIONALITY_H
#define FINALPROJECT_FUNCTIONALITY_H
void save_game(Game *my_game, char *path);
int save_to_file(Game *my_game, char *path);
int has_erroneous_values(Game *my_game);
int load_from_file(Game *my_game, char *path);
int is_valid(Game *my_game,int x, int y, int z);
void mark_errors(Game *my_game);
Game * init_game(char *command, char *path, Game *new_game);
void mark_erroneous_after_change(Game *my_game, int x, int y, int z);
void unmark_erroneous_before_change(Game *my_game, int x, int y, int z);

#endif //FINALPROJECT_FUNCTIONALITY_H
