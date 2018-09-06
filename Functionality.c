#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "Play.h"
#include "Functionality.h"
#include "Error_handler.h"


/* the function used when the user enters the "save" command*/
void save_game(Game *my_game, char *path){
    /*if game mode is solve, simply save to file*/
    if (my_game->mode == 1) {
        if (save_to_file(my_game, path)==0){
            printf("Error: File cannot be created or modified\n");
        } else{
            printf("Saved to: %s\n", path);
        }
       return;
    }

    /*make sure board is not erroneous before save in edit mode*/
    if (has_erroneous_values(my_game)) {
        printf("Error: board contains erroneous values\n");
        return;
    }

    /*make sure the board is valid before save in edit mode ------ NEED TO BUILD FUNCTION - need gurubi*/
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
    int i,j,cell_value;
    FILE *file;
    int m = my_game->m_block_rows;
    int n = my_game->n_block_cols;
    int N = my_game->m_mult_n;
    int assertion = 0;
    file = fopen(path, "w");
    if (file == NULL){
        /*file cannot be opened*/
        return 0;
    }
    assertion=fprintf(file, "%d %d\n", m, n);
    if (assertion<0){
        /*problem modifying file*/
        fclose(file);
        return 0;
    }
    for (i=0; i<N; i++) {
        for (j=0; j<N; j++) {
            cell_value = my_game->user_game_board[i][j].value;
            if (cell_value!=0 && (my_game->mode == 0 || my_game->user_game_board[i][j].is_fix == 1)){
                assertion=fprintf(file, "%d. ", cell_value);
                if (assertion<0){
                    /*problem modifying file*/
                    fclose(file);
                    return 0;
                }
            } else{
                assertion=fprintf(file, "%d ", cell_value);
                if (assertion<0){
                    /*problem modifying file*/
                    fclose(file);
                    return 0;
                }
            }
        }
        /*end of row, new line*/
        assertion = fprintf(file, "\n");
        if (assertion<0){
            /*problem modifying file*/
            fclose(file);
            return 0;
        }
    }
    fclose(file);
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

/* DONT FORGET  to point out erroneous cells if needed - do this in the solve/edit functions
need to resolve the board after load to get the solved_board for my_game */

int load_from_file(Game *my_game, char *path) {
    FILE *file;
    int value, i, j;
    file = fopen(path, "r");
    if (file == NULL) {
        /*file cannot be opened*/
        if (my_game->mode == 1) {  /*solve mode*/
            printf("Error: File doesn't exist or cannot be opened\n");
        } else {                    /*edit mode*/
            printf("Error: File cannot be opened\n");
        }
        fclose(file);
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
        fclose(file);
        return 0;
    }
    value-= '0';
    my_game->m_block_rows = value;

    /* read n */
    while ((value = getc(file)) != EOF) {
        if (isspace(value) == 0) {
            break;
        }
    }
    if (ferror(file)) {      /*test if error, if so return 0) */
        printf("Error: Unable to read file\n");
        fclose(file);
        return 0;
    }
    value-= '0';
    my_game->n_block_cols = value;
    my_game->m_mult_n = my_game->n_block_cols * my_game->m_block_rows;

    /*create empty new board*/
    my_game->user_game_board = create_new_board(my_game->m_block_rows, my_game->n_block_cols);

    /*read matrix values */
    for (i = 0; i < my_game->m_mult_n; i++) {
        for (j = 0; j < my_game->m_mult_n; j++) {
            while ((value = getc(file)) != EOF) {

                /*if we get '.' - set last cell to fixed and continue to look for next cell's value */
                if (value == '.') {
                    my_game->user_game_board[i][j-1].is_fix = 1;
                    continue;
                }
                /*if we read a whitespace char - keep reading */
                if (isspace(value) == 0) {
                    break;
                }
            }
            if (ferror(file)) {      /*test if error, if so return 0) */
                printf("Error: Unable to read file\n");
                fclose(file);
                return 0;
            }
            value-= '0';
            my_game->user_game_board[i][j].value = value;
        }
    }
    fclose(file);
    return 1;
}




/*returns 1 if the number z is valid for cell (x,y)
 *returns 0 if the number isn't valid for the cell - based on the current solution */
int is_valid(Game *my_game, int x, int y, int z) {
    int i, j, block_first_row, block_first_col;
    int m = my_game->m_block_rows;
    int n = my_game->n_block_cols;
    int N = n * m;
    if (z==0) {
        return 1;
    }
    /* search row (row is x) */
    for (i = 0; i < N; i++) {
        if ((i != y) && (my_game->user_game_board[x][i].value == z)) {
            return 0;
        }
    }
    /* search col (col is y)*/
    for (j = 0; j < N; j++) {
        if ((j != x) && (my_game->user_game_board[j][y].value == z))
            return 0;
    }
    /* search in block - dividing ints returns the floor value of the actual division */
    block_first_row = (x / m) * m;
    block_first_col = (y / n) * n;
    for (i = block_first_row; i < (block_first_row + m); i++) {
        for (j = block_first_col; j < (block_first_col + n); j++) {
            if (my_game->user_game_board[i][j].value == z && (j != y || i != x)) {
                return 0;
            }
        }
    }
    return 1;
}

/* change the mark errors option on and off */
void mark_errors(Game *my_game) {
    if (my_game->mark_error==1) {
        my_game->mark_error = 0;
    } else if (my_game->mark_error==0){
        my_game->mark_error = 1;
    } else {
        printf("Error: the value should be 0 or 1\n");
    }
}


/* the function is called after user used the solve/edit command
 * given the command and the path, create a new game and load the board from
 * the file in the path argument */

/* ---------- need to free previous game memory before creating new, maybe another function ----- */
Game * init_game(char *command, char *path, Game *new_game, int is_there_old_game) {
    int assert,i,j;
    if (is_there_old_game){
        free_all_mem(new_game); /* free old game boards and dll if there was an old game */
    }
    if (strcmp(command, "solve") == 0) {
        new_game->mode = 1;
    }
    if (strcmp(command, "edit") == 0) {
        new_game->mode = 0;
    }
    new_game->mark_error = 1; /*default value */
    new_game->doubly_linked_list = create_new_dll();
    if (path == NULL){
        /* create 9X9 empty board (will only happen on edit, checked by another function */
        new_game->n_block_cols = 3;
        new_game->m_block_rows = 3;
        new_game->m_mult_n = 9;
        new_game->user_game_board = create_new_board(3,3);
    } else {
        assert = load_from_file(new_game, path);
        if (assert == 0) {
            exit(1);
        }
    }
    /* ------------ need function to check all cells for is_error ----------------------------- */
    return new_game;
}

/* executes the set command, after making sure the input numbers are in range and cell is not fixed
 * returns 1 if value was changed, else 0 */
int set(Game *my_game, int x, int y, int z, Node *node){
    Data *data;
    int prev_val;
    prev_val = my_game->user_game_board[x][y].value;
    if (!(x<my_game->m_mult_n && y<my_game->m_mult_n && z<=my_game->m_mult_n)) {
        not_in_range(my_game->m_mult_n);
        return 0;
    }
    if (my_game->user_game_board[x][y].is_fix == 1){
        cell_is_fixed();
        return 0;
    }
    my_game->user_game_board[x][y].value = z;
    my_game->user_game_board[x][y].is_error = 0;
    unmark_erroneous_before_change(my_game, x, y, prev_val);
    mark_erroneous_after_change(my_game, x, y, z);
    data = create_new_data (x ,y ,z, prev_val);
    append_data_to_node(node, data);
}

/* go over the board after a value was change, change is_error of all cells that are erroneous
 *  after the change to 1 */
void mark_erroneous_after_change(Game *my_game, int x, int y, int z){
    int i, j, block_first_row, block_first_col;
    int m = my_game->m_block_rows;
    int n = my_game->n_block_cols;
    int N = my_game->m_mult_n;
    /* changing value to 0 is always valid */
    if (z==0){
        return;
    }
    /* search row (row is x) */
    for (i = 0; i < N; i++) {
        if ((i != y) && (my_game->user_game_board[x][i].value == z)) {
            my_game->user_game_board[x][i].is_error = 1;
            my_game->user_game_board[x][y].is_error = 1;
        }
    }
    /* search col (col is y)*/
    for (j = 0; j < N; j++) {
        if ((j != x) && (my_game->user_game_board[j][y].value == z)) {
            my_game->user_game_board[j][y].is_error = 1;
            my_game->user_game_board[x][y].is_error = 1;
        }
    }
    /* search in block - dividing ints returns the floor value of the actual division */
    block_first_row = (x / m) * m;
    block_first_col = (y / n) * n;
    for (i = block_first_row; i < (block_first_row + m); i++) {
        for (j = block_first_col; j < (block_first_col + n); j++) {
            if (my_game->user_game_board[i][j].value == z && (j != y && i != x)) {
                my_game->user_game_board[x][y].is_error = 1;
                my_game->user_game_board[i][j].is_error = 1;
            }
        }
    }
}

/* go over the board before a value was change, change is_error of all cells that will not
 * be erroneous after the change to 0 */
void unmark_erroneous_before_change(Game *my_game, int x, int y, int z){
    int i, j, block_first_row, block_first_col;
    int m = my_game->m_block_rows;
    int n = my_game->n_block_cols;
    int N = my_game->m_mult_n;
    /* search row (row is x) */
    for (i = 0; i < N; i++) {
        if ((i != y) && (my_game->user_game_board[x][i].value == z)) {
            if (is_valid(my_game, x, i, z)) {
                my_game->user_game_board[x][i].is_error = 0;
            }
        }
    }
    /* search col (col is y)*/
    for (j = 0; j < N; j++) {
        if ((j != x) && (my_game->user_game_board[j][y].value == z)) {
            if (is_valid(my_game, j, y, z)) {
                my_game->user_game_board[j][y].is_error = 0;
            }
        }
    }
    /* search in block - dividing ints returns the floor value of the actual division */
    block_first_row = (x / m) * m;
    block_first_col = (y / n) * n;
    for (i = block_first_row; i < (block_first_row + m); i++) {
        for (j = block_first_col; j < (block_first_col + n); j++) {
            if (my_game->user_game_board[i][j].value == z && (j != y || i != x)) {
                if (is_valid(my_game, i, j, z)) {
                    my_game->user_game_board[i][j].is_error = 0;
                }
            }
        }
    }
}


/* fill cells with only 1 valid option */

/* NEED TO MAKE SURE that changing value to my_game (with set function) does not effect clone (otherwise, the function won't work properly) */
int autofill(Game *my_game, Node *node) {
    int i,j,k,num_of_valid_nums, new_val=0, changed=0;
    Game *clone = clone_game(my_game);
    for (i=0;i<my_game->m_mult_n;i++){
        for (j=0;j<my_game->m_mult_n;j++){
            if (my_game->user_game_board[i][j].value==0){
                num_of_valid_nums=0;
                for (k=1;k<=my_game->m_mult_n;k++){
                    if (is_valid(clone,i,j,k)){
                        num_of_valid_nums+=1;
                        new_val = k;
                    }
                }
                if (num_of_valid_nums==1) {
                    set(my_game, i, j, new_val, node);
                    changed=1;
                }
            }
        }
    }
    return changed;
}

/* creates a basic clone of the input Game, copying the cell values and
 *  the fields of the input Game, not including solved_game_board and doubly_linked_list
 *  used by autofill and backtracking functions */
Game * clone_game(Game *my_game) {
    int i,j;
    Game *clone = (Game *) malloc(sizeof(Game));
    clone->solved_game_board=NULL;
    clone->mark_error=my_game->mark_error;
    clone->mode=my_game->mode;
    clone->m_block_rows = my_game->m_block_rows;
    clone->n_block_cols = my_game->n_block_cols;
    clone->m_mult_n=my_game->m_mult_n;
    clone->doubly_linked_list=NULL;
    clone->user_game_board = create_new_board(my_game->m_block_rows,my_game->n_block_cols);
    for (i=0;i<my_game->m_mult_n;i++){
        for (j=0;j<my_game->m_mult_n;j++) {
            clone->user_game_board[i][j].value = my_game->user_game_board[i][j].value;
        }
    }
    return clone;
}


/* undo the current move (accodring to the pointer) */
void undo(Game *my_game){
    Node *node_to_undo = my_game->doubly_linked_list->dll_pointer;
    int i;
    if (my_game->doubly_linked_list->dll_pointer->prev == NULL){
        printf("Error: no moves to undo\n");
        return;
    }
    /* need to print the board and then the changes, so we divided it to 2 separate loops */
    for (i=0; i<node_to_undo->node_data_size;i++){
        set_without_dll(my_game,node_to_undo->node_data[i]->row,node_to_undo->node_data[i]->col,node_to_undo->node_data[i]->prev_value);
    }
    print_user_board(my_game);
    for (i=0; i<node_to_undo->node_data_size;i++){
        undo_print(node_to_undo->node_data[i]);
    }
    my_game->doubly_linked_list->dll_pointer = my_game->doubly_linked_list->dll_pointer->prev;
}

/* (re)do the next move on the move list (according to the pointer) */
void redo(Game *my_game){
    Node *node_to_redo = my_game->doubly_linked_list->dll_pointer->next;
    int i;
    if (my_game->doubly_linked_list->dll_pointer->next == NULL){
        printf("Error: no moves to redo\n");
        return;
    }
    for (i=0; i<node_to_redo->node_data_size;i++){
        set_without_dll(my_game,node_to_redo->node_data[i]->row,node_to_redo->node_data[i]->col,node_to_redo->node_data[i]->value);
    }
    print_user_board(my_game);
    for (i=0; i<node_to_redo->node_data_size;i++){
        redo_print(node_to_redo->node_data[i]);
    }
    my_game->doubly_linked_list->dll_pointer = my_game->doubly_linked_list->dll_pointer->next;
}


/* same as set, without adding a new data entry to a node data array.
 * used by undo/redo to change values without effecting the dll */
void set_without_dll(Game *my_game, int x, int y, int z) {
    int prev_val = my_game->user_game_board[x][y].value;
    if (!(x<my_game->m_mult_n && y<my_game->m_mult_n && z<=my_game->m_mult_n)) {
        not_in_range(my_game->m_mult_n);
        return;
    }
    if (my_game->user_game_board[x][y].is_fix == 1){
        cell_is_fixed();
        return;
    }
    my_game->user_game_board[x][y].value = z;
    my_game->user_game_board[x][y].is_error = 0;
    unmark_erroneous_before_change(my_game, x, y, prev_val);
    mark_erroneous_after_change(my_game, x, y, z);
};

/* revert to the original board the user loaded (either from a file or a blank 9x9)
 * clear the moves list */
void reset(Game *my_game){
    /* if there is a play to undo, undo it */
    while (strcmp(my_game->doubly_linked_list->dll_pointer->command_name, "start_node")!=0){
        undo_without_output(my_game);
    }
    /* reset dll */
    while (strcmp(my_game->doubly_linked_list->last->command_name, "start_node")!=0){
        remove_last(my_game->doubly_linked_list);
    }
    printf("Board reset\n");
}

/* same as the undo function, but does not print any input
 * used by reset function */
void undo_without_output(Game *my_game) {
    Node *node_to_undo = my_game->doubly_linked_list->dll_pointer;
    int i;
    for (i=0; i<node_to_undo->node_data_size;i++){
        set_without_dll(my_game,node_to_undo->node_data[i]->row,node_to_undo->node_data[i]->col,node_to_undo->node_data[i]->prev_value);
    }
    my_game->doubly_linked_list->dll_pointer = my_game->doubly_linked_list->dll_pointer->prev;
}

/* free all memory alocated inside Game, including dll and boards (user and solution)
 * does not free Game itself */

void free_all_mem(Game *my_game){
    /* free dll */
    while (strcmp(my_game->doubly_linked_list->last->command_name, "start_node")!=0){
        remove_last(my_game->doubly_linked_list);
    }
    free(my_game->doubly_linked_list);
    /* free both game boards */
    free_boards(my_game);

}