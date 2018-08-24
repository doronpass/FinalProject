//
// Created by Doron_Passal on 23/08/2018.
//

#include "Dll.h"
//
// Created by Doron_Passal on 23/08/2018.
//
#import "Strucjs_builder.h"

void add_last(Doubly_linked_list *dll, Comamand *node_data){
    Node* new_node =(Node*)malloc(sizeof(Node));
    new_node->data = node_data;
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

