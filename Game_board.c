//
// Created by Doron_Passal on 24/08/2018.
//
#include "Game_board.h"


/*tests whether calloc failed
 * if so, alert the user and terminate program*/
void check_memory(Cell* point){
    if (point == NULL) {
        printf("Error: calloc has failed\n");
        exit(0);
    }
}

/*tests whether calloc failed
 * if so, alert the user and terminate program*/
void check_memory2(Cell** point){
    if (point == NULL) {
        printf("Error: calloc has failed\n");
        exit(0);
    }


/*allocates dynamic memory space for a game board based on size N*N */
Cell** create_new_board(Game *game){
    int i;
    Cell** arr =(Cell**)calloc(game->m_mult_n,sizeof(*Cell));
    check_memory2(arr);
    for (i=0; i<game->m_mult_n; i++){
        arr[i] = (Cell*) calloc (game.m_mult_n,sizeof(cell));
        check_memory(arr[i]);
    }
    return arr;
    }

Cell* create_new_cell(int value ,int is_fix, int is_error){
    Cell *new_cell;
    new_cell = (Cell*)malloc(sizeof(Cell));
    new_cell->is_error=is_error;
    new_cell->is_fix=is_fix;
    new_cell->value=value;
    return new_cell;
    }

void print_board(Game *game){
    int i;
    int size = 4*game->m_mult_n+game->m_block_rows+1;
    char sep_raw[] = (char*)malloc(size);
    for (i = 0; i < size ; ++i) {
        strcat(sep_raw,"-");
    }
    printf("%s\n", sep_raw);

    }
void print_cell(Cell *cell){
     char *arr =" ";
     char *str[8];
     itoa(cell->value,str,10);
     strcat(arr,str);
     if (cell->is_fix){
         strcat(arr,".");
     }
     if (cell->is_error){
         strcat(arr,"*");
     }
     printf("%s\n", arr);
    }

