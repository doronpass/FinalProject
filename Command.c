//
// Created by Doron_Passal on 24/08/2018.
//
#include <stdio.h>
#include <stdlib.h>
#include "Command.h"

Command* create_new_command(enum command_name name ,int *args_arr )/* to check I took the enum value in the right way */
{
    int i;
    int j=0;
    Command *new_command =(Command*)malloc(sizeof(Command));
    new_command->name = name;
    for (i = 0; i < 3; ++i) {
        new_command->arg_arr[i] = args_arr[i];
    }
    new_command->num_of_args = 99999; // to take enum nueric value
    return new_command;
}

