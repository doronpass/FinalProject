#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "Play.h"
#include "Functionality.h"
#include "Error_handler.h"
#include "IlpSolver.h"


/* writes the block sizes and the user board to a file according to the given format*/
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

/* if there is a cell with an erroneous value in the board - return 1, else 0*/
int has_erroneous_values(Game *my_game) {
    int i,j;
    int N = my_game->m_mult_n;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            if (my_game->user_game_board[i][j].is_error == 1) {
                return 1;
            }
        }
    }
    return 0;
}


/*read the block sizes of a board and then the board itself from a file, according to given format
 * the function ignores excess whitespace chars between values in the matrix*/
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
                    if (my_game->mode==1){
                        if (j==0) { /*fixed cell is last in row */
                            my_game->user_game_board[i-1][my_game->m_mult_n-1].is_fix = 1;
                        } else {
                            my_game->user_game_board[i][j-1].is_fix = 1;
                        }
                    }
                    continue;
                }
                /*if we read a whitespace char - do nothing and read the next char */
                if (isspace(value) == 0) {
                    break;
                }
            }
            if (ferror(file)) {      /*test if error, if so return 0) */
                printf("Error: Unable to read file\n");
                fclose(file);
                free_boards(my_game);
                return 0;
            }
            value-= '0';
            my_game->user_game_board[i][j].value = value;
        }
    }
    /* test if last cell on matrix is fixed  */
    value = getc(file);
    if (value=='.' && my_game->mode==1){
        my_game->user_game_board[my_game->m_mult_n-1][my_game->m_mult_n-1].is_fix = 1;
    }
    fclose(file);
    return 1;
}


/*returns 1 if the number z is valid for cell (x,y) on the current user board
 *returns 0 if the number isn't valid for the cell  */
