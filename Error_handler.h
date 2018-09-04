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

void validation_passed();
void nothing_to_undo();
void nothing_to_redo();

#endif //FINALPROJECT_ERROR_HANDLER_H
