//
// Created by Doron_Passal on 23/08/2018.
//
//
// Created by Doron_Passal on 22/08/2018.
//

#ifndef UNTITLED_MAIN_H
#define UNTITLED_MAIN_H

#endif //UNTITLED_MAIN_H
typedef struct
{
    int value;
    int is_fix;
    int is_error;
} Cell;

typedef struct
{
    enum command_name;
    int arg_arr[3]; // to make sure it with zeroes
    int num_of_args;
    //to enter var to make arg

} Command;

typedef struct
{
    struct Node *p_first;
    struct Node*p_last;
    int Doubly_linked_list_size;


} Doubly_linked_list;

struct Node {
    Command *data;
    struct Node *next;
    struct Node *prev;
};

typedef struct
{
    Cell **user_game_board;
    Cell **solved_game_board;
    int mark_error;// 0 do not mark, 1 do mark
    int mode; // 0 if edit , 1 if solve
    int m_block_rows;
    int n_block_cols;
    int m_mult_n; // for conv
    Doubly_linked_list *p_Doubley_linked_list;
} Game;


#ifndef FINALPROJECT_MAIN_H
#define FINALPROJECT_MAIN_H

#endif //FINALPROJECT_MAIN_H