int is_valid(Game *my_game, int x, int y, int z) {
    int i, j, block_first_row, block_first_col;
    int m = my_game->m_block_rows;
    int n = my_game->n_block_cols;
    int N = n * m;
    if (z==0) {
        return 1;
    }
    /* search col */
    for (i = 0; i < N; i++) {
        if ((i != y) && (my_game->user_game_board[x][i].value == z)) {
            return 0;
        }
    }
    /* search row*/
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


/* goes over a board and marks erroneous cells.
 * used when loading new board */
void mark_error_cells(Game *my_game){
    int i,j;
    for (i=0;i<my_game->m_mult_n;i++){
        for (j=0;j<my_game->m_mult_n;j++){
            if (my_game->user_game_board[i][j].value!=0){
                if (!is_valid(my_game,i,j,my_game->user_game_board[i][j].value)){
                    my_game->user_game_board[i][j].is_error=1;
                }
            }
        }
    }
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
void unmark_erroneous_before_change(Game *my_game, int x, int y, int z){ /* not change because set have chnged */
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

/* creates a basic clone of the input Game, copying the cell values and
 *  the fields of the input Game, not including doubly_linked_list.
 *  used by autofill and backtracking functions */
Game * clone_game(Game *my_game) {
    int i,j;
    Game *clone = (Game *) malloc(sizeof(Game));
    if (clone==NULL){
        printf("Error: malloc has failed\n");
        exit(0);
    }
    clone->mark_error=my_game->mark_error;
    clone->mode=my_game->mode;
    clone->m_block_rows = my_game->m_block_rows;
    clone->n_block_cols = my_game->n_block_cols;
    clone->m_mult_n=my_game->m_mult_n;
    clone->doubly_linked_list=NULL;
    clone->user_game_board = create_new_board(my_game->m_block_rows,my_game->n_block_cols);
    clone->solved_game_board= create_new_board(my_game->m_block_rows,my_game->n_block_cols);
    for (i=0;i<my_game->m_mult_n;i++){
        for (j=0;j<my_game->m_mult_n;j++) {
            clone->user_game_board[i][j].value = my_game->user_game_board[i][j].value;
            clone->user_game_board[i][j].is_error = my_game->user_game_board[i][j].is_error;
            clone->user_game_board[i][j].is_fix = my_game->user_game_board[i][j].is_fix;
            clone->solved_game_board[i][j].value = my_game->solved_game_board[i][j].value;
            clone->solved_game_board[i][j].is_fix = my_game->solved_game_board[i][j].is_fix;
        }
    }
    return clone;
}

/* same as set, without adding a new data entry to a node data array.
 * used by undo/redo to change values without effecting the dll */
void set_without_dll(Game *my_game, int y, int x, int z) {
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
}

/* same as the undo function, but does not print any input
 * used by reset function */
void undo_without_output(Game *my_game) {
    Node *node_to_undo = my_game->doubly_linked_list->dll_pointer;
    int i;
    for (i=0; i<node_to_undo->node_data_size;i++){
        set_without_dll(my_game,node_to_undo->node_data[i]->col,node_to_undo->node_data[i]->row,node_to_undo->node_data[i]->prev_value);
    }
    my_game->doubly_linked_list->dll_pointer = my_game->doubly_linked_list->dll_pointer->prev;
}

/* free all memory allocated inside Game, including dll and boards (user and solution)
 * does not free Game itself */
void free_all_mem(Game *my_game){
    /* free dll */
    while (strcmp(my_game->doubly_linked_list->last->command_name, "start_node")!=0){
        remove_last(my_game->doubly_linked_list);
    }
    free(my_game->doubly_linked_list->first);
    free(my_game->doubly_linked_list);
    /* free both game boards */
    free_boards(my_game);
}

/* print a move that was undone in the correct format
 * used by the undo function*/
void undo_print(Data *data) {
    int y = data->row+1; /* change between*/
    int x = data->col+1;
    int value = data->value;
    int prev_value = data->prev_value;
    if (value != 0 && prev_value != 0) {
        printf("Undo %d,%d: from %d to %d\n", x, y, value,prev_value);
    } else if (value != 0 && prev_value == 0) {
        printf("Undo %d,%d: from %d to _\n", x, y, value);
    } else if (value == 0 && prev_value == 0) {
        printf("Undo %d,%d: from _ to _\n", x, y);
    } else {
        printf("Undo %d,%d: from _ to %d\n", x, y, prev_value);
    }
}

/* print a move that was redone in the correct format
 * used by the redo function */
void redo_print(Data *data) {
    int y = data->row+1;
    int x = data->col+1;
    int value = data->value;
    int prev_value = data->prev_value;
    if (value != 0 && prev_value != 0) {
        printf("Redo %d,%d: from %d to %d\n", x, y, prev_value, value);
    } else{
        if (value != 0 && prev_value == 0) {
            printf("Redo %d,%d: from _ to %d\n", x, y, value);
        }
        else{
            if (value == 0 && prev_value == 0) {
                printf("Redo %d,%d: from _ to _\n", x, y);
            }
            else {
                printf("Redo %d,%d: from %d to _\n", x, y,prev_value);
            }
        }
    }
}

/* change the value of every cell in the user board to 0 */
void clear_board(Game *game){
    int N = game->m_mult_n;
    int i,j;
    for ( i = 0; i <N ; ++i) {
        for ( j = 0; j <N ; ++j) {
            game->user_game_board[i][j].value = 0 ;
        }
    }
}

/* returns the number of empty cells in the user board*/
int num_of_empty_cells(Game *game){
    int j,i, empty_cells = 0;
    int N = game->m_mult_n;
    for ( i = 0; i <N ; ++i) {
        for ( j = 0; j <N ; ++j) {
            if (game->user_game_board[i][j].value==0){
                empty_cells++;
            }
        }
    }
    return empty_cells;
}

/* randomly selects and returns a valid number for cell (col,row) in the user's board
 * used by the "generate" function*/
int get_legal_random_val(Game *game,int row,int col) {
    int N;
    int *valid_arr;
    int valid_number_counter=0;
    int x,order;
    N = game->m_mult_n;
    valid_arr = (int*)malloc(sizeof(int)*(N+1));
    valid_number_counter = create_valid_arr(game,valid_arr,row,col,N);
    if (valid_number_counter == 0) {
        free(valid_arr);
        return 0;
    }
    order = rand() % valid_number_counter;
    x = get_the_order_number_from_arr(order+1, valid_arr);
    free(valid_arr);
    return x;

}
/* fill the valid_arr with 1 in index i if i is a valid value for cell (col,row), or 0 else */
int create_valid_arr(Game *game,int *valid_arr, int row , int col,int N) {
    int i,cnt=0;
    valid_arr[0] = 0; /* index 0 is not used */
    for (i = 1; i < (N + 1); ++i) {
        if (is_valid(game, row, col, i) == 1) {
            valid_arr[i] = 1;
            cnt++;
        }
        else{
            valid_arr[i] = 0;
        }
    }
    return cnt;
}
/* goes over the valid_arr array, count every element in the array with 1 in it (meaning index is valid number)
 * and return the index of the element after "order" (some number) 1's were found */
int get_the_order_number_from_arr(int order, int *valid_arr){
    int res,index=1,ones_cnt=0;
    while(ones_cnt<order){
        if ( valid_arr[index] == 1 ){
            ones_cnt++;
        }

        index++;
    }
    res = (index-1);
    return res;
}
/* goes over the user board and counts how many cells are erroneous, return the result */
int count_invalid_numbers(Game *game){
    int i,j,count_errors=0;
    int N = game->m_mult_n;
    for (i = 0; i <N ; ++i) {
        for (j = 0; j < N; ++j) {
            if(game->user_game_board[i][j].is_error == 1){
                count_errors++;
            }
        }
    }
    return count_errors;
}

void copy_solve_2_user(Game *game){ /* copy solve board to user board , used by ilp_solver */
    int i,j;
    int N=game->m_mult_n;
    for (i = 0; i <N ; ++i) {
        for (j = 0; j <N; ++j) {
            game->user_game_board[i][j]=game->solved_game_board[i][j];
        }
    }
}
/*  this function called from generate and choosing x cells ranbdomly and operate ilp for generate goal */
int generate_x(Game *game,int x){

    int N = game->m_mult_n, res_from_ilp = 0;
    int x_counter = 1, rand_value = -1, row = -1, col = -1, i = 0;
    while (i <= 1000) {
        if (x_counter == x) {
            res_from_ilp = ilp_solver (game);
            if (res_from_ilp == 1) {
                break;
            }
            else {
                i++;
                clear_board (game);
            }
        }
        x_counter = 1;
        while (x_counter <= x) {
            if (i == 1000) {
                printf ("i= %d\n",i);
                clear_board (game);
                puzzle_generator_failed ();
                clear_board (game);
                return 0;
            }
            row = rand () % N;
            col = rand () % N;
            if (game->user_game_board[row][col].value != 0) {
                continue;
            }
            else {

                rand_value = get_legal_random_val (game, row,
                                                   col); /* function returnes 0 if there isnt a legal value and the right one if there is*/

                if (rand_value == 0) {
                    clear_board (game);
                    i++;
                    break;
                }
                else {
                    game->user_game_board[row][col].value = rand_value;
                    x_counter++;

                }
            }
        }
    }
    if (res_from_ilp == 0) {

        clear_board (game);
        puzzle_generator_failed ();
        return 0;
    }
    return 1;
}

