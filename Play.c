#include <string.h>
#include <stdio.h>
#include "Play.h"
#include "Game_board.h"
#include "Error_handler.h"
#include "Functionality.h"
#include "IlpSolver.h"

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
        puzzle_solution_erroneus();
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

/* change the mark errors option on and off */
void mark_errors(Game *my_game, int x) {
    if (x==1) {
        my_game->mark_error = 1;
    } else if (x==0){
        my_game->mark_error = 0;
    } else {
        printf("Error: the value should be 0 or 1\n");
    }
}

/* the function is called after user used the solve/edit command
 * given the command and the path, create a new game and load the board from
 * the file in the path argument */

/* ---------- need to free previous game memory before creating new, maybe another function ----- */
Game * init_game(char *command, char *path, Game *new_game, int is_there_old_game) {
    int assert;
    if (is_there_old_game){
        free_all_mem(new_game); /* free old game boards and dll if there was an old game */
    } else {
        new_game->mark_error = 1; /*default value */
    }
    if (strcmp(command, "solve") == 0) {
        new_game->mode = 1;
    }
    if (strcmp(command, "edit") == 0) {
        new_game->mode = 0;
    }
    new_game->solved_game_board = create_new_board(new_game->m_block_rows,new_game->n_block_cols);
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
            free(new_game->doubly_linked_list->first);
            free(new_game->doubly_linked_list);
            new_game->mode = -1; /*indicates an error */
        } else {
            mark_error_cells(new_game);
        }
    }
    print_user_board(new_game);
    return new_game;
}



/* executes the set command, after making sure the input numbers are in range and cell is not fixed
 * returns 1 if value was changed, else 0 */
int set(Game *my_game, int x, int y, int z, Node *node){
    Data *data;
    int prev_val;
    if (!(x<my_game->m_mult_n && y<my_game->m_mult_n && z<=my_game->m_mult_n)) {
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

/* fill cells with only 1 valid option */
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
                    if (is_valid(clone,i,j,k)){
                        num_of_valid_nums+=1;
                        new_val = k;
                    }
                }
                if (num_of_valid_nums==1) {
                    set(my_game, i, j, new_val, node);
                    printf("Cell <%d,%d> set to %d\n", i+1,j+1,new_val);
                    changed=1;
                }
            }
        }
    }
    free_boards(clone);
    free(clone);
    return changed;
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

void generate(Game *game, Node *node,int x, int y) { /* Generates a puzzle by randomly filling X cells with random legal values, running ILP to solve the resulting board, and then clearing all but Y random cells.*/
    int empty_cells = 0, i, res_from_ilp = 0, j;
    int N = game->m_mult_n;
    int row = -1, col = -1, rand_value = -1, x_counter = 1;
    Data *data;
    printf("205\n");
    empty_cells = num_of_empty_cells(game); /* checking the number of empty cells in board*/
    printf("207\n");

    if (x > (game->m_mult_n * game->m_mult_n) ||
        y > (game->m_mult_n * game->m_mult_n)) { /* checks if x and y valid vualues*/
        not_in_range(empty_cells);
        return;
    } else if (empty_cells < game->m_mult_n * game->m_mult_n) { /* if the we  try generate on not empty board */
        board_not_empty();
        return;
    } else {
        i = 0;
        while (i <= 1000) {
            if (x_counter == x) {
                printf("221\n");

                res_from_ilp = ilp_solver(game);

                printf("225\n");

                if (res_from_ilp == 1) {
                    break;
                } else {
                    i++;
                    clear_board(game);
                }
            }
            x_counter = 1;
            while (x_counter <= x) {
                if (i == 1000) {
                    puzzle_generator_failed();

                    clear_board(game); /*to add func the returned the board to be all 0 */


                    return;
                }

                row = rand() % N;
                col = rand() % N;

                if (game->user_game_board[row][col].value != 0) {
                    /*  i++;  TO CHECK IF NEEDED*/

                    continue;
                } else {
                    printf("254\n");


                    rand_value = get_legal_random_val(game, row,
                                                      col); /* function returnes 0 if there isnt a legal value and the right one if there is*/
                    printf("rand val is %d\n", rand_value);
                    if (rand_value == 0) {
                        printf("means no valid value \n");
                        clear_board(game);
                        i++;
                        printf("i = %d", i);
                        break;
                    } else {
                        game->user_game_board[row][col].value = rand_value;
                        x_counter++;
                    }
                }
            }
        }
    }


    /* until here we choose x random places and gave every one an optional number */
    /* from here try to solve the board, and then delete y valuse randomly */

    if (res_from_ilp == 0) {
        clear_board(game);
        puzzle_generator_failed(); /* i put this one out! to check with itay its ok, and to check that the ilp solver to prints it by himself */
        return;
    }
    copy_solve_2_user(game); /* this method copy solved board to user board */;
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
    print_user_board(game);
}

void validate(Game *game){
    int res;
    if(count_invalid_numbers(game) != 0 ){
        puzzle_solution_erroneus();
        return;
    }
    res = ilp_solver(game);

    if (res ==1 ){
        validation_passed();
    }
    else {
        validation_failed();
    }
}

void hint(Game *game, int row, int cols){

    int N  = game->m_mult_n;
    int res;
    if (game->mode==0){ /* check we are in solve mode */
        invalid_command();
        return;

    }
    if (row<0 || cols <0){
        invalid_command();
    }
    if (row>N || cols>N){
        not_in_range(N);
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

