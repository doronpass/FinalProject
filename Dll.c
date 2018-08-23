//
// Created by Doron_Passal on 23/08/2018.
//

#include "Dll.h"
//
// Created by Doron_Passal on 23/08/2018.
//
#import "main.h"
typedef struct Node Node;

void push_first(Node **head_refr,Comamand *p_data){
    Node* new_node =(Node*)malloc(sizeof(Node));
    new_node->data = p_data;
    new_node->next = *head_refr;
    new_node->prev = Null;
    if (*head_refr != NULL)
        *head_refr->prev = new_node;
    *head_refr = new_node;
}

void push_last(Node **tail_refr, Command *p_data){
    Node* new_node =(Node*)malloc(sizeof(Node));
    new_node->data = p_data;
    new_node->next = NULL;
    new_node->prev = *tail_refr;
    if (*tail_refr !=NULL){
        *tail_refr->next = new_node;
    }
    *tail_refr = new_node;
}