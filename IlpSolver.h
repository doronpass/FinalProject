#ifndef FINALPROJECT_ILPSOLVER_H
#define FINALPROJECT_ILPSOLVER_H
#include "Game.h"
#include "gurobi_c.h"


int ilp_solver(Game *game) ;
void copy_sol_to_board(double *sol, Game *game) ;
void free_grb(int *ind ,int *indarr2,double *val , double *valarr2, double *sol , char *vtype, GRBenv *env,
              GRBmodel *model,double *lb );
#endif