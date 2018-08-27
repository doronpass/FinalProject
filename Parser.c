#include <stdint.h>
#include <stdio.h>
#include <mem.h>
#include "Parser.h"
#include "Game.h"
#include "Error_handler.h"
#include "Functionality.h"

int user_turn(Game *my_game) {
    int x = -1, y = -1, z = -1, i = 0;
    char *command;
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
    command = token;
    token = strtok(NULL, delimiter);
    if (strcmp(command, "save")==0) {
        if (token == NULL) {
            invalid_command();
        } else {
            save_game(my_game,token);
        }
    } else if (strcmp(command, "solve")==0) {
        if (token == NULL) {
            invalid_command();
        } else {
            Game *new_game = init_game(command, token);
        }
    } else if (strcmp(command, "edit")==0){
        Game *new_game = init_game(command, token);
    }

    while (token != NULL && i < 3) {
        if (i == 0) {
            y = token[0] - '0';
            y--;
        } else if (i == 1) {
            x = token[0] - '0';
            x--;
        } else if (i == 2) {
            z = token[0] - '0';
        }
        i++;
        token = strtok(NULL, delimiter);
    }
    fflush(stdin);
   /* ------------------add function to check and react if gameover ------ */

    return 0;
}
