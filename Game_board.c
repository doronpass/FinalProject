//
// Created by Doron_Passal on 24/08/2018.
//
#include <stdio.h>
#include <stdlib.h>
#include "Game_board.h"
#include "Game.h"
#include "Error_handler.h"

Cell* create_new_cell(int value ,int is_fix, int is_error){
    Cell *new_cell;
    new_cell = (Cell*)malloc(sizeof(Cell));
    new_cell->is_error=is_error;
    new_cell->is_fix=is_fix;
    new_cell->value=value;
    return new_cell;
}

void print_cell(Cell *cell,int mode) {
    printf("%s", " ");
    printf("%2d", cell->value);
    if (cell->is_fix) {
        printf("%s", ".");
    } else if (cell->is_error || mode == 0) /* mode 0 is edit mode*/
    {
        printf("%s", "*");
    }
}

/*allocates dynamic memory space for a game board based on size N*N */
    Cell** create_new_board(int rows_size ,int cols_size){
        int size,i,j;
        size = rows_size*cols_size;
        Cell** arr =(Cell**)calloc(size,sizeof(Cell*));
        check_memory2(arr);
        for(i=0;i<size;++i){
            arr[i] = (Cell*) calloc (size,sizeof(Cell));
            check_memory(arr[i]);
        }
        insert_zero_cells(arr,size);
        return arr;
    }

    void insert_zero_cells(Cell **arr,int size){
    int i,j;
        for ( i = 0; i <size ; ++i) {
            for ( j = 0; j <size ; ++j) {
                arr[i][j].is_fix =0;
                arr[i][j].is_error =0;
                arr[i][j].value =0;
            }
        }
}

    void print_board(Cell **board) {
    }

    void free_board(Cell **board,int size){
        int i,j;
        for ( i = 0; i <size ; ++i) {
                free(board[i]);
            }
        free(board);
        }


