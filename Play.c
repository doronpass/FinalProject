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


void do_generate(Game *game, Node *node,int x, int y) { /* Generates a puzzle by randomly filling X cells with random legal values, running ILP to solve the resulting board, and then clearing all but Y random cells.*/
    int empty_cells = 0, i, res_from_ilp=0,j;
    int N = game->m_mult_n;
    int row=-1, col=-1, rand_value = -1 , x_counter = 1;
    Data *data;
    empty_cells = num_of_empty_cells(game); /* checking the number of empty cells in board*/
    if (game->mode != 0) /* if the mode is not edit print invalid command */{
        invalid_command();
        return;
    } else if (num_not_valid(empty_cells, x) || num_not_valid(empty_cells, y) || (x<0) || (y<0)) { /* checks if x and y valid vualues*/
        not_in_range(empty_cells);
        return;
    } else if (empty_cells < game->m_mult_n * game->m_mult_n) { /* if the we  try generate on not empty board */
        board_not_empty();
        return;
    } else {
        i = 0;
        while (i <= 1000) {
            printf("77\n");
            if(x_counter == x){
                printf("79\n");
                res_from_ilp = ilp_solver(game);
                printf("81\n");
                if (res_from_ilp == 1){
                    break;
                } else{
                        i++;
                        clear_board(game);
                }
            }
            x_counter = 1;
            printf("90\n");
            while (x_counter <= x) {
                if (i == 1000) {
                    printf("93\n");
                    puzzle_generator_failed();
                    printf("95\n");

                    clear_board(game); /*to add func the returned the board to be all 0 */

                    printf("97\n");

                    return;
                }
                printf("103\n");

                row = rand() % N;
                col = rand() % N;
                printf("107\n");
                printf("row num is :%d\n",row);
                printf("col num is : %d\n",col);
                printf("the value is : row: %d,col: %d ,value:%d\n",row,col, game->user_game_board[row][col].value);
                if (game->user_game_board[row][col].value != 0) {
                  /*  i++;  TO CHECK IF NEEDED*/
                    printf("111\n");
                    printf("111\n");
                    continue;
                } else {
                    printf("114\n");

                    rand_value = get_leagel_random_val(game, row,
                                                       col); /* function returnes 0 if there isnt a leagel value and the right one if there is*/
                    printf("rand val is %d\n", rand_value);
                    if (rand_value == 0 ) {
                        printf("117\n");
                        printf("before clear board\n" );

                        clear_board(game);
                        printf("after clear board\n" );
                        i++;
                        break;
                    } else {
                        printf("125\n");

                        game->user_game_board[row][col].value = rand_value;
                        x_counter++;
                    }
                }
            }
        }
    }

    /* until here we choose x random places and gave every one an optional number */
    /* from here try to solve the board, and then delete y valuse randomly */
    printf("137\n");
    if (res_from_ilp == 0) {
        printf("140\n");
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
                data = create_new_data (x ,y ,game->user_game_board[i][j].value, 0);
                append_data_to_node(node, data);
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
int get_leagel_random_val(Game *game,int row,int col) {
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



void do_validate(Game *game){
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

void do_hint(Game *game, int row, int cols){

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
