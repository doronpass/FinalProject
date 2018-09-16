#include <string.h>
#include <stdio.h>
#include "Play.h"
#include "Game_board.h"
#include "Error_handler.h"
#include "Functionality.h"
#include "IlpSolver.h"

/* performs the necessary tests before saving the user's board, since save in edit mode can only
 * happen if the puzzle is valid and has no errors*/
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
        puzzle_solution_erroneus();
        return;
    }
    /* make sure the board is valid before save in edit mode */
     if (validate(my_game)==0){
        printf("Error: board validation failed\n");
        return;
    }
    if (save_to_file(my_game, path)==0){
        printf("Error: File cannot be created or modified\n");
    } else{
        printf("Saved to: %s\n", path);
    }
}

/* change the mark_errors value to 0 or 1, or inform the user about a problem in the input */
void mark_errors(Game *my_game, int x) {
    if (x<-1) { /*meaning input was not a number */
        printf("Error: the value should be 0 or 1\n");
        return;
    }
    x++; /*add 1 because we decrease 1 when we get input to match matrix coordinates */
    if (x==1) {
        my_game->mark_error = 1;
    } else if (x==0){
        my_game->mark_error = 0;
    } else {
        printf("Error: the value should be 0 or 1\n");
    }
}

/* the function is called after user used the solve/edit command
 * given the command and the path to a file, check if a game is in progress (is_there_old_game is 1)
 * or not (is_there_old_game is 0 or 2) and pass the arguments to the appropriate function to fill the
 * Game struct fields and read the user_board values from the file given in the "path" argument */
Game * init_game(char *command, char *path, Game *new_game, int is_there_old_game) {
    if (is_there_old_game==1) {
        return init_during_game(command, path, new_game);
    } else {
        return init_without_game(command, path, new_game,is_there_old_game);
    }
}

/* initiate a game while another game is in progress, including freeing previously allocated
 * memory and loading user board from the file given in the "path" input
 * in case an error occurred while loading from file, returns the previous game unchanged */
Game * init_during_game(char *command, char *path, Game *new_game) {
    int assert, i, j;
    Game *clone;
    if (path == NULL) {
        /* free all previous game memory and create 9X9 empty board (only on edit mode, checked by another function) */
        free_all_mem(new_game);
        new_game->mode = 0;
        new_game->n_block_cols = 3;
        new_game->m_block_rows = 3;
        new_game->m_mult_n = 9;
        new_game->user_game_board = create_new_board(3, 3);
        new_game->doubly_linked_list = create_new_dll();
        new_game->solved_game_board = create_new_board(new_game->m_block_rows, new_game->n_block_cols);
        print_user_board(new_game);
        return new_game;
    }
    /* there is a previous game and file path/name was passed */
    if (strcmp(command, "solve") == 0) {
        new_game->mode = 1;
    } else if (strcmp(command, "edit") == 0) {
        new_game->mode = 0;
    }
    clone = clone_game(new_game); /* clone previous game boards */
    free_boards(new_game); /* free boards before trying to load new ones */
    assert = load_from_file(new_game, path);
    if (assert == 0) {
        /* if we failed to load, we want to return the prev game as it was */
        new_game->user_game_board = create_new_board(new_game->m_block_rows, new_game->n_block_cols);
        new_game->solved_game_board = create_new_board(new_game->m_block_rows, new_game->n_block_cols);
        for (i = 0; i < new_game->m_mult_n; i++) {
            for (j = 0; j < new_game->m_mult_n; j++) {
                new_game->user_game_board[i][j] = clone->user_game_board[i][j];
                new_game->solved_game_board[i][j] = clone->solved_game_board[i][j];
            }
        }
        free_boards(clone); /*clone no longer needed, free it */
        free(clone);
        return new_game;
    }
    /*after loading successfully, reset dll and create new solved_board */
    while (strcmp(new_game->doubly_linked_list->last->command_name, "start_node")!=0){
        remove_last(new_game->doubly_linked_list);
    }
    new_game->solved_game_board = create_new_board(new_game->m_block_rows, new_game->n_block_cols);
    free_boards(clone); /*clone no longer needed, free it */
    free(clone);
    print_user_board(new_game);
    return new_game;
}
/* initiate a game from init mode, meaning before a game was created (is_there_old_game is 0)
 * or a game was created but the user solved it (game over, is_there_old_game is 2)
 * keeps the user in init mode if an error occurred while loading from file*/
