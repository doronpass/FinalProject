#include <stdio.h>
#include <stdlib.h>
#include "Game.h"
#include "Functionality.h"
#include "Parser.h"

/*starts the game and let user type commands
 * first loop takes care if "init mode", inner loop takes care of the rest of the game
 * game_status:
 * 0 during the game
 * 1 when the game is over (so the user will return to "init mode" = in the first loop)
 * 2 when the user enters "exit" command to exit program */
void start_game(){
    int game_status = 0, is_there_old_game=0;
    Game *my_game=(Game *) malloc(sizeof(Game));
    if (my_game==NULL){
        printf("Error: malloc has failed\n");
        exit(0);
    }
    printf("Sudoku\n------\n");
    while (1){
        if (game_status==2) {
            break;
        }
        game_status = init_user_turn(my_game,is_there_old_game);
        while (game_status==1){
            game_status = init_user_turn(my_game,is_there_old_game);
        }
        if (game_status==2) {
            break;
        }
        is_there_old_game=1;
        while(1){
            game_status = user_turn(my_game);
            if (game_status==1 || game_status == 2){
                break;
            }
        }
    }
    free(my_game);
}
