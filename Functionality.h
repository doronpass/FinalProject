#ifndef FINALPROJECT_FUNCTIONALITY_H
#define FINALPROJECT_FUNCTIONALITY_H

void save_game(game *my_game, char *path);
int save_to_file(game *my_game, char *path);
int has_erroneous_values(game *my_game);
int load_from_file(game *my_game, char *path);

#endif //FINALPROJECT_FUNCTIONALITY_H
