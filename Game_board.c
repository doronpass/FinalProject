#include <stdio.h>
#include <stdlib.h>
#include "Game_board.h"
#include "Error_handler.h"
#include "Game.h"
#define PIPE "|"
Cell* create_new_cell(int value ,int is_fix, int is_error){
    Cell *new_cell;
    new_cell = (Cell*)malloc(sizeof(Cell));
    if (new_cell==NULL){
        printf("Error: malloc has failed\n");
        exit(0);
    }
    new_cell->is_error=is_error;
    new_cell->is_fix=is_fix;
    new_cell->value=value;
    return new_cell;
}

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
        printf(" "); /* mabye would be better do check if the value is 0 at start and then pring 4 spaces, to check if leagal*/
    }
}

/*allocates dynamic memory space for a game board based on size N*N */
Cell **create_new_board(int rows_size, int cols_size) {
    int size, i;
    size = rows_size * cols_size;
    Cell **arr = (Cell **) malloc(sizeof(Cell *) * size);
    check_memory2(arr);
    for (i = 0; i < size; ++i) {
        arr[i] = (Cell *) malloc(sizeof(Cell) * size);
        check_memory(arr[i]);
    }
    insert_zero_cells(arr, size);
    return arr;
}

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

void print_separator_row(int size,int m) {
    int i;
    int num_of_dash=(size*4)+m+1;
    for (i = 0; i <num_of_dash ; ++i) {
        printf("-");
    }
    printf("\n");
}
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



void free_boards(Game *my_game){
    int i;
    for (i=0;i<my_game->m_mult_n;i++){
        free(my_game->user_game_board[i]);
        /* free(my_game->solved_game_board[i]); -------------------- un-comment this when we add gurubi and solved board */
    }
    free(my_game->user_game_board);
    /* free(my_game->solved_game_board); -------------------- un-comment this when we add gurubi and solved board */
}

/*allocates dynamic memory space for a game board based on size
 *all cells are initialized to 0 */
int** create_matrix(int N){
    int i;
    int** arr =(int**) calloc(N,sizeof(int*));
    if(arr ==NULL){
        printf("somthing about that maloc has faild@!#!#!@#!@#!@#!@#\n");
        exit(1);
    }
    for (i=0; i<N; i++){
        arr[i] = (int*) calloc (N,sizeof(int));
        if((arr[i])== NULL){
            printf("somthing about that maloc has faild@!#!#!@#!@#!@#!@#\n");
            exit(1);
        }
    }
    return arr;
}

void copy_board_to_game(int **result_arr,int N,Game *game){
    int i,j;
    game->solved_game_board = create_new_board(N,N);
    for (i = 0; i <N ;  ++i) {
        for (j = 0; j <N ; ++j) {
            printf("copy_board_to_game i=%d,j=%d \n",i,j);
            game->solved_game_board[i][j].value = result_arr[i][j];


        }
    }
}
void copy_sol_to_board(double *sol,int **result_arr, int N ) {
    int i, j, v;
    printf("301\n");
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            for (v = 0; v < N; v++) {
                printf("i=%d,j=%d,v=%d\n", i, j, v);
                if (sol[i * N * N + j * N + v] == 1) {
                    printf("307\n");
                    result_arr[i][j] = v + 1;
                    printf("309\n");
                } else { printf("not enter to if\n"); }
            }
        }
    }
}