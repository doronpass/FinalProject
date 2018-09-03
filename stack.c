#include "stack.h"
#include "Game.h"
#include "Functionality.h"
#include "Error_handler.h"

Stack * create_stack(){
    Stack *stack = (Stack *) malloc(sizeof(Stack));
    stack->size=0;
    stack->stk = malloc(sizeof(Data));
}

int is_empty(Stack *stack) {
    if(stack->size == 0)
        return 1;
    else
        return 0;
}

int peek(Stack *stack) {
    return stack->stk[stack->size-1].value;
}

Data pop(Stack *stack) {
    Data data;
    if(!is_empty(stack)) {
        stack->size -= 1;
        data = stack->stk[stack->size];
        stack->stk = realloc(stack->stk, stack->size* sizeof(Data));
        return data;
    }
}

int push(Stack *stack, Data data) {
    stack->size += 1;
    stack->stk = realloc(stack->stk, stack->size* sizeof(Data));
    stack->stk[stack->size] = data;

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

int exhaustive_backtracking(Game *my_game){
    int i,j;
    Game *clone = clone_game(my_game);
    Stack *stack =create_stack();
    if (check_if_erroneous(my_game)){
        puzzle_solution_erroneus();
        return -1; /* ------------------------ remember to check if returns -1 then do nothing */
    }
    for (i=0;i<my_game->m_mult_n;i++){
        for (j=0;j<my_game->m_mult_n;j++){
            if (clone->user_game_board[i][j].value==0){

            }
        }
    }
}


