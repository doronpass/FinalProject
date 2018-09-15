/* this module contains error messages printed to the user to inform him of a problem or success of
 * some functions during his game */
#ifndef FINALPROJECT_ERROR_HANDLER_H
#define FINALPROJECT_ERROR_HANDLER_H
#include "Game_board.h"

void check_memory(Cell* point);
void check_memory_int(int* point);
void check_memory2_int(int** point);
void check_memory2(Cell** point);
void invalid_command();
void not_in_range(int N);
void cell_is_fixed();
void puzzle_solution_erroneus();
void board_not_solvable();
void validation_failed();
void cell_contains_value();
void validation_passed();
void board_not_empty();
void puzzle_generator_failed();

#endif
