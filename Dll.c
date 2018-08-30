#include <stdlib.h>
#include <stdio.h>
#include <mem.h>
#include "Dll.h"

Node * create_new_node(char* command_name){
    Node *node = (Node *) malloc(sizeof(Node));
    node->next = NULL;
    node->prev = NULL;
    strcpy(node->command_name,command_name);
    node->node_data = NULL;
    node->node_data_size = 0;
    return node;
}
/*(Data *) malloc(sizeof(Data)) */
Data* create_new_data (int row ,int col ,int value, int prev_value){
    Data *data = (Data *) malloc(sizeof(Data));
    data->row = row;
    data->col = col;
    data->value = value;
    data->prev_value = prev_value;
    return data;
}

void append_node_to_list(Doubly_linked_list *dll, Node *node){
    Node *prev_node = dll->dll_pointer;

    while (prev_node->next != NULL){
        remove_last(dll);
    }
    node->prev = prev_node;
    if (dll->first == NULL){
        dll->first = node;
    } else {
        prev_node->next = node;
    }
    dll->dll_pointer = node;
    dll->last = node;
    dll->doubly_linked_list_size++;
}

void remove_last (Doubly_linked_list *dll){
    Node *to_delete;
    to_delete = dll->last;
    dll->last->prev->next = NULL;
    dll->last = dll->last->prev;
    dll->doubly_linked_list_size--;
    free_node(to_delete);
}

void free_node(Node *node){
    free(node->node_data);
    free(node);
}

void append_data_to_node(Node *node,Data *data){
    node->node_data_size++;
    node->node_data = (Data *) realloc(node->node_data, node->node_data_size* sizeof(Data));
    node->node_data[node->node_data_size] = *data;
}














































Node * create_new_node(){
    Node *new_node = (Node*)malloc(sizeof(Node));
    if (new_node == NULL){
        printf("to say  ther's an errro , probably malloc . at memory allocation");
    }
    new_node->node_data = NULL;
    new_node->prev = NULL;
    new_node->next = NULL;

    return new_node;
}

Doubly_linked_list* create_new_dll(){
    Doubly_linked_list *new_dll = (Doubly_linked_list*)malloc(sizeof(Doubly_linked_list));
    if (new_dll == NULL){
        printf("to say  ther's an errro , probably malloc . at memory allocation");
    }
    new_dll->doubly_linked_list_size = 0;
    new_dll->first = NULL;
    new_dll->last = NULL;
    new_dll->dll_pointer = new_dll->last;

    return new_dll;
}



void remove_last (Doubly_linked_list *dll) {
    if (dll == NULL || dll->first == NULL) { // if NULL passed to fun, return NULL ***OR MABYE ERROR??***

        return ;
    }
    Node *to_delete;
    to_delete = dll->last;
    dll->last->prev->next = NULL;
    dll->last = dll->last->prev;
    free(to_delete);
}


void add_last(Doubly_linked_list *dll, Command *node_data){
    Node* new_node =(Node*)malloc(sizeof(Node));
    new_node->node_data = node_data;
    new_node->next = NULL;
    new_node->prev = dll->last;
    if (dll->first == NULL)
    {dll->first = new_node;}
    if (dll->last !=NULL){
        dll->last = new_node;
    }
    dll->last = new_node;
    dll->doubly_linked_list_size++;

}