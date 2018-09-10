#include <stdio.h>
#include "Play.h"
#include "Game.h"
#include "Error_handler.h"
#include <string.h>
#include "Functionality.h"
#include "IlpSolver.h"

void undo_print(Data *data) {
    int x = data->row+1;
    int y = data->col+1;
    int value = data->value;
    int prev_value = data->prev_value;
    if (value != 0 && prev_value != 0) {
        printf("Undo %d,%d :from %d to %d\n", x, y, prev_value, value);
    } else if (value != 0 && prev_value == 0) {
        printf("Undo %d,%d :from _ to %d\n", x, y, value);
    } else if (value == 0 && prev_value == 0) {
        printf("Undo %d,%d :from _ to _\n", x, y);
    } else {
        printf("Undo %d,%d :from %d to _\n", x, y, prev_value);
    }
}

void redo_print(Data *data) {
    int x = data->row+1;
    int y = data->col+1;
    int value = data->value;
    int prev_value = data->prev_value;
    if (value != 0 && prev_value != 0) {
        printf("Redo %d,%d :from %d to %d\n", x, y, prev_value, value);
    } else{
        if (value != 0 && prev_value == 0) {
            printf("Redo %d,%d :from _ to %d\n", x, y, value);
        }
        else{
            if (value == 0 && prev_value == 0) {
                printf("Redo %d,%d :from _ to _\n", x, y);
            }
            else {
                printf("Redo %d,%d :from %d to _\n", x, y,prev_value);
            }
        }
    }
}
void clear_board(Game *game){
    int N = game->m_mult_n*game->m_mult_n;
    int i,j;
    for ( i = 0; i <N ; ++i) {
        for ( j = 0; j <N ; ++j) {
           game->user_game_board[i][j].value = 0 ;
        }
    }
}


void do_generate(Game *game,int x, int y) { /* Generates a puzzle by randomly filling X cells with random legal values, running ILP to solve the resulting board, and then clearing all but Y random cells.*/
    int empty_cells = 0, i, res_from_ilp;
    int N = game->m_mult_n * game->m_mult_n;
    int row, col, rand_value, x_counter = 0, y_counter = 0;
    empty_cells = num_of_empty_cells(game); /* checking the number of empty cells in board*/
    if (game->mode != 0) /* if the mode is not edit print invalid command */{
        invalid_command();
        return;
        /* should we exit after invalid command?! */
    } else if (num_not_valid(empty_cells, x) || num_not_valid(empty_cells, y)) { /* checks if x and y valid vualues*/
        not_in_range(empty_cells);
        return;
    } else if (empty_cells < game->m_mult_n * game->m_mult_n) { /* if the we  try generate on not empty board */
        board_not_empty();
        return;
    } else {
        i = 0;
        while (i <= 1000) {
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
                    i++;
                    break;
                } else {
                    rand_value = get_leagel_random_val(game, row,
                                                       col); /* function returnes 0 if there isnt a leagel value and the right one if there is*/
                    if (rand_value == 0 || num_of_sol(game) == 0) {
                        clear_board(game);
                        i++;
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

    res_from_ilp = ilp_solver(game);
    if (res_from_ilp == 0) {
        clear_board(game);
        puzzle_generator_failed(); /* i put this one out! to check with itay its ok, and to check that the ilp solver to prints it by himself */
        return;
    }

    for (int j = 0; j < y; ++j) {
        row = rand() % N;
        col = rand() % N;
        while (game->user_game_board[row][col].is_fix == 1) {
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
            else { /* if it is one of the error cells so we initilize it back to 0 */
                game->user_game_board[i][j].is_error=0;
            }

        }

    }
}

int num_of_empty_cells(Game *game){ /* get game and return the number of empty cells in user_boardv*/
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

int num_not_valid(int empty_cells,int x){ /* return 0 if x is valid value , else return 1 */
    if (x>empty_cells){
        return 1;
    }
    else {
        return 0;
    }
}

int get_leagel_random_val(Game *game,int row,int col){ /* get game,num of row and col and create a list with all the valid numbers that can be insert to this cel,
 if there is none of these return 0, if there is valid numbers - choose one with random and return it */
    int i,res =1 ,num_of_valid_values=0; /* to make sure it is ok that res starts from 1 and not from 0 */
    int cnt=0,j;
    int order_in_arr;
    int N = game->m_mult_n;
    int *valid_arr = malloc(sizeof(int)*N + 1); /* notice the +1, may couse prblems */
    if (valid_arr == NULL){
        printf("memory allocation problem with malloc in get_leagel_random_al fnunch");
        exit(1);
    }
    for ( i = 1; i <N+1; ++i) { /* assign 1 in leagel numbers that are valid to the place in the arry */
       if(is_valid(game,row,col,i)==1){
           valid_arr[i]=1;
       }
    }
    for ( j = 1; j <N+1 ; ++j) { /* count the number of valid numbers */
        num_of_valid_values += valid_arr[j];
    }
    if (num_of_valid_values==0){
        return 0;
    }
    order_in_arr = rand() % num_of_valid_values; /* get the valid number that is in the place that random gave */
    while(cnt <= order_in_arr){
        res++;
        if (valid_arr[cnt] == 1) {
            cnt++;
        }
    }
    free(valid_arr);
    return res;
    }

void do_validate(Game *game){
    int res,num_of_errors=0;

    if (num_of_errors(game) != 0 ){
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

void do_hint(Game *game, int row, int cols){
    /* to check if i need to verify the mode here or it going to be checkgd outside */
    int N  = game->m_mult_n;
    int res;
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
        cell_contatins_value();
        return;
    }
    res = ilp_solver(game);
    if (res = 0){
        board_not_solvable();
        return;
    }
    printf("Hint: set cell to %d\n",game->solved_game_board[row][cols].value);
    /* shoude i do the set my self or the user need to do it?????????? */

}