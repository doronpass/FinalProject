/*
  Sudoku example.

  The Sudoku board is a 9x9 grid, which is further divided into a 3x3 grid
  of 3x3 grids.  Each cell in the grid must take a value from 0 to 9.
  No two grid cells in the same row, column, or 3x3 subgrid may take the
  same value.

  In the MIP formulation, binary variables x[i,j,v] indicate whether
  cell <i,j> takes value 'v'.  The constraints are as follows:
    1. Each cell must take exactly one value (sum_v x[i,j,v] = 1)
    2. Each value is used exactly once per row (sum_i x[i,j,v] = 1)
    3. Each value is used exactly once per column (sum_j x[i,j,v] = 1)
    4. Each value is used exactly once per 3x3 subgrid (sum_grid x[i,j,v] = 1)

  Input datasets for this example can be found in examples/data/sudoku*.
*/
//int guobi_ilp(){
//#include <stdlib.h>
//#include <stdio.h>
//#include <string.h>
//#include "gurobi_c.h"
//{
//    FILE     *fp    = NULL;
//    GRBenv   *env   = NULL;
//    GRBmodel *model = NULL;
//    int       board[DIM][DIM];
//    char      inputline[100];
//    int       ind[DIM];
//    double    val[DIM];
//    double    lb[DIM*DIM*DIM];
//    char      vtype[DIM*DIM*DIM];
//    char     *names[DIM*DIM*DIM];
//    char      namestorage[10*DIM*DIM*DIM];
//    char     *cursor;
//    int       optimstatus;
//    double    objval;
//    int       zero = 0;
//    int       i, j, v, ig, jg, count;
//    int       error = 0;
//
//
//            error = GRBaddconstr(model, DIM, ind, val, GRB_EQUAL, 1.0, NULL);
//            if (error) goto QUIT;
//        }
//
//
//    /* Each value must appear once in each row */
//
//    for (v = 0; v < DIM; v++) {
//        for (j = 0; j < DIM; j++) {
//            for (i = 0; i < DIM; i++) {
//                ind[i] = i*DIM*DIM + j*DIM + v;
//                val[i] = 1.0;
//            }
//
//            error = GRBaddconstr(model, DIM, ind, val, GRB_EQUAL, 1.0, NULL);
//            if (error) goto QUIT;
//        }
//    }
//
//    /* Each value must appear once in each column */
//
//    for (v = 0; v < DIM; v++) {
//        for (i = 0; i < DIM; i++) {
//            for (j = 0; j < DIM; j++) {
//                ind[j] = i*DIM*DIM + j*DIM + v;
//                val[j] = 1.0;
//            }
//
//            error = GRBaddconstr(model, DIM, ind, val, GRB_EQUAL, 1.0, NULL);
//            if (error) goto QUIT;
//        }
//    }
//
//    /* Each value must appear once in each subgrid */
//
//    for (v = 0; v < DIM; v++) {
//        for (ig = 0; ig < SUBDIM; ig++) {
//            for (jg = 0; jg < SUBDIM; jg++) {
//                count = 0;
//                for (i = ig*SUBDIM; i < (ig+1)*SUBDIM; i++) {
//                    for (j = jg*SUBDIM; j < (jg+1)*SUBDIM; j++) {
//                        ind[count] = i*DIM*DIM + j*DIM + v;
//                        val[count] = 1.0;
//                        count++;
//                    }
//                }
//
//                error = GRBaddconstr(model, DIM, ind, val, GRB_EQUAL, 1.0, NULL);
//                if (error) goto QUIT;
//            }
//        }
//    }
//
//
//
//    /* Free model */
//
//    GRBfreemodel(model);
//
//    /* Free environment */
//
//    GRBfreeenv(env);
//
//    return 0;
//}