Game * init_without_game(char *command, char *path, Game *new_game, int is_there_old_game){
    int assert;
    if (is_there_old_game==0){
        new_game->mark_error = 1; /*default value, else keep value of the previous game */
    }
    if (strcmp(command, "solve") == 0) {
        new_game->mode = 1;
    } else if (strcmp(command, "edit") == 0) {
        new_game->mode = 0;
    }
    if (path == NULL) { /*file path not given */
        /* create 9X9 empty board (only on edit mode, checked by another function) */
        new_game->n_block_cols = 3;
        new_game->m_block_rows = 3;
        new_game->m_mult_n = 9;
        new_game->user_game_board = create_new_board(3, 3);
    } else { /* file path given */
        assert = load_from_file(new_game, path);
        if (assert == 0) { /*loading failed */
            new_game->mode = -1; /*indicates an error */
            return new_game;
        }
    }
    new_game->doubly_linked_list = create_new_dll();
    new_game->solved_game_board = create_new_board(new_game->m_block_rows,new_game->n_block_cols);
    mark_error_cells(new_game);
    print_user_board(new_game);
    return new_game;
}

/* change the value of cell (x,y) to z, after making sure the input numbers are in range and cell is not fixed
 * returns 1 and append the new data entry to dll node data array if value was changed, else return 0 */
int set(Game *my_game, int x, int y, int z, Node *node){/*changed x and y order */
    Data *data;
    int prev_val;
    if (!(x<my_game->m_mult_n && y<my_game->m_mult_n && z<=my_game->m_mult_n) || x<0 || y<0 || z<0) {
        not_in_range(my_game->m_mult_n);
        return 0;
    }
    prev_val = my_game->user_game_board[x][y].value;
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
    return 1;
}

/* fill cells that has only 1 valid option.
 * note that we check of a cell is valid with a clone Game that holds the user board
 * of my_game input before executing the command*/
int autofill(Game *my_game, Node *node) {
    int i,j,k,num_of_valid_nums, new_val=0, changed=0;
    Game *clone;
    if (has_erroneous_values(my_game)==1){
        puzzle_solution_erroneus();
        return 0;
    }
    clone = clone_game(my_game);

    for (i=0;i<my_game->m_mult_n;i++){
        for (j=0;j<my_game->m_mult_n;j++){
            if (my_game->user_game_board[i][j].value==0){
                num_of_valid_nums=0;
                for (k=1;k<=my_game->m_mult_n;k++){
                    if (is_valid(clone,i,j,k)){ /*check if valid in the clone, meaning the original board, before the command */
                        num_of_valid_nums+=1;
                        new_val = k;
                    }
                }
                if (num_of_valid_nums==1) {
                    set(my_game, i, j, new_val, node);
                    printf("Cell <%d,%d> set to %d\n", j+1,i+1,new_val); /* swiched between j+1 and i+1 */
                    changed=1;
                }
            }
        }
    }
    free_boards(clone);
    free(clone);
    return changed;
}

/* undo the current move (accodring to the dll pointer) */
void undo(Game *my_game){
    Node *node_to_undo = my_game->doubly_linked_list->dll_pointer;
    int i;
    if (my_game->doubly_linked_list->dll_pointer->prev == NULL){
        printf("Error: no moves to undo\n");
        return;
    }
    /* need to print the board and then the changes, so we divided it to 2 separate loops */
    for (i=0; i<node_to_undo->node_data_size;i++){
        set_without_dll(my_game,node_to_undo->node_data[i]->col,node_to_undo->node_data[i]->row,node_to_undo->node_data[i]->prev_value); /* changed between cole and row */
    }
    print_user_board(my_game);
    for (i=0; i<node_to_undo->node_data_size;i++){
        undo_print(node_to_undo->node_data[i]);
    }
    my_game->doubly_linked_list->dll_pointer = my_game->doubly_linked_list->dll_pointer->prev;
}

