

//Created by Doron_Passal on 24/08/2018.


#ifndef FINALPROJECT_GAME_BOARD_H
#define FINALPROJECT_GAME_BOARD_H
#include "Game.h"

typedef struct {
    int value;
    int is_fix;
    int is_error;
}Cell;

typedef struct Game Game;


void check_memory2(int** point);
void check_memory(int* point);
Cell** create_new_board(Game *game);
Cell* create_new_cell(int value,int is_fix, int is_error);
void  print_board(Game *game);
void print_cell(Cell *cell);



#endif FINALPROJECT_GAME_BOARD_H
