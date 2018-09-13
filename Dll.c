#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Dll.h"


/*crate now dll node ,node data array is empty */
Node * create_new_node(char* command_name){
    Node *node;
    node = malloc(sizeof(struct Node));
    if (node==NULL){
        printf("Error: malloc has failed\n");
        exit(4);
    }
    node->command_name = command_name;
    node->next = NULL;
    node->prev = NULL;
    node->node_data = NULL;
    node->node_data_size = 0;
    return node;
}
/* create new node data entry, which is the cell we change (row,col), the value after the change
 * and the value before the change (prev_value) */
Data* create_new_data (int row ,int col ,int value, int prev_value){
    Data *data = (Data *) malloc(sizeof(Data));
    if (data==NULL){
        printf("Error: malloc has failed\n");
        exit(4);
    }
    data->row = row;
    data->col = col;
    data->value = value;
    data->prev_value = prev_value;
    return data;
}

/* delete any node ahead of the node the dll_pointer points to,
 * then append the input node to the end of the linked list  */
void append_node_to_list(Doubly_linked_list *dll, Node *node){
    Node *prev_node = dll->dll_pointer;
    if (prev_node!=NULL) {
        while (prev_node->next != NULL){
            remove_last(dll);
        }
        node->prev = prev_node;
    }
    prev_node->next = node;
    dll->dll_pointer = node;
    dll->last = node;
    dll->doubly_linked_list_size++;
}


/* remove the last node in the dll */
void remove_last (Doubly_linked_list *dll){
    Node *to_delete;
    to_delete = dll->last;
    if (to_delete!=NULL){
        dll->last->prev->next = NULL;
        dll->last = dll->last->prev;
        dll->doubly_linked_list_size--;
        free_node(to_delete);
    }
}

/*free all memory space assigned to the input node */
void free_node(Node *node){
    int i;
    for(i=node->node_data_size; i>0;i--){
        free(node->node_data[i-1]);
    }
    free(node->node_data);
    free(node);
}

/* add a new data entry ( (row,col) of the cell, new value, old value) to the node data array */
void append_data_to_node(Node *node,Data *data){
    node->node_data_size++;
    node->node_data = (Data **) realloc(node->node_data, node->node_data_size* sizeof(Data));
    node->node_data[node->node_data_size-1] = data;
}

/* create a new dll, and add a "start node" to it */
Doubly_linked_list* create_new_dll(){
    Node *start_node;
    Doubly_linked_list *new_dll = (Doubly_linked_list*)malloc(sizeof(Doubly_linked_list));
    if (new_dll == NULL){
        printf("Error: malloc has failed\n");
        exit(3);
    }
    new_dll->doubly_linked_list_size = 1;
    start_node = create_new_node("start_node");
    new_dll->first = start_node;
    new_dll->last = start_node;
    new_dll->dll_pointer = new_dll->last;
    return new_dll;
}

