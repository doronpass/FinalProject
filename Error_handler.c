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

/* same as the above function, only for an array of ints */
void check_memory_int(int* point){
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

/* same as the above function, only for ints */
void check_memory2_int(int** point) {
    if (point == NULL) {
        printf("Error: calloc has failed\n");
        exit(0);
    }
}
/* prints "ERROR: invalid command" when needed */

void invalid_command(){
    printf("ERROR: invalid command\n");
}
/* prints Error: value not in range when needed */

void not_in_range(int N){
    printf("Error: value not in range 0-%d\n",N);
}
/* prints "Error: cell is fixed when needed */

void cell_is_fixed(){
    printf("Error: cell is fixed\n");
}
/* prints Error: board contains erroneous values when needed */

void puzzle_solution_erroneus(){
    printf("Error: board contains erroneous values\n");
}
/* prints  Error: board is unsolvable when needed */

void board_not_solvable(){
    printf("Error: board is unsolvable\n");
}
/* prints Validation passed: board is solvable when needed */

void validation_passed(){
    printf("Validation passed: board is solvable\n");
}

/* prints Validation failed: board is unsolvable when needed */

void validation_failed(){
    printf("Validation failed: board is unsolvable\n");
}
/* prints Error: board is not empty when needed */

void board_not_empty(){
    printf("Error: board is not empty\n");
}
/* prints Error: puzzle generator failed when needed */

void puzzle_generator_failed(){
    printf("Error: puzzle generator failed\n");

}
/* printsError: cell already contains a value when needed */

void cell_contains_value(){
    printf("Error: cell already contains a value\n");

}
