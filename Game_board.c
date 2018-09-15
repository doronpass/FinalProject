#include <stdio.h>
#include <stdlib.h>
#include "Game_board.h"
#include "Error_handler.h"
#include "Functionality.h"
#include "Parser.h"
#define PIPE "|"

/*starts the game
 * the first loop will keep going until user exits game, the 2nd loop makes the user initiates a new game
 * the last loop keeps the game going until it is over or the user exits the game
 * game_status:
 * 0 during a game
 * 1 when the game is in init mode
 * 2 when the user enters "exit" command to exit program
 *
 * is_there_old_game:
 * 0 if there is no game
 * 1 if there is a game
 * 2 if there was a game but it is over (game over)*/

void start_game(){
    int game_status = 0, is_there_old_game=0;
    Game *my_game=(Game *) malloc(sizeof(Game));
    if (my_game==NULL){
        printf("Error: malloc has failed\n");
        exit(0);
    }
    printf("Sudoku\n------\n");
    while (1){
        if (game_status==2) {
            break;
        }
        game_status = init_user_turn(my_game,is_there_old_game);
        while (game_status==1){
            game_status = init_user_turn(my_game,is_there_old_game);
        }
        if (game_status==2) {
            break;
        }
        is_there_old_game=1;
        while(1){
            game_status = user_turn(my_game);
            if (game_status==1 || game_status == 2){
                if(game_status==1) {/*if game is over */
                    is_there_old_game=2;
                }
                break;
            }
        }
    }
    free(my_game);
}
/* print a cell in the board according to the given format */
void print_cell(Cell *cell,int mode, int mark_error) {
    printf("%s", " ");
    if ((cell->value) == 0){
        printf("%s","  ");
    }
    else{
        printf("%2d",cell->value);}
    if (cell->is_fix && cell->value != 0) {
        printf("%s","."); }
    else if (cell->value!=0 && cell->is_error==1 && ((mode == 0) || (mark_error==1))){ /* mode 0 is edit mode*/
        printf("%s", "*");
    }
    else {
        printf(" ");
    }
}

/*allocates memory for a game board with N*N Cells */
Cell **create_new_board(int rows_size, int cols_size) {
    int size, i;
    Cell **arr;
    size = rows_size * cols_size;
    arr = (Cell **) calloc(size,sizeof(Cell *));
    check_memory2(arr);
    for (i = 0; i < size; ++i) {
        arr[i] = (Cell *) calloc(size,sizeof(Cell));
        check_memory(arr[i]);
    }
    insert_zero_cells(arr, size);
    return arr;
}
/* set all values inside every cell in a board to 0
 * used when creating new board */
void insert_zero_cells(Cell **arr, int size){
    int i, j;
    for (i = 0; i < size; ++i) {
        for (j = 0; j < size; ++j) {
            arr[i][j].is_fix = 0;
            arr[i][j].is_error = 0;
            arr[i][j].value = 0;
        }
    }
}
/* prints separator row between blocks in the game board */
void print_separator_row(int size,int m) {
    int i;
    int num_of_dash=(size*4)+m+1;
    for (i = 0; i <num_of_dash ; ++i) {
        printf("-");
    }
    printf("\n");
}
/* goes over the user board and print it according to the given format.
 * erroneous cells are marked only in edit mode or if the mark_error flag in Game struct is 1 */
void print_user_board(Game *game){
    int i, j, size;
    if (game == NULL){
        exit(1);
    }
    size = game->m_mult_n;
    for (i = 0; i < size; ++i) {
        if ((i % (game->m_block_rows)) == 0) {
            print_separator_row(size,game->m_block_rows);
        }
        for ( j = 0; j < size; ++j) {
            if ((j % (game->n_block_cols)) == 0) {
                printf(PIPE);
            }
            print_cell(&(game->user_game_board[i][j]), game->mode, game->mark_error);
        }
        printf("%s\n",PIPE);
    }
    print_separator_row(size,game->m_block_rows); /* there is an empty line after! check if it's fine */
}


/* free the user board and, if initialized, solved board */
void free_boards(Game *my_game){
    int i;
    if( my_game->solved_game_board == NULL){
        for (i=0;i<my_game->m_mult_n;i++){
            free(my_game->user_game_board[i]);
        }
        free(my_game->user_game_board);
    } else {
        for (i=0;i<my_game->m_mult_n;i++){
            free(my_game->user_game_board[i]);
            free(my_game->solved_game_board[i]);
        }
        free(my_game->user_game_board);
        free(my_game->solved_game_board);
    }
}

/*works the same as create_new_board function, but creates a matrix of ints, not Cells */
int** create_matrix(int N){
    int i;
    int** arr;
    arr =(int**) calloc(N,sizeof(int*));
    if(arr ==NULL){
        printf ("Error: malloc has failed\n");
        exit(1);
    }
    for (i=0; i<N; i++){
        arr[i] = (int*) calloc (N,sizeof(int));
        if((arr[i])== NULL){
            printf ("Error: malloc has failed\n");
            exit(1);
        }
    }
    return arr;
}
/* copy the values of the result_arr input matrix into the input game user board matrix */
void copy_board_to_game(int **result_arr,int N,Game *game){
    int i,j;
    for (i = 0; i <N ;  ++i) {
        for (j = 0; j <N ; ++j) {
            game->solved_game_board[i][j].value = result_arr[i][j];


        }
    }
}
/* use in gurubi for copy the matrix to sol */
void copy_sol_to_board(double *sol,int **result_arr, int N ) {

    int i, j, v;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            for (v = 0; v < N; v++) {
                if (sol[i * N * N + j * N + v] == 1) {
                    result_arr[i][j] = v + 1;
                }
            }
        }
    }
}