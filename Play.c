#include <string.h>
#include <stdio.h>
#include "Play.h"
#include "Game_board.h"
#include "Error_handler.h"
#include "Functionality.h"
#include "IlpSolver.h"


/* print a move that was undone in the correct format
 * used by the undo function*/
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

/* print a move that was redone in the correct format
 * used by the redo function */
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
    int N = game->m_mult_n;
    int i,j;
    for ( i = 0; i <N ; ++i) {
        for ( j = 0; j <N ; ++j) {
            printf("i = %d , j= %d \n",i,j);
           game->user_game_board[i][j].value = 0 ;
      /*      game->solved_game_board[i][j].value = 0 ;     */

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
int get_legal_random_val(Game *game,int row,int col) {
    printf("197\n");
    int N = game->m_mult_n;
    int *valid_arr;
    int valid_number_counter=0;
    int x,order;
    valid_arr = (int*)malloc(sizeof(int)*(N+1));
    printf("203\n");
    valid_number_counter = create_valid_arr(game,valid_arr,row,col,N); /* create arr of 1 in the valid number and return unmber of valid numbers*/
    printf("206\n");

    if (valid_number_counter == 0) {
        return 0;
    }
    printf("211\n");

    order = rand() % valid_number_counter;
    printf("214\n");

    x = get_the_order_number_from_arr(order, valid_arr);
    printf("217\n");

    free(valid_arr);
    return x;

}

int create_valid_arr(Game *game,int *valid_arr, int row , int col,int N) {
    printf("225\n");

    int i,cnt=0;
    for (i = 1; i < (N + 1); ++i) {
        if (is_valid(game, row, col, i) == 1) {
            valid_arr[i] = 1;
            cnt++;
        }
        else{
            valid_arr[i] = 0;
        }
    }
    printf("237\n");

    return cnt;
}

int get_the_order_number_from_arr(int order, int *valid_arr){
    printf("243\n");

    int res,index=0,ones_cnt=0;
    while(ones_cnt<order){
        if ( valid_arr[index] == 1 ){
            ones_cnt++;
            }

        index++;
    }
    res = valid_arr[index-1];
    printf("254\n");

    return res;
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

void copy_solve_2_user(Game *game){ /* copy solve board to user board */
    int i,j;
    int N=game->m_mult_n;
    for (i = 0; i <N ; ++i) {
        for (j = 0; j <N; ++j) {
            game->user_game_board[i][j]=game->solved_game_board[i][j];
        }
    }


}
