cmake_minimum_required(VERSION 3.12)
project(FInalProject C)

set(CMAKE_C_STANDARD 90)


add_executable(FInalProject main.c Game.c Game.h Game_board.c Game_board.h Dll.c Dll.h Command.c Command.h Functionality.c Functionality.h)
