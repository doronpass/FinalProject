//
// Created by Doron_Passal on 27/08/2018.
//
#include <stdio.h>
#include <stdlib.h>
#include "Error_handler.h"
/*tests whether calloc failed
 * if so, alert the user and terminate program*/
void check_memory(Cell* point){
    if (point == NULL) {
        printf("Error: calloc has failed\n");
        exit(0);
    }
}

/*tests whether calloc failed
 * if so, alert the user and terminate program - not sure i should send a messege*/
void check_memory2(Cell** point) {
    if (point == NULL) {
        printf("Error: calloc has failed\n");
        exit(0);
    }
}

void invalid_command(){
    printf("ERROR: invalid command\n");
}

void not_in_range(int N){
    printf("Error: value not in range 0-%d\n",N);
}
void cell_is_fixed(){
    printf("Error: cell is fixed\n");
}
void puzzle_solution_erroneus(){
    printf("Error: board contains erroneous values\n");
}
void board_not_solvable(){
    printf("Error: board is unsolvable\n");
}
void validation_passed(){
    printf("Validation passed: board is solvable\n")
}


