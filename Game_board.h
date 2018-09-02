#ifndef FINALPROJECT_GAME_BOARD_H
#define FINALPROJECT_GAME_BOARD_H

typedef struct {
    int value;
    int is_fix;
    int is_error;
} Cell;
typedef struct  Game Game;

Cell** create_new_board(int rows_size , int cols_size);
Cell* create_new_cell(int value ,int is_fix, int is_error);
void print_cell(Cell *cell,int mode);
void insert_zero_cells(Cell **arr,int size);
void free_board(Game *my_game);
void print_separator_row(int size,int m);
void print_user_board(Game *game);
void free_boards(Game *my_game);




#endif //FINALPROJECT_GAME_BOARD_H
