
#include <stdio.h>
#include "Commmand.h"
#include <assert.h>
#include <ctype.h>
#include "saving.h"

*Command create_new_command(enum command_name name ,int *args_arr )/* to check I took the enum value in the right way */
{
    Command *new_command =(*Command)malloc(sizeof(Command));
    new_command->name = name;
    new_command->arg_arr = args_arr;
    new_command->num_of_args = int(name);
    return new_command;
}