/* this module includes the structs and functions needed to implement a doubly linked list of moves.
 * during the game, the user can undo (and then redo) moves he did that changed the board.
 * note that a "move" is all the changes made to the board by a single command, so undoing (or redoing) a move
 * can change several values on the board */

#ifndef FINALPROJECT_DLL_H
#define FINALPROJECT_DLL_H
#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    int row;
    int col;
    int value;
    int prev_value;
} Data;

typedef struct Node {
    struct Node *next;
    struct Node *prev;
    char* command_name;
    Data **node_data;
    int node_data_size;
} Node;


typedef struct
{
    struct Node *first;
    struct Node *last;
    struct Node *dll_pointer;
    int doubly_linked_list_size;
} Doubly_linked_list;


Node * create_new_node(char* command_name);
Data* create_new_data (int row ,int col ,int value, int prev_value);
void append_node_to_list(Doubly_linked_list *dll, Node *node);
void remove_last (Doubly_linked_list *dll); /* inside it remove node, with all the free necessery */
void free_node(Node *node);
void append_data_to_node(Node *node,Data *data);/* add new data to the data array in node*/
Doubly_linked_list* create_new_dll();
#endif
