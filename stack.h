#ifndef FINALPROJECT_STACK_H
#define FINALPROJECT_STACK_H

#include "Dll.h"
#include "Game.h"

typedef struct Stack_Node {
    int **board;
    struct Stack_Node *next;
    struct Stack_Node *prev;

}Stack_Node;

typedef struct Stack {
    Stack_Node *top;
}Stack;


Stack_Node * create_first_stack_node(Game *my_game);
Stack_Node * create_stack_node(int size, Stack_Node *input_node);
Stack * create_stack();
int is_empty(Stack *stack);
Stack_Node * pop(Stack *stack);
void push(Stack *stack, Stack_Node *node);
int check_if_erroneous(Game *my_game);
int is_valid_ints(int** board,int m, int n, int x, int y, int z);
void free_stack_node(Stack_Node *node, int size);
int exhaustive_backtracking(Game *my_game);

#endif //FINALPROJECT_STACK_H
