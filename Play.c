//
// Created by Doron_Passal on 27/08/2018.
//

#include <stdio.h>
#include "Play.h"
#include "Game.h"
#include "Error_handler.h"
#include <string.h>

void undo_print(int x,int y,int pre_value,int value) {

    if (value != 0 && pre_value != 0) {
        printf("Undo %d,%d :from %d to %d\n", x, y, pre_value, value);
    } else{
        if (value != 0 && pre_value == 0) {
            printf("Undo %d,%d :from _ to %d\n", x, y, value);
        }
        else{
            if (value == 0 && pre_value == 0) {
                printf("Undo %d,%d :from _ to _\n", x, y);
            }
            else {
                printf("Undo %d,%d :from %d to _\n", x, y,pre_value);
            }
        }
    }

}

void redo_print(int x,int y,int pre_value,int value) {

    if (value != 0 && pre_value != 0) {
        printf("Redo %d,%d :from %d to %d\n", x, y, pre_value, value);
    } else{
        if (value != 0 && pre_value == 0) {
            printf("Redo %d,%d :from _ to %d\n", x, y, value);
        }
        else{
            if (value == 0 && pre_value == 0) {
                printf("Redo %d,%d :from _ to _\n", x, y);
            }
            else {
                printf("Redo %d,%d :from %d to _\n", x, y,pre_value);
            }
        }
    }

}
void do_solve (char *x){
}

void do_edit(char *x){}

void do_mark_errors (char *x){}

void do_print_board (char *x){}

void do_validate (char *x){}

void do_generate (char *x){}

void do_undo (Game *game ,int x, int y , int value , int pre_value){ // i need to add to all dll constructors and dunctions the dll_pointer logic
    Doubly_linked_list *p = game->doubly_linked_list;
    if (p->doubly_linked_list_size<1 || p->dll_pointer ==NULL){
        nothing_to_undo();
    }
    else {
        if (strcmp(p->dll_pointer->node_data->command_name,"set")){ /* if it's regular set command */
            p->dll_pointer = p->dll_pointer->prev;
            do_set(game,x,y,pre_value);
            print_user_board(game);
            undo_print(x,y,value,pre_value);
        }
        else{
            if (strcmp(p->dll_pointer->node_data->command_name,"autofull")){ /* if it's autofillcommand */
                while(p->dll_pointer->node_data->autofill_counter != 0){ /* while op_number in the node is not 0 keep going back */
                    do_set(game,p->dll_pointer->node_data->x,p->dll_pointer->node_data->y,p->dll_pointer->node_data->prev_value); /* each time undo the current step */
                    p->dll_pointer = p->dll_pointer->prev; /* and go one back to keep checking */
                }
                do_set(game,p->dll_pointer->node_data->x,p->dll_pointer->node_data->y,p->dll_pointer->node_data->prev_value); /* also do the first to happen */
                while ((p->dll_pointer->node_data->autofill_counter == 0) || (!strcmp(p->dll_pointer->node_data->command_name,"autofull"))){ /* now moving forward to do all prints required */
                    undo_print(p->dll_pointer->node_data->x,p->dll_pointer->node_data->y,p->dll_pointer->node_data->value,p->dll_pointer->node_data->prev_value);
                    p->dll_pointer = p->dll_pointer->next;
                }
                while(p->dll_pointer->node_data->autofill_counter != 0){       /* while op_number in the node is not 0 keep going back */
                    p->dll_pointer = p->dll_pointer->prev;
                }
                p->dll_pointer = p->dll_pointer->prev;
                print_user_board(game); /* print board at the end of autofull undo*/
            }
            else if (strcmp(p->dll_pointer->node_data->command_name,"generate")){ /* stiil to do ********************/
            }
         }
    }
}

void do_redo (Game *game ,int x, int y , int value , int pre_value ){
    Doubly_linked_list *p = game->doubly_linked_list;
    if(game->doubly_linked_list == NULL);{
        nothing_to_redo();
    /* to check what to do in erros like that */
    }
    if(game->doubly_linked_list->dll_pointer ==game->doubly_linked_list->last ){
        nothing_to_redo();
    }
    else{

        /* game->doubly_linked_list->dll_pointer = game->doubly_linked_list->dll_pointer->next; */  /* check where to dll pointer rest */
        /* here i asuume the dll pointer is on the last command that was undone */

        if (strcmp(p->dll_pointer->node_data->command_name,"set")){ /* if it's regular set command */
            p->dll_pointer = p->dll_pointer->next;
            do_set(game,x,y,value);
            print_user_board(game);
            redo_print(x,y,pre_value,value);
        }
        else{
            if (strcmp(p->dll_pointer->node_data->command_name,"autofull")){ /* if it's autofillcommand */
                do_set(game,x,y,value);
                redo_print(x,y,pre_value,value);
                p->dll_pointer = p->dll_pointer->next;
                while((p->dll_pointer->node_data->autofill_counter != 0 && (strcmp(p->dll_pointer->node_data->command_name,"autofull"))))
                { /* while op_number in the node is not 0 keep going forward */
                    do_set(game,p->dll_pointer->node_data->x,p->dll_pointer->node_data->y,p->dll_pointer->node_data->value); /* each time redo the current step */
                    redo_print(p->dll_pointer->node_data->x,p->dll_pointer->node_data->y,p->dll_pointer->node_data->prev_value,p->dll_pointer->node_data->value);
                    p->dll_pointer = p->dll_pointer->next; /* and go one back to keep checking */
                }
                do_set(game,p->dll_pointer->node_data->x,p->dll_pointer->node_data->y,p->dll_pointer->node_data->value); /* also do the first to happen */
                redo_print(p->dll_pointer->node_data->x,p->dll_pointer->node_data->y,p->dll_pointer->node_data->prev_value,p->dll_pointer->node_data->value);
                print_user_board(game); /* print board at the end of autofull undo*/
            }
            else if (strcmp(p->dll_pointer->node_data->command_name,"generate")){
                /* stiil to do ********************/
            }
            }
        }


}


