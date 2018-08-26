ndef FINALPROJECT_COMMMAND_H
#define FINALPROJECT_COMMMAND_H
enum command_name {
    solve = 1 ,
    edit = 1,
    mark_errors = 1 ,
    print_board = 0,
    set = 3,
    validate = 0 ,
    generate = 2,
    undo = 0,
    redo = 0,
    save = 1,
    hint = 2,
    num_solutions = 0,
    autofill = 0,
    reset = 0,
    exit = 0,
        };

typedef struct
{
    enum command_name name;
    int arg_arr[3]; // to make sure it with zeroes
    int num_of_args;
    //to enter var to make arg

} Command;

*Command create_new_command(enum command_name name ,int *args_arr ;
#endif //FINALPROJECT_COMMMAND_H
