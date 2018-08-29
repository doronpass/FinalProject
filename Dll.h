//
// Created by Doron_Passal on 23/08/2018.
//

#ifndef FINALPROJECT_DLL_H
#define FINALPROJECT_DLL_H
#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    char* command_name;
    int x;
    int y;
    int value;
    int prev_value;
    int autofill_counter;
} Command;

typedef struct Node {
    struct Node *next;
    struct Node *prev;
    Command *node_data;

} Node;


typedef struct
{
    struct Node *first;
    struct Node *last;
    int doubly_linked_list_size;
} Doubly_linked_list;


void add_last(Doubly_linked_list *dll, Command *node_data);
Doubly_linked_list* create_new_dll();
void remove_last (Doubly_linked_list *dll);
Command* create_new_command(char* command_name ,int *args_arr ,int num_of_args);


#endif // FINALPROJECT_DLL_H
