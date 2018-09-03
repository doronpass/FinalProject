#include "stack.h"
#include "Game.h"
#include "Functionality.h"
#include "Error_handler.h"

/* creates the first stack node, it's board is a duplicate of the user board
 * of the my_game input*/
Stack_Node * create_first_stack_node(Game *my_game){
    int i,j, size = my_game->m_mult_n;
    Stack_Node *node = (Stack_Node *) malloc(sizeof(Stack_Node));
    node->board = (int **) calloc((size_t) size, sizeof(int *));
    check_memory2_int(node->board);
    for (i = 0; i < size; ++i) {
        node->board[i] = (int *) calloc((size_t)size, sizeof(int));
        check_memory_int(node->board[i]);
    }
    for (i=0;i<my_game->m_mult_n;i++) {
        for (j = 0; j < my_game->m_mult_n; j++) {
            node->board[i][j] = my_game->user_game_board[i][j].value;
        }
    }
}

/* creates a new stack node, it's board will be a duplicate of the board of the input node */
Stack_Node * create_stack_node(int size, Stack_Node *input_node){
    int i,j;
    Stack_Node *node = (Stack_Node *) malloc(sizeof(Stack_Node));
    node->next = NULL;
    node->board = (int **) calloc((size_t) size, sizeof(int *));
    check_memory2_int(node->board);
    for (i = 0; i < size; ++i) {
        node->board[i] = (int *) calloc((size_t)size, sizeof(int));
        check_memory_int(node->board[i]);
    }
    for (i=0;i<size;i++) {
        for (j = 0; j < size; j++) {
            node->board[i][j] = input_node->board[i][j];
        }
    }
}


Stack * create_stack(){
    Stack *stack = (Stack *) malloc(sizeof(Stack));
    stack->size=0;
    stack->top = NULL;
    return stack;
}

int is_empty(Stack *stack) {
    if(stack->top == NULL)
        return 1;
    else
        return 0;
}

int** pop(Stack *stack) {
    int **board = stack->top->board;
    if(!is_empty(stack)) {
        stack->size -= 1;
        stack->top = stack->top->prev;
        stack->top->next = NULL;
        return board;
    }
}

void push(Stack *stack, Stack_Node *node) {
    stack->size += 1;
    stack->top->next = node;
    node->prev = stack->top;
    stack->top = node;
}

/* checks if the user board contains erroneous values
 * returns 1 there are erroneous values, else returns 0 */
int check_if_erroneous(Game *my_game){
    int i,j;
    for (i=0;i<my_game->m_mult_n;i++) {
        for (j = 0; j < my_game->m_mult_n; j++) {
            if (my_game->user_game_board[i][j].is_error == 1) {
                return 1;
            }
        }
    }
    return 0;
}

int is_valid_ints(int** board,int m, int n, int x, int y, int z){
    int i, j, block_first_row, block_first_col;
    int N = n * m;
    /* search row (row is x) */
    for (i = 0; i < N; i++) {
        if ((i != y) && (board[x][i] == z)) {
            return 0;
        }
    }
    /* search col (col is y)*/
    for (j = 0; j < N; j++) {
        if ((j != x) && (board[j][y] == z))
            return 0;
    }
    /* search in block - dividing ints returns the floor value of the actual division */
    block_first_row = (x / m) * m;
    block_first_col = (y / n) * n;
    for (i = block_first_row; i < (block_first_row + m); i++) {
        for (j = block_first_col; j < (block_first_col + n); j++) {
            if (board[i][j] == z && (j != y || i != x)) {
                return 0;
            }
        }
    }
    return 1;
}


int exhaustive_backtracking(Game *my_game){
    int i,j,k;
    Stack *stack = create_stack();
    Stack_Node *node = create_first_stack_node(my_game);
    push(stack, node);
    if (check_if_erroneous(my_game)){
        puzzle_solution_erroneus();
        return -1; /* ------------------------ remember to check if returns -1 then do nothing */
    }
    for (i=0;i<my_game->m_mult_n;i++){
        for (j=0;j<my_game->m_mult_n;j++){
            if (stack->top->board[i][j]==0){
                node = pop(stack);
                for (k=1;k<=my_game->m_mult_n;k++){
                    if(is_valid_ints(node->board,my_game->m_block_rows, my_game->n_block_cols,i,j, k)){

                    }
                }
            }
        }
    }
}



