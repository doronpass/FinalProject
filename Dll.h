//
// Created by Doron_Passal on 23/08/2018.
//

#ifndef FINALPROJECT_DLL_H
#define FINALPROJECT_DLL_H
#include "Command.h"

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

void add_last(Doubly_linked_list *dll, Command *node_data);
Doubly_linked_list* create_new_dll();
void remove_last (Doubly_linked_list *dll);

#endif // FINALPROJECT_DLL_H
