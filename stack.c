#include "stack.h"
#include "Game_board.h"
#include "Functionality.h"
#include "Error_handler.h"

/* creates the first stack node, it's board is a duplicate of the user board
 * of the my_game input*/
Stack_Node * create_first_stack_node(Game *my_game){
    int i,j, size = my_game->m_mult_n;
    Stack_Node *node = (Stack_Node *) malloc(sizeof(Stack_Node));
    if (node==NULL){
        printf("Error: malloc has failed\n");
        exit(0);
    }
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
    node->prev=NULL;
    node->next=NULL;
    return node;
}

/* creates a new stack node, it's board will be a duplicate of the board of the input node */
Stack_Node * create_stack_node(int size, Stack_Node *input_node){
    int i,j;
    Stack_Node *node = (Stack_Node *) malloc(sizeof(Stack_Node));
    if (node==NULL){
        printf("Error: malloc has failed\n");
        exit(0);
    }
    node->next = NULL;
    node->prev=NULL;
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
    return node;
}

/* create an empty stack */
Stack * create_stack(){
    Stack *stack = (Stack *) malloc(sizeof(Stack));
    if (stack==NULL){
        printf("Error: malloc has failed\n");
        exit(0);
    }
    stack->top = NULL;
    return stack;
}

/* if stack is empty, return 1, else return 0 */
int is_empty(Stack *stack) {
    if(stack->top == NULL)
        return 1;
    else
        return 0;
}

/* remove the item at the top of the stack from the stack, and return it */
Stack_Node * pop(Stack *stack) {
    Stack_Node * stack_node = stack->top;
    stack->top = stack->top->prev;
    if (is_empty(stack)==0){
        stack->top->next = NULL;
    }
    return stack_node;
}

/* push an item to the top of the stack */
void push(Stack *stack, Stack_Node *node) {
    if (is_empty(stack)==0){
        stack->top->next = node;
        node->prev = stack->top;
    }
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
/* same function as "is_valid", but input is a matrix and some ints, rather than
 * a Game struct - used for backtracking */
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

/* free all memory allocated to a stack node */
void free_stack_node(Stack_Node *node, int size){
    int i;
    for (i=0;i<size;i++){
        free(node->board[i]);
    }
    free(node->board);
    free(node);
}


/* function for testing only! -------------------------------------------------------
void print_test(Game *game, int** matrix){
    int i, j, size;
    if (game == NULL){
        exit(1);
    }
    size = game->m_mult_n;
    for (i = 0; i < size; ++i) {
        if ((i % (game->n_block_cols)) == 0) {
            print_separator_row(size,game->n_block_cols);
        }
        for ( j = 0; j < size; ++j) {
            if ((j % (game->m_block_rows)) == 0) {
                printf(PIPE);
            }
            printf("%s", " ");
            if ((matrix[i][j]) == 0){
                printf("%s","  ");
            } else {
                printf("%2d",matrix[i][j]);
            }
        }
        printf("%s\n",PIPE);
    }
    print_separator_row(size,game->n_block_cols);
    printf("new_board_here\n");
}
*/



/* ------------------------------------ think about the "free()", where it should be for node or new_node -----*/
int exhaustive_backtracking(Game *my_game){
    int i,j,k,num_valid, num_sols = 0;
    Stack *stack = create_stack();
    Stack_Node *node = create_first_stack_node(my_game);
    Stack_Node *new_node;
    push(stack, node);

    if (check_if_erroneous(my_game)){

        puzzle_solution_erroneus();
        free_stack_node(node,my_game->m_mult_n );
        free(stack);
        return -1;
    }
    for (j=0;j<my_game->m_mult_n;j++){ /* changed from i to j */
        if(is_empty(stack)){

            break;
        }
        for (i=0;i<my_game->m_mult_n;i++){/* changed from j to i */
            if(is_empty(stack)){

                break;
            }
            if (stack->top->board[j][i]==0){ /* swiched between i and j */

                node = pop(stack);
                num_valid = 0;
                for (k=1;k<=my_game->m_mult_n;k++){
                    if(is_valid_ints(node->board,my_game->n_block_cols, my_game->m_block_rows,j,i, k)){ /* swiched between i and j  and n with m*/
                        num_valid++;
                        new_node=create_stack_node(my_game->m_mult_n, node);
                        new_node->board[j][i] = k; /*change between i and j */
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
                i=0;
                j=0;
            }
        }
    }
    free(stack);
    printf("Number of solutions: %d\n", num_sols);
    if (num_sols==1){
        printf("This is a good board!\n");
    } else if (num_sols>1){
        printf("The puzzle has more than 1 solution, try to edit it further\n");
    }
    return num_sols;
}



