//
// Created by Doron_Passal on 24/08/2018.
//

#ifndef FINALPROJECT_GAME_BAORD_H
#define FINALPROJECT_GAME_BAORD_H
typedef struct
{
    int value;
    int is_fix;
    int is_error;
} Cell;

void check_memory2(int** point);
void check_memory(int* point);
Cell** create_new_board(Game *game);



#endif //FINALPROJECT_GAME_BAORD_H
