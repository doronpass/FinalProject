#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "Functionality.h"


/* the function used when the user enters the "save" command*/
void save_game(Game *my_game, char *path){
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

    /*make sure the board is valid before save in edit mode ------ NEED TO BUILD FUNCTION*/
/*
     if (!validate(my_game)){
        printf("Error: board validation failed\n");
        return;
    }
*/
    if (save_to_file(my_game, path)==0){
        printf("Error: File cannot be created or modified\n");
    } else{
        printf("Saved to: %s\n", path);
    }
}


/* the actual process of writing the game board in a text file in the correct format for saving*/
int save_to_file(Game *my_game, char *path){
    int i,j;
    FILE *file;
    int m = my_game->m_block_rows;
    int n = my_game->n_block_cols;
    int N = my_game->m_mult_n;
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
    for (i=0; i<N; i++) {
        for (j=0; j<(N-1); j++) {
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
int has_erroneous_values(Game *my_game) {
    int i,j;
    int N = my_game->m_mult_n;
    for (i = 0; i < N; i++) {
        for (j = 0; j < (N - 1); j++) {
            if (my_game->user_game_board[i][j].is_error == 1) {
                return 1;
            }
        }
    }
    return 0;
}


/*getting matrix information (size and value) from a formatted file */

//don't forget to point out erroneous cells if needed - do this in the solve/edit functions
//need to resolve the board after load to get the solved_board for my_game

int load_from_file(Game *my_game, char *path) {
    FILE *file;
    int value, i, j;
    int N = my_game->m_mult_n;
    file = fopen(path, "r");
    if (file == NULL) {
        /*file cannot be opened*/
        if (my_game->mode == 1) {  /*solve mode*/
            printf("Error: File doesn't exist or cannot be opened\n");
        } else {                    /*edit mode*/
            printf("Error: File cannot be opened\n");
        }
        return 0;
    }
    /* read m */
    while ((value = getc(file)) != EOF) {    /*note that error in getc returns an EOF*/
        if (isspace(value) == 0) {
            break;
        }
    }
    if (ferror(file)) {      /*test if error, if so return 0) */
        printf("Error: Unable to read file\n"); /*custom comment as instructed in forum */
        return 0;
    }
    my_game->m_block_rows = value;

    /* read n */
    while ((value = getc(file)) != EOF) {
        if (isspace(value) == 0) {
            break;
        }
    }
    if (ferror(file)) {      /*test if error, if so return 0) */
        printf("Error: Unable to read file\n");
        return 0;
    }
    my_game->n_block_cols = value;
    my_game->m_mult_n = my_game->n_block_cols * my_game->m_block_rows;

    /*create empty new board*/
    my_game->user_game_board = create_new_board(my_game->m_block_rows, my_game->n_block_cols);

    /*read matrix values */
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            while ((value = getc(file)) != EOF) {

                /*if we get '.' - set last cell to fixed and continue to look for next cell's value */
                if (value == '.') {
                    my_game->user_game_board[j][i - 1].is_fix = 1;
                    continue;
                }
                /*if we read a whitespace char - keep reading */
                if (isspace(value) == 0) {
                    break;
                }
            }
            if (ferror(file)) {      /*test if error, if so return 0) */
                printf("Error: Unable to read file\n");
                return 0;
            }
            my_game->user_game_board[j][i].value = value;
        }
    }
}




/*returns 1 if the number z is valid for cell (x,y)
 *returns 0 if the number isn't valid for the cell - based on the current solution */
int is_valid(Game *my_game, int x, int y, int z) {
    int i, j, block_first_row, block_first_col;
    int m = my_game->m_block_rows;
    int n = my_game->n_block_cols;
    int N = n * m;

    if (my_game->user_game_board[x][y].value == z) {
        return 1;
    }
    /* search col (col is x) */
    for (i = 0; i < N; i++) {
        if ((i != y) && (my_game->user_game_board[x][i].value == z)) {
            return 0;
        }
    }
    /* search row (row is y)*/
    for (j = 0; j < N; j++) {
        if ((j != x) && (my_game->user_game_board[j][y].value == z))
            return 0;
    }
    /* search in block - dividing ints returns the floor value of the actual division */
    block_first_col = (x / n) * n;
    block_first_row = (y / m) * m;
    for (i = block_first_row; i < (block_first_row + m); i++) {
        for (j = block_first_col; j < (block_first_col + n); j++) {
            if (my_game->user_game_board[j][i].value == z && (i != y || j != x)) {
                return 0;
            }
        }
    }
    return 1;
}

void mark_errors(Game *my_game) {
    if (my_game->mark_error) {
        my_game->mark_error = 0;
    } else {
        my_game->mark_error = 1;
    }
}


/* the function is called after user used the solve/edit command
 * given the command and the path, create a new game and load the board from
 * the file in the path argument */

/* ---------- need to free previous game memory before creating new, maybe another function ----- */
Game *init_game(char *command, char *path) {
    int assert,i,j;
    Game *new_game = (Game *) malloc(sizeof(Game));
    if (new_game == NULL) {
        printf("Error: malloc has failed\n");
        exit(0);
    }
    if (strcmp(command, "solve") == 0) {
        new_game->mode = 1;
    }
    if (strcmp(command, "edit") == 0) {
        new_game->mode = 0;
    }
    new_game->mark_error = 1; /*default value */
    if (path == NULL){
        /* create 9X9 empty board (will only happen on edit, checked by another function */
        new_game->user_game_board = create_new_board(3,3);
    } else {
        assert = load_from_file(new_game, path);
        if (assert == 0) {
            exit(1);
        }
        /* make all fixed on "edit" command */
        if (new_game->mode == 0) {
            for (i = 0; i < new_game->m_mult_n; i++) {
                for (j = 0; j < new_game->m_mult_n; j++) {
                    new_game->user_game_board[j][i].is_fix = 1;
                }
            }
        }

    }
    /* ------------ need function to check all cells for is_error ----------------------------- */
    return new_game;
}

