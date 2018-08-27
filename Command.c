//
// Created by Doron_Passal on 24/08/2018.
//
#include <stdio.h>
#include <stdlib.h>
#include "Command.h"
#include <string.h>
Command* create_new_command(char* command_name,int *args_arr,int num_of_args )/* to check I took the enum value in the right way */
{
    int i;
    Command *new_command =(Command*)malloc(sizeof(Command));
    new_command->command_name = malloc(strlen(command_name)+1);
    strcpy(new_command->command_name, command_name);
    new_command->num_of_args = num_of_args;
    for (i = 0; i < 3; ++i) {
        new_command->arg_arr[i] = args_arr[i];
    }
    return new_command;
}

