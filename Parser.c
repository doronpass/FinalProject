#include <stdint.h>
#include <stdio.h>
#include <mem.h>
#include <ctype.h>
#include "Parser.h"
#include "Game.h"
#include "Error_handler.h"
#include "Functionality.h"

int user_turn(Game *my_game) {
    int x = -1, y = -1, z = -1, i = 0;
    char *command_name;
    char *token = NULL, input[1024] = "", delimiter[] = " \t\r\n";
    Node *node;
    while (token == NULL) {
        printf("Enter your command:\n");
        fgets(input, 1024, stdin);
        token = strtok(input, delimiter);
    }
    if (strlen(input) == 1) {       //Test if this works with both Enter, and spaces then Enter
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
            init_game(command_name, token, my_game);
            return 0;
        }
    } else if (strcmp(command_name, "edit")==0){
        init_game(command_name, token, my_game);
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
    if (strcmp(command_name, "mark_errors")==0){
        if (x == 0 || x ==1){
            mark_errors(my_game);
        } else {
            printf("Error: the value should be 0 or 1\n");
            return 0;
        }
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
        /* ----------------------- UNDO --------------------------------*/
    } else if (strcmp(command_name, "redo")==0) {
        /* ----------------------- REDO --------------------------------*/
    } else if (strcmp(command_name, "hint")==0) {
        if (x<my_game->m_mult_n && y<my_game->m_mult_n){
            /* ----------------------- HINT --------------------------------*/
        } else {
            not_in_range(my_game->m_mult_n);
            return 0;
        }
    } else if (strcmp(command_name, "num_solutions")==0) {
        /* ----------------------- NUM_SOLUTIONS --------------------------------*/
    } else if (strcmp(command_name, "autofill")==0) {
        node = create_new_node("autofill");
        autofill(my_game, node);
        append_node_to_list(my_game->doubly_linked_list, node);
        print_user_board(my_game);
    } else if (strcmp(command_name, "reset")==0) {
        /* ----------------------- RESET --------------------------------*/
    } else if (strcmp(command_name, "exit")==0) {
        /* ----------------------- EXIT --------------------------------*/
        return 2; //for exit
    } else {
        invalid_command();
    }
    /* ----------------- test if game over, if so return 1, else return 0 --------------------*/
     return 0;
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
        number += mult*(str[i] - '0');
        mult*=10;
    }
    return number;
}


/*takes user input while game is in "init mode"
 * handles only "solve", "edit" or "exit" commands */
int init_user_turn(Game *my_game){
    char *command_name;
    char *token = NULL;
    char input[1024] = "";
    char delimiter[] = " \t\r\n";
    while (token == NULL) {
        printf("Enter your command:\n");
        fgets(input, 1024, stdin);
        token = strtok(input, delimiter);
    }
    if (strlen(input) == 1) {       //Test if this works with both Enter, and spaces then Enter
        user_turn(my_game);
    }
    command_name = token;
    token = strtok(NULL, delimiter);
    if (strcmp(command_name, "solve")==0) {
        if (token == NULL) {
            invalid_command();
            return 0;
        } else {
            init_game(command_name, token, my_game);
            return 0;
        }
    } else if (strcmp(command_name, "edit")==0){
        init_game(command_name, token, my_game);
        return 0;
    } else if (strcmp(command_name, "exit")==0) {
        /* ----------------------- EXIT --------------------------------*/
        return 2; //for exit
    } else {
        invalid_command();
    }
    return 0;
}