//
// Created by Doron_Passal on 23/08/2018.
//

#include <stdio.h>
#include "Command.h"
#include "Dll.h"

#include <stdlib.h>

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

Doubly_linked_list* create_new_dll(){
    Doubly_linked_list *new_dll = (Doubly_linked_list*)malloc(sizeof(Doubly_linked_list));
    if (new_dll == NULL){
        printf("to say  ther's an errro , probably malloc . at memory allocation");
    }
    new_dll->doubly_linked_list_size = 0;
    new_dll->first = NULL;
    new_dll->last = NULL;
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
