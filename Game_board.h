

//Created by Doron_Passal on 24/08/2018.


#ifndef FINALPROJECT_GAME_BOARD_H
#define FINALPROJECT_GAME_BOARD_H

typedef struct {
    int value;
    int is_fix;
    int is_error;
}Cell;

 typedef struct Game Game;

void check_memory2(Cell** point);
void check_memory(Cell* point);
void print_board(Game *game);
Cell** create_new_board(Game *game);
Cell* create_new_cell(int value ,int is_fix, int is_error) ;
void print_cell(Cell *cell);


#endif // FINALPROJECT_GAME_BOARD_H
