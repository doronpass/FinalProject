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

/* master developer error handling, Google come hire me */
void invalid_command(){
    printf("ERROR: invalid command\n");
}