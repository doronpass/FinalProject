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
    int size;
    Stack_Node *top;

}Stack;




#endif //FINALPROJECT_STACK_H
