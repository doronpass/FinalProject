cmake_minimum_required(VERSION 3.12)
project(FInalProject C)

set(CMAKE_C_STANDARD 90)

add_executable(FInalProject main.c Game_board.c Game_board.h Dll.c Dll.h Error_handler.c Error_handler.h Play.c Play.h Functionality.c Functionality.h IlpSolver.c IlpSolver.h Parser.h Parser.c stack.c stack.h)
