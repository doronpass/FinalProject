#include <stdint.h>
#include <stdio.h>
#include <mem.h>
#include <ctype.h>
#include "Parser.h"
#include "Game.h"
#include "Error_handler.h"
#include "Functionality.h"
#include "stack.h"


int user_turn(Game *my_game) {
    int x = -1, y = -1, z = -1, i = 0;
    char *command_name;
    char *token = NULL, input[1024] = "", delimiter[] = " \t\r\n";
    while (token == NULL) {
        printf("Enter your command:\n");
        fgets(input, 1024, stdin);
        token = strtok(input, delimiter);
    }
    if (strlen(input) == 1) {
        user_turn(my_game);
    }
    command_name = token;
    token = strtok(NULL, delimiter);
    /* the following commands take a "string", the rest take ints*/
    if (strcmp(command_name, "save")==0) {
        if (token == NULL) {
            invalid_command();
            return 0;
        } else {
            save_game(my_game,token);
            return 0;
        }
    } else if (strcmp(command_name, "solve")==0) {
        if (token == NULL) {
            invalid_command();
            return 0;
        } else {
            init_game(command_name, token, my_game,1);
            return 0;
        }
    } else if (strcmp(command_name, "edit")==0){
        init_game(command_name, token, my_game,1);
        return 0;
    }
    else {
        while (token != NULL && i < 3) {
            if (i == 0) {
                x = is_number(token);
                if (x==-1){
                    not_in_range(my_game->m_mult_n);
                    return 0;
                }
                x--;
            } else if (i == 1) {
                y = is_number(token);
                if (y==-1){
                    not_in_range(my_game->m_mult_n);
                    return 0;
                }
                y--;
            } else if (i == 2) {
                z = is_number(token);
                if (z == -1){
                    not_in_range(my_game->m_mult_n);
                    return 0;
                }
            }
            i++;
            token = strtok(NULL, delimiter);
        } fflush(stdin);
    }
    return (execute_function(my_game, command_name, x,y,z));
}

/* returns the number typed by the user as int, or -1 if not a number */
int is_number(char *str){
    int i;
    int mult = 1;
    int number = 0;
    for (i=(int) strlen(str) ; i>0 ;i--){
        if (isdigit(str[i-1])==0) {
            return -1;
        }
        number += mult*(str[i-1]-'0');
        mult*=10;
    }
    return number;
}

/* compare the command name and args to valid commands and performs the command
 * or informs the user the command is invalid.
 * created mainly due to the length of "user_turn" function */
int execute_function(Game *my_game, char *command_name, int x, int y, int z){
    int autofill_change=0;
    Node *node;
    if (strcmp(command_name, "mark_errors")==0){
        mark_errors(my_game);
    } else if (strcmp(command_name, "print_board")==0){
        print_user_board(my_game);
    } else if (strcmp(command_name, "set")==0){
        node = create_new_node("set");
        set(my_game, x, y, z, node);
        append_node_to_list(my_game->doubly_linked_list, node);
        print_user_board(my_game);
    } else if (strcmp(command_name, "validate")==0){
        /*---------------- VALIDATE! ------------------------------------*/
    } else if (strcmp(command_name, "generate")==0) {
        if (x<= (my_game->m_mult_n * my_game->m_mult_n) && y <= (my_game->m_mult_n * my_game->m_mult_n)){
            /* ------------------ GENERATE XY ---------------------------*/
        } else {
            not_in_range(my_game->m_mult_n*my_game->m_mult_n);
            return 0;
        }
    } else if (strcmp(command_name, "undo")==0) {
        undo(my_game);
    } else if (strcmp(command_name, "redo")==0) {
        redo(my_game);
    } else if (strcmp(command_name, "hint")==0) {
        if (x<my_game->m_mult_n && y<my_game->m_mult_n){
            /* ----------------------- HINT --------------------------------*/
        } else {
            not_in_range(my_game->m_mult_n);
        }
    } else if (strcmp(command_name, "num_solutions")==0) {
        exhaustive_backtracking(my_game);
    } else if (strcmp(command_name, "autofill")==0) {
        node = create_new_node("autofill");
        autofill_change=autofill(my_game, node);
        /* according to forum, if autofill made no changes to the board, it should not be added to dll */
        if (autofill_change==1){
            append_node_to_list(my_game->doubly_linked_list, node);
        } else {
            free_node(node);
        }
        print_user_board(my_game);
    } else if (strcmp(command_name, "reset")==0) {
        reset(my_game);
    } else if (strcmp(command_name, "exit")==0) {
        free_all_mem(my_game);
        printf("Exiting...\n");
        return 2;
    } else {
        invalid_command();
    }
    if (is_game_over(my_game)){
        /* user solved the board - game over */
        return 1;
    }
    return 0;
}

/*takes user input while game is in "init mode"
 * handles only "solve", "edit" or "exit" commands
 * return 0 for a valid command (solve/edit with proper args),
 * 1 for invalid command
 * 2 for exit command */
int init_user_turn(Game *my_game,int is_there_old_game){
    char *command_name;
    char *token = NULL;
    char input[1024] = "";
    char delimiter[] = " \t\r\n";
    while (token == NULL) {
        printf("Enter your command:\n");
        fgets(input, 1024, stdin);
        token = strtok(input, delimiter);
    }
    if (strlen(input) == 1) {
        init_user_turn(my_game, is_there_old_game);
    }
    command_name = token;
    token = strtok(NULL, delimiter);
    if (strcmp(command_name, "solve")==0) {
        if (token == NULL) {
            invalid_command();
            return 0;
        } else {
            init_game(command_name, token, my_game, is_there_old_game);
            return 0;
        }
    } else if (strcmp(command_name, "edit")==0){
        init_game(command_name, token, my_game, is_there_old_game);
        return 0;
    } else if (strcmp(command_name, "exit")==0) {
        if (my_game!=NULL){
            free_all_mem(my_game);
        }
        printf("Exiting...\n");
        return 2; //for exit
    } else {
        invalid_command();
    }
    print_user_board(my_game); /* ------------------- for testing only! remove later ------------*/
    return 1;
}

/* checks if the board is full and contains no erroneous values - if so, the user won and the game is over
 * returns 1 if the game is over and 0 else. */
int is_game_over(Game *my_game){
    int i,j;
    for (i=0;i<my_game->m_mult_n;i++) {
        for (j = 0; j < my_game->m_mult_n; j++) {
            if (my_game->user_game_board[i][j].value == 0 || my_game->user_game_board[i][j].is_error == 1) {
                return 0;
            }
        }
    }
    return 1;
}