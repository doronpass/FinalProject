//
// Created by Doron_Passal on 27/08/2018.
//

#include <stdio.h>
#include "Play.h"
#include "Game.h"
#include "Error_handler.h"
#include <string.h>

void do_solve (char *x){
    create_new_command("SOLVE",)
}

void do_solve (char *x){}

void do_edit(char *x){}

void do_mark_errors (char *x){}

void do_print_board (char *x){}

void do_set (char *x){}

void do_validate (char *x){}

void do_generate (char *x){}

void do_undo (Game *game){
    if (game->doubly_linked_list->doubly_linked_list_size<1){
        nothing_to_undo();
    }
    else {
        if (strcmp(game->doubly_linked_list->last->node_data->command_name,"set"){

        }
        else{
            if (strcmp(game->doubly_linked_list->last->node_data->command_name,"autofull"){
            }
            else if (strcmp(game->doubly_linked_list->last->node_data->command_name,"generate"){
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

void do_solve (char *x){}