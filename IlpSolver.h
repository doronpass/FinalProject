#ifndef FINALPROJECT_ILPSOLVER_H
#define FINALPROJECT_ILPSOLVER_H
#include "Game_board.h"
#include "gurobi_c.h"


int ilp_solver(Game *game) ;
void free_grb(int *ind, int *indarr2, double *val, double *valarr2, double *sol, char *vtype, GRBenv *env,
              GRBmodel *model, double *lb, int **result_arr, int N) ;
void memo_and_check(int **result_arr,double  *sol,char *vtype,double  *lb,double  *val,int *ind,double  *valarr2,int *indarr2);

#endif