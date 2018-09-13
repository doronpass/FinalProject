/* the function used when the user enters the "save" command*/
void save_game(Game *my_game, char *path){
    /*if game mode is solve, simply save to file*/
    if (my_game->mode == 1) {
        if (save_to_file(my_game, path)==0){
            printf("Error: File cannot be created or modified\n");
        } else{
            printf("Saved to: %s\n", path);
        }
        return;
    }

    /*make sure board is not erroneous before save in edit mode*/
    if (has_erroneous_values(my_game)) {
        puzzle_solution_erroneus();
        return;
    }

    /*make sure the board is valid before save in edit mode ------ NEED TO BUILD FUNCTION - need gurubi*/
/*
     if (!validate(my_game)){
        printf("Error: board validation failed\n");
        return;
    }
*/
    if (save_to_file(my_game, path)==0){
        printf("Error: File cannot be created or modified\n");
    } else{
        printf("Saved to: %s\n", path);
    }
}