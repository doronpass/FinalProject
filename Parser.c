#include <stdint.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "Parser.h"
#include "Game_board.h"
#include "Play.h"
#include "Error_handler.h"
#include "Functionality.h"
#include "stack.h"

/* read the user command and arguments and calls the function that performs the command with
 * the given arguments. 3 commands are checked inside the function, the rest
 * are checked inside the "execute_function" function, which is called at the end of this one*/
int user_turn(Game *my_game) {
    int x = -5, y = -5, z = -5, i = 0,ch;
    char *command_name;
    char *token = NULL, input[1024] = "", delimiter[] = " \t\r\n";
    while (token == NULL) {
        if (feof(stdin)){
            free_all_mem(my_game);
            return 2; /*exiting game */
        }
        printf("Enter your command:\n");
        fgets(input, 270, stdin);
        while (strlen(input)>256){
            invalid_command();
            fflush(stdin);
            while ((ch = getchar()) != '\n' && ch != EOF) {} /* to clean the buffer */
            strcpy(input, "");
            printf("Enter your command:\n");
            fgets(input, 270, stdin);
        }
        token = strtok(input, delimiter);
    }
    if (strlen(input) == 1) {
        user_turn(my_game);
    }
    command_name = token;
    token = strtok(NULL, delimiter);
    /* the following commands take a "string", the rest of them takes int as arguments and will be
     * handled in another function*/
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
                y = is_number(token);
                if (y!=-1){ /*if y is a number */
                    y--;
                }
            } else if (i == 1) {
                x = is_number(token);
                if (x!=-1){ /*if x is a number */
                    x--;
                }
            } else if (i == 2) {
                z = is_number(token);
            }
            i++;
            token = strtok(NULL, delimiter);
        } fflush(stdin);
    }
    return (execute_function(my_game, command_name, x,y,z));
}

/* returns the number typed by the user as int, or -3 if not a number */
int is_number(char *str){
    int i;
    int mult = 1;
    int number = 0;
    for (i=(int) strlen(str) ; i>0 ;i--){
        if (isdigit(str[i-1])==0) {
            return -3;
        }
        number += mult*(str[i-1]-'0');
        mult*=10;
    }
    return number;
}

/* compare the command name and args to valid commands and call the correct function to perform the command
 * or informs the user that the command is invalid. */
int execute_function(Game *my_game, char *command_name, int x, int y, int z){
    int autofill_change=0, set_complete = 0, generate_complete = 0 , val_res=0;
    Node *node;
    if (strcmp(command_name, "mark_errors")==0){
        if (y==-5){
            invalid_command();
        } else {
            mark_errors(my_game,y);
        }
    } else if (strcmp(command_name, "print_board")==0){
        print_user_board(my_game);
    } else if (strcmp(command_name, "set")==0){
        if (x!=-5 && y!=-5 && z!=-5){
            node = create_new_node("set");
            set_complete = set(my_game, x, y, z, node);
            if (set_complete){
                append_node_to_list(my_game->doubly_linked_list, node);
                print_user_board(my_game);
            } else {
                free_node(node);
            }
        } else {
            invalid_command();
            return 0;
        }
    } else if (strcmp(command_name, "validate")==0){
        val_res = validate(my_game);
        if(val_res == 2 ){
            puzzle_solution_erroneus();
        }
        else if (val_res == 1) {
            validation_passed ();
        }
        else if(val_res == 0){
            validation_failed();
        }
    } else if (strcmp(command_name, "generate")==0 && my_game->mode==0) {
        if (x!=-5 && y!=-5){
            node = create_new_node("generate");
            generate_complete = generate(my_game,node,(y+1),(x+1));
            if (generate_complete == 1){
                append_node_to_list(my_game->doubly_linked_list, node);
                print_user_board(my_game);
            } else {
                free_node(node);
            }
        } else {
            invalid_command();
            return 0;
        }
    } else if (strcmp(command_name, "undo")==0) {
        undo(my_game);
    } else if (strcmp(command_name, "redo")==0) {
        redo(my_game);
    } else if (strcmp(command_name, "hint")==0) {
        if (x!=-5 && y!=-5){
            hint(my_game,x,y);
        } else {
            invalid_command();
        }
    } else if (strcmp(command_name, "num_solutions")==0) {
        if (exhaustive_backtracking(my_game)==-1){
            puzzle_solution_erroneus();
        }
    } else if (strcmp(command_name, "autofill")==0 && my_game->mode==1) {
        node = create_new_node("autofill");
        autofill_change=autofill(my_game, node);
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
    if (my_game->mode==1 && is_game_over(my_game)==1){
        free_all_mem(my_game);
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
    int ch=0;
    char *command_name;
    char *token = NULL;
    char input[1024] = "";
    char delimiter[] = " \t\r\n";
    while (token == NULL) {
        if (feof(stdin)){
            return 2; /*exiting game */
        }
        printf("Enter your command:\n");
        fgets(input, 1024, stdin);
        while (strlen(input)>256){
            invalid_command();
            fflush(stdin);
            while ((ch = getchar()) != '\n' && ch != EOF) {} /* to clean the buffer */
            strcpy(input, "");
            printf("Enter your command:\n");
            fgets(input, 270, stdin);
        }
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
            return 1;
        } else {
            init_game(command_name, token, my_game, is_there_old_game);
           if (my_game->mode==-1){/* loading from file unsuccessful */
               return 1;
           } else {
               return 0;
           }
        }
    } else if (strcmp(command_name, "edit")==0){
        init_game(command_name, token, my_game, is_there_old_game);
        if (my_game->mode==-1){/* loading from file unsuccessful */
            return 1;
        } else {
            return 0;
        }
    } else if (strcmp(command_name, "exit")==0) {
        if (is_there_old_game==1){
            free_all_mem(my_game);
        }
        printf("Exiting...\n");
        return 2;
    } else {
        invalid_command();
    }
    return 1;
}

/* checks if the board is full and contains no erroneous values.
 * if it is, the user won and the game is over
 * returns 1 so that the user will go back to "init mode" */
int is_game_over(Game *my_game){
    int i,j, has_errors = 0;
    for (i=0;i<my_game->m_mult_n;i++) {
        for (j = 0; j < my_game->m_mult_n; j++) {
            if (my_game->user_game_board[i][j].value == 0) {
                /*board not full */
                return 0;
            }
            if(my_game->user_game_board[i][j].is_error == 1) {
                /*board has at least one error */
                has_errors = 1;
            }
        }
    }
    if(has_errors){
        printf("Puzzle solution erroneous\n");
        return 0;
    } else {
        printf("Puzzle solved successfully\n");
        return 1;
    }
}