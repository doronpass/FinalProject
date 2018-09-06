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
Game * init_game(char *command, char *path, Game *new_game, int is_there_old_game);
int set(Game *my_game, int x, int y, int z, Node *node);
void mark_erroneous_after_change(Game *my_game, int x, int y, int z);
void unmark_erroneous_before_change(Game *my_game, int x, int y, int z);
int autofill(Game *my_game, Node *node);
Game * clone_game(Game *my_game);
void undo(Game *my_game);
void redo(Game *my_game);
void set_without_dll(Game *my_game, int x, int y, int z);
void reset(Game *my_game);
void undo_without_output(Game *my_game);
void free_all_mem(Game *my_game);
#endif
