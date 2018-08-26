#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include "Functionality.h"




/* the function used when the user enters the "save" command*/
void save_game(Game *my_game, char *path){
    int m = my_game->m_block_rows;
    int n = my_game->n_block_cols;
    int N = n*m;
    /*if game mode is solve, simply save to file*/
    if (my_game->mode == 0) {
        save_to_file(my_game, path);
        printf("Saved to: %s\n", path);
        return;
    }

    /*make sure board is not erroneous before save in edit mode*/
    if (has_erroneous_values(my_game)) {
        printf("Error: board contains erroneous values\n");
        return;
    }

    /*make sure the board is valid before save in edit mode*/
    if (!is_valid(my_game)){
        printf("Error: board validation failed\n");
        return;
    }

    if (save_to_file(my_game, path)==0){
        printf("Error: File cannot be created or modified\n");
    } else{
        printf("Saved to: %s\n", path);
    }
}


/* the actual process of writing the game board in a text file in the correct format for saving*/
int save_to_file(game *my_game, char *path){
    FILE *file;
    int m = my_game->m_block_rows;
    int n = my_game->n_block_cols;
    int N = n*m;
    int cell_value;
    int assertion = 0;
    file = fopen(path, "w");
    if (file == NULL){
        /*file cannot be opened*/
        return 0;
    }
    assertion=fprintf(file, "%d %d\n", m, n);
    if (assertion<0){
        /*problem modifying file*/
        return 0;
    }
    for (int i=0; i<N; i++) {
        for (int j=0; j<(N-1); j++) {
            cell_value = my_game->user_game_board[i][j].value;
            if (my_game->mode == 0 || my_game->user_game_board[i][j].is_fix == 1){
                assertion=fprintf(file, "%d. ", cell_value);
                if (assertion<0){
                    /*problem modifying file*/
                    return 0;
                }
            } else{
                assertion=fprintf(file, "%d ", cell_value);
                if (assertion<0){
                    /*problem modifying file*/
                    return 0;
                }
            }
        }

        /*last cell in the row, no space after it*/
        cell_value = my_game->user_game_board[i][N-1].value;
        if (my_game->mode == 0 || my_game->user_game_board[i][N-1].is_fix == 1){
            assertion=fprintf(file, "%d.", cell_value);
            if (assertion<0){
                /*problem modifying file*/
                return 0;
            }
        } else{
            assertion = fprintf(file, "%d", cell_value);
            if (assertion<0){
                /*problem modifying file*/
                return 0;
            }
        }

        /*end of row, new line*/
        assertion = fprintf(file, "\n");
        if (assertion<0){
            /*problem modifying file*/
            return 0;
        }
    }
    return 1;
}

/* return 1 if there is a cell with an erroneous value in the board
 * to test before save in edit mode*/
int has_erroneous_values(game *my_game) {
    int N = my_game->m_block_rows * my_game->n_block_cols;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < (N - 1); j++) {
            if (my_game->user_game_board[i][j].is_error == 1) {
                return 1;
            }
        }
    }
    return 0;
}


/*getting matrix information (size and value) from a formatted file */

//don't forget to point out errouos cells if needed - do this in the solve/edit functions
//need to resolve the board after load to get the solved_board for my_game

int load_from_file(game *my_game, char *path){
    FILE *file;
    int value;
    int N; //=n*m
    file = fopen(path, "r");
    if (file == NULL){
        /*file cannot be opened*/
        return 0;
    }
    /* read m */
    while (1){
        value = getc(file);
        if (isspace(value)==0){
            break;
        }
    }
    my_game->m_block_rows = value;

    /* read n */
    while (1){
        value = getc(file);
        if (isspace(value)==0){
            break;
        }
    }
    my_game->n_block_cols = value;

    /*read matrix values
     * no need to test for EOF, since files are guaranteed to be correct */
    N = my_game->n_block_cols * my_game->m_block_rows;
    for (int i=0; i<N; i++) {
        for (int j = 0; j < N; j++) {
            while (1){
                value = getc(file);
                /*if we get '.' - set last cell to fixed and continue to look for next cell's value */
                if (value == '.') {
                    my_game->user_game_board[j][i-1].is_fix = 1;
                    continue;
                }
                /*if we read a whitespace char - keep reading */
                if (isspace(value)==0){
                    break;
                }
            }
            my_game->user_game_board[j][i].value = value;
        }
    }
}



/*returns 1 if the number z is valid for cell (x,y)
 *returns 0 if the number isn't valid for the cell - based on the current solution */
int is_valid(game *my_game,int x, int y, int z) {
    int i, j, block_first_row, block_first_col;
    int m = my_game->m_block_rows;
    int n = my_game->n_block_cols;
    int N = n*m;

    if (my_game->user_game_board[x][y] == z){
        return 1;
    }
    /* search col (col is x) */
    for (i=0 ; i<N ; i++){
        if ( (i!=y) && (my_game->user_game_board[x][i] == z)){
            return 0;
        }
    }
    /* search row (row is y)*/
    for (j=0 ; j <N ; j++ ){
        if ((j != x) &&  (my_game->user_game_board[j][y] == z) )
            return 0;
    }
    /* search in block - dividing ints returns the floor value of the actual division */
    block_first_col = (x/n) * n;
    block_first_row = (y/m) * m;
    for (i=block_first_row; i<(block_first_row+m); i++){
        for (j=block_first_col; j<(block_first_col+n); j++) {
            if (my_game->user_game_board[j][i] == z && (i!=y || j !=x)){
                return 0;
            }
        }
    }
    return 1;
}