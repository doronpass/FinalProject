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
    stack->top = NULL;
    return stack;
}

int is_empty(Stack *stack) {
    if(stack->top == NULL)
        return 1;
    else
        return 0;
}

Stack_Node * pop(Stack *stack) {
    Stack_Node * stack_node = stack->top;
    if(!is_empty(stack)) {
        stack->top = stack->top->prev;
        stack->top->next = NULL;
        return stack_node;
    }
}

void push(Stack *stack, Stack_Node *node) {
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

void free_stack_node(Stack_Node *node, int size){
    int i;
    for (i=0;i<size;i++){
        free(node->board[i]);
    }
    free(node->board);
    free(node);
}

/* ------------------------------------ think about the "free()", where it should be for node or new_node -----*/
int exhaustive_backtracking(Game *my_game){
    int i,j,k,num_valid, num_sols = 0;
    Stack *stack = create_stack();
    Stack_Node *node = create_first_stack_node(my_game);
    Stack_Node *new_node;
    push(stack, node);
    if (check_if_erroneous(my_game)){
        puzzle_solution_erroneus();
        return -1; /* ------------------------ remember to check if returns -1 then do nothing */
    }
    for (i=0;i<my_game->m_mult_n;i++){
        for (j=0;j<my_game->m_mult_n;j++){
            if(is_empty(stack)){
                break;
            }
            if (stack->top->board[i][j]==0){
                node = pop(stack);
                num_valid = 0;
                for (k=1;k<=my_game->m_mult_n;k++){
                    if(is_valid_ints(node->board,my_game->m_block_rows, my_game->n_block_cols,i,j, k)){
                        num_valid++;
                        new_node=create_stack_node(my_game->m_mult_n, node);
                        new_node->board[i][j] = k;
                        push(stack,new_node);
                    }
                }
                if(num_valid==0){
                    i=0;
                    j=0;
                }
                free_stack_node(node,my_game->m_mult_n);
            }
            /* if we got to the last cell in the matt and filled it with a number,
             * then we have a valid solution to the board. increment the counter and free the node at the top of the
             * stack. note that the last cell will not have more then 1 valid number, so looking only on the
             * top node is valid */
            if(i==(my_game->m_mult_n-1)&& j==(my_game->m_mult_n-1) && stack->top->board[i][j]){
                num_sols++;
                free_stack_node(pop(stack),my_game->m_mult_n );
            }
        }
    }
    free(stack);
}


