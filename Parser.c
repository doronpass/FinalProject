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
    char *token = NULL;
    char input[1024] = "";
    char delimiter[] = " \t\r\n";
    Command *command;
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
    /* the following commands take a string arg, the rest take ints*/
    if (strcmp(command_name, "save")==0) {
        if (token == NULL) {
            invalid_command();
        } else {
            save_game(my_game,token);
        }
    } else if (strcmp(command_name, "solve")==0) {
        if (token == NULL) {
            invalid_command();
        } else {
            Game *new_game = init_game(command_name, token);
        }
    } else if (strcmp(command_name, "edit")==0){
        Game *new_game = init_game(command_name, token);
    }
    else {
        while (token != NULL && i < 3) {
            if (is_number(token) == -1){
                not_in_range(my_game->m_mult_n);
                return 0;
            }
            if (i == 0) {
                x = token[0] - '0';
                x--;
            } else if (i == 1) {
                y = token[0] - '0';
                y--;
            } else if (i == 2) {
                z = token[0] - '0';
            }
            i++;
            token = strtok(NULL, delimiter);
        }
        fflush(stdin);
    }
    if (strcmp(command_name, "mark_errors")==0){
        if (x == 0 || x ==1){
            mark_errors(my_game);
        } else {
            printf("Error: the value should be 0 or 1\n");
        }
    } else if (strcmp(command_name, "print_board")==0){
        /* ----------------------- PRINT BOARD!!! -------------------------*/
    } else if (strcmp(command_name, "set")==0){
        if ()
    }

    return 0;
}

/* returns the number typed by the user as int, or -1 if not a number */
int is_number(char *str){
    int i;
    int mult = 1;
    int number = 0;
    for (i=(int) strlen(str) ; i>0 ;i--){
        if (isdigit(str[i])==0) {
            return -1;
        }
        number += mult*(str[i] - '0');
        mult*=10;
    }
    return number;

}