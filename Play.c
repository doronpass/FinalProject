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

void do_solve (char *x){
}

void do_edit(char *x){}

void do_mark_errors (char *x){}

void do_print_board (char *x){}

void do_set (Game *game,x,y,z){}

void do_validate (char *x){}

void do_generate (char *x){}

void do_undo (Game *game ,int x, int y , int value , int pre_value){ // i need to add to all dll constructors and dunctions the dll_pointer logic
    Doubly_linked_list *p = game->doubly_linked_list;
    if (p->doubly_linked_list_size<1 || p->dll_pointer ==NULL){
        nothing_to_undo();
    }
    else {
        if (strcmp(p->dll_pointer->node_data->command_name,"set")){
            p->dll_pointer = p->dll_pointer->prev;
            do_set(game,x,y,pre_value);
            print_user_board(game);
            undo_print(x,y,pre_value,value);
        }
        else{
            if (strcmp(p->dll_pointer->node_data->command_name,"autofull")){
            }
            else if (strcmp(p->dll_pointer->node_data->command_name,"generate")){
            }
         }
    }
}

void do_redo (char *x){}

void do_save (char *x){}

void do_hint (char *x){}

void do_num_soulutions (char *x){}

void do_autofill (char *x){}

void do_reset (char *x){}

void do_exit (char *x){}

