#ifndef FINALPROJECT_ERROR_HANDLER_H
#define FINALPROJECT_ERROR_HANDLER_H
#include "Game_board.h"

void check_memory(Cell* point);/*tests whether calloc failed
 * if so, alert the user and terminate program*/
void check_memory_int(int* point);/* same as the above function, only for an array of ints */
void check_memory2_int(int** point);/* same as the above function, only for ints */
void check_memory2(Cell** point);/*tests whether calloc failed
 * if so, alert the user and terminate program - not sure i should send a messege*/
void invalid_command();/* prints "ERROR: invalid command" when needed */
void not_in_range(int N);/* prints Error: value not in range when needed */
void cell_is_fixed();/* prints "Error: cell is fixed when needed */
void puzzle_solution_erroneus();/* prints Error: board contains erroneous values when needed */
void board_not_solvable();/* prints  Error: board is unsolvable when needed */
void validation_failed();/* prints Validation failed: board is unsolvable when needed */
void cell_contains_value();/* printsError: cell already contains a value when needed */
void validation_passed();/* prints Validation passed: board is solvable when needed */
void board_not_empty();/* prints Error: board is not empty when needed */
void puzzle_generator_failed();/* prints Error: puzzle generator failed when needed */
#endif
