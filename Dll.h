//
// Created by Doron_Passal on 23/08/2018.
//
#include "Commmand.h"

#ifndef FINALPROJECT_DLL_H
#define FINALPROJECT_DLL_H
typedef struct
{
    struct Node *first;
    struct Node *last;
    int doubly_linked_list_size;
} Doubly_linked_list;

typedef struct Node {
    struct Node *next;
    struct Node *prev;
    Command *node_data;

} Node;

void add_last(Doubly_linked_list *dll,Command *node_data); /* add command to redo-undo list*/
*Doubly_linked_list create_new_dll(); /* create new list, where all pointers set to NULL and num of args set to 0 */
void remove_last(Doubly_linked_list *dll);
#endif //FINALPROJECT_DLL_H