/* (re)do the next move on the dll (according to the dll pointer) */
void redo(Game *my_game){
    Node *node_to_redo = my_game->doubly_linked_list->dll_pointer->next;
    int i;
    if (my_game->doubly_linked_list->dll_pointer->next == NULL){
        printf("Error: no moves to redo\n");
        return;
    }
    for (i=0; i<node_to_redo->node_data_size;i++){
        set_without_dll(my_game,node_to_redo->node_data[i]->col,node_to_redo->node_data[i]->row,node_to_redo->node_data[i]->value); /* change between col and row */
    }
    print_user_board(my_game);
    for (i=0; i<node_to_redo->node_data_size;i++){
        redo_print(node_to_redo->node_data[i]);
    }
    my_game->doubly_linked_list->dll_pointer = my_game->doubly_linked_list->dll_pointer->next;
}

/* revert to the original board the user loaded (either from a file or a blank 9x9)
 * clear the dll */
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

int generate(Game *game, Node *node,int x, int y) { /* Generates a puzzle by randomly filling X cells with random legal values, running ILP to solve the resulting board, and then clearing all but Y random cells.*/
    int empty_cells = 0, i, j;
    int N = game->m_mult_n, flag = 0;
    int row = -1, col = -1;
    Data *data;
    empty_cells = num_of_empty_cells(game); /* checking the number of empty cells in board*/

    if (x > (game->m_mult_n * game->m_mult_n) ||
        y > (game->m_mult_n * game->m_mult_n) || x<0 || y<0) { /* checks if x and y valid vualues*/
        not_in_range(N*N);
        return 0;
    } else if (empty_cells < game->m_mult_n * game->m_mult_n) { /* if the we  try generate on not empty board */
        board_not_empty();
        return 0 ;
    } else {
        flag = generate_x(game,x);
    }
    /* until here we choose x random places and gave every one an optional number */
    /* from here try to solve the board, and then delete y valuse randomly */

    copy_solve_2_user(game);
    /* this method copy solved board to user board */
    for ( j = 0; j < y; ++j) {
        row = rand() % N;
        col = rand() % N;
        while (game->user_game_board[row][col].is_error == 1) {
            row = rand() % N;
            col = rand() % N;
        }
        game->user_game_board[row][col].is_error = 1; /* use error for other goal, only to specify what value enter there */
    }
    /* here we're going to delete all except y values*/
    for (i = 0; i < N; ++i) {
        for (j = 0; j < N; ++j) {
            if (game->user_game_board[i][j].is_error != 1) { /* if its not a "error" cell so we need to delete it*/
                game->user_game_board[i][j].value= 0;
            }
            else { /* if it is one of the error cells so we initilize it back to 0  and add to the do\undo list the changed that have been mase*/
                game->user_game_board[i][j].is_error=0;
                data = create_new_data (i ,j ,game->user_game_board[i][j].value, 0);
                append_data_to_node(node, data);
            }
        }
    }
    return flag;
}
/*Validates the current board using ILP, ensuring it is solvable,the func returns 2 if there erros, 1 if seccess and 0 if failed */
int validate(Game *game){
    int res;
    if(count_invalid_numbers(game) != 0 ){
        return 2;
    }
    res = ilp_solver(game); /* res 0 if ilp_solver failed, else it get 1 */
    return  res;

}
/*Give a hint to the user by showing the solution of a single cell X,Y.*/
void hint(Game *game, int row, int cols){
    int N  = game->m_mult_n;
    int res;
    if (game->mode==0){ /* check we are in solve mode */
        invalid_command();
        return;

    }
    if (row>=N || cols>=N || row<0 || cols <0){ /*checks that all the arguments are valid */
        printf("Error: value not in range 1-%d\n",N);
        return;
    }
    if ( count_invalid_numbers(game) != 0){
        puzzle_solution_erroneus();
        return;
    }
    if (game->user_game_board[row][cols].is_fix==1){
        cell_is_fixed();
        return;
    }
    if ((game->user_game_board[row][cols].is_fix==0) && (game->user_game_board[row][cols].value!=0)){
        cell_contains_value();
        return;
    }
    res = ilp_solver(game);
    if (res == 0){
        board_not_solvable();
        return;
    }
    printf("Hint: set cell to %d\n",game->solved_game_board[row][cols].value);
}

