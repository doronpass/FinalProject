#ifndef FINALPROJECT_PLAY_H
#define FINALPROJECT_PLAY_H
#include "Game_board.h"

void save_game(Game *my_game, char *path);
void mark_errors(Game *my_game, int x);
int set(Game *my_game, int x, int y, int z, Node *node);
int autofill(Game *my_game, Node *node);
void undo(Game *my_game);
void redo(Game *my_game);
void reset(Game *my_game);
Game * init_game(char *command, char *path, Game *new_game, int is_there_old_game);
void hint(Game *game,int row,int cols);
void generate (Game *game,Node *node, int x,int y);
void validate(Game *game);

#endif

