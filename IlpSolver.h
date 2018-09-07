//
// Created by Doron_Passal on 29/08/2018.
//

#ifndef FINALPROJECT_ILPSOLVER_H
#define FINALPROJECT_ILPSOLVER_H

#include "Game.h"

void free_ilp();
int ilp_solver(Game *game) ;
void copy_sol_to_board(int *sol, Game *game) ;
void free_grb(int *ind ,int *indarr2,double *val , double *valarr2, double *sol , char *vtype, GRBenv *env,
              GRBmodel *model,double *lb );

#endif //FINALPROJECT_ILPSOLVER_H
