#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "gurobi_c.h"
#include "Game_board.h"
#include "IlpSolver.h"

/* this functions use gurobi to solve the sudoku puzzle with linear programming, returns 1 if successful, else returns 0*/
int ilp_solver(Game *game) {
    GRBenv   *env = NULL;
    GRBmodel *model= NULL;
    int      *ind, *indarr2, optimstatus, i, j, v, ig, jg, count, error = 0, N, temportry_value, flag = 0;
    double   *val, *valarr2, *lb, *sol;
    char     *vtype;
    double   objval;
    int      **result_arr;
    N       = game->m_mult_n;
    /* allocate memory to relevent pointers */
    indarr2 = (int *) malloc (N * sizeof (int));
    valarr2 = (double *) malloc (N * sizeof (double));
    ind     = (int *) malloc (N * sizeof (int));
    val     = (double *) malloc (N * sizeof (double));
    lb      = (double *) malloc (N * N * N * sizeof (double));
    vtype   = (char *) calloc (sizeof (char), N * N * N);
    sol     = (double *) calloc (sizeof (double), N * N * N);


    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            for (v = 0; v < N; v++) {
                lb[N * N * i + N * j + v]  = 0.0;
                sol[N * N * i + N * j + v] = 0.0;
            }
        }
    }


    /* initialize arrays and pointer for later use */
    result_arr = create_matrix (N);
    memo_and_check (result_arr, sol, vtype, lb, val, ind, valarr2, indarr2);
    GRBsetintparam(env,GRB_INT_PAR_LOGTOCONSOLE,0);
    /* Create an empty model before we starts*/
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            for (v = 0; v < N; v++) {
                if (game->user_game_board[i][j].value == v + 1)
                {
                    lb[i * N * N + j * N + v] = 1;
                } else {

                    lb[i * N * N + j * N + v] = 0;
                }
                vtype[i * N * N + j * N + v] = GRB_BINARY;


            }
        }
    }

    /* Create environment before we starts */
    error = GRBloadenv (&env, "sudoku.log");
    if (error) {
        free_grb (ind, indarr2, val, valarr2, sol, vtype, env, model, lb, result_arr, N);
        return flag;
    }
    GRBsetintparam(env,GRB_INT_PAR_LOGTOCONSOLE,0);
    /* Create new model */
    error = GRBnewmodel (env, &model, "sudoku", N * N * N, NULL, lb, NULL, vtype, NULL);
    if (error) {
        free_grb (ind, indarr2, val, valarr2, sol, vtype, env, model, lb, result_arr, N);
        return flag;
    }
    /* Each cell gets a value */
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            for (v = 0; v < N; v++) {
                ind[v] = N * i * N + N * j + v;
                val[v] = 1.0;
            }
            error = GRBaddconstr (model, N, ind, val, GRB_EQUAL, 1.0, NULL);
            if (error) {
                free_grb (ind, indarr2, val, valarr2, sol, vtype, env, model, lb, result_arr, N);
                return flag;
            }
        }
    }

/* this constraint checks and make sure that every value that was in board stay in his place*/
    for (j = 0; j < N; j++) {
        for (i = 0; i < N; i++) {
            temportry_value = game->user_game_board[j][i].value;
            if (0 != temportry_value) {
                indarr2[0] = N * j * N + i * N + (temportry_value - 1);
                valarr2[0] = 1;

                error = GRBaddconstr (model, 1, indarr2, valarr2, GRB_EQUAL, 1.0, NULL);
                if (error) {
                    free_grb (ind, indarr2, val, valarr2, sol, vtype, env, model, lb, result_arr, N);
                    return flag;
                }
            }
        }
    }
    /* This constraints make sure that Each value must appear once in each row */
    for (v = 0; v < N; v++) {
        for (j = 0; j < N; j++) {
            for (i = 0; i < N; i++) {
                ind[i] = N * i * N + j * N + v;
                val[i] = 1.0;
            }
            error = GRBaddconstr (model, N, ind, val, GRB_EQUAL, 1.0, NULL);
            if (error) {
                free_grb (ind, indarr2, val, valarr2, sol, vtype, env, model, lb, result_arr, N);
                return flag;
            }
        }
    }
    /* This constraints make sure Each value must appear once in each column */
    for (v = 0; v < N; v++) {
        for (i = 0; i < N; i++) {
            for (j = 0; j < N; j++) {
                ind[j] = N * i * N + j * N + v;
                val[j] = 1.0;
            }
            error = GRBaddconstr (model, N, ind, val, GRB_EQUAL, 1.0, NULL);
            if (error) {
                free_grb (ind, indarr2, val, valarr2, sol, vtype, env, model, lb, result_arr, N);
                return flag;
            }
        }
    }
    /* Each value must appear once in each subgrid */ /* here i changed #@%#@%#@%@#%@#%@#% */
    for (v = 0; v < N; v++) {
        for (ig = 0; ig < game->n_block_cols; ig++) {
            for (jg = 0; jg < game->m_block_rows; jg++) {
                count  = 0;
                for (i = ig * game->m_block_rows; i < (ig + 1) * game->m_block_rows; i++) {
                    for (j = jg * game->n_block_cols; j < (jg + 1) * game->n_block_cols; j++) {
                        ind[count] = i * N * N + N * j + v;
                        val[count] = 1.0;
                        count++;
                    }
                }
                error = GRBaddconstr (model, N, ind, val, GRB_EQUAL, 1.0, NULL);
                if (error) {
                    free_grb (ind, indarr2, val, valarr2, sol, vtype, env, model, lb, result_arr, N);
                    return flag;
                }
            }
        }
    }
    /* Optimizing model */
    error  = GRBoptimize (model);
    if (error) {
        free_grb (ind, indarr2, val, valarr2, sol, vtype, env, model, lb, result_arr, N);
        return flag;
    }
    /* Write model to 'sudoku.lp' */
    error = GRBwrite (model, "sudoku.lp");
    if (error) {
        free_grb (ind, indarr2, val, valarr2, sol, vtype, env, model, lb, result_arr, N);
        return flag;
    }
    /* Capture solution information */
    error = GRBgetintattr (model, GRB_INT_ATTR_STATUS,
                           &optimstatus); /*query the status of the optimization process by retrieving the value of the Status attribute*/
    if (error) {
           free_grb (ind, indarr2, val, valarr2, sol, vtype, env, model, lb, result_arr, N);
           return flag;
       }
       /* this call would return a non-zero error result if no solution was found for this model.*/

    error = GRBgetdblattr (model, GRB_DBL_ATTR_OBJVAL,
                           &objval); /* the value of the objective function for the computed solution*/
    if (error) {
        free_grb (ind, indarr2, val, valarr2, sol, vtype, env, model, lb, result_arr, N);
        return flag;
    }
    /*this routine retrieves the values of an array-valued attribute. The third and fourth arguments indicate the index of the first array element to be retrieved, and the number of elements to retrieve*/
    error = GRBgetdblattrarray (model, GRB_DBL_ATTR_X, 0, N * N * N, sol);
    if (error) {
        free_grb (ind, indarr2, val, valarr2, sol, vtype, env, model, lb, result_arr, N);
        return flag;
    }
    /* the solution is a good one so we take it to out solver board in game and make it be the same */
    if (optimstatus == GRB_OPTIMAL) {
        copy_sol_to_board (sol, result_arr, N);
        copy_board_to_game (result_arr, N, game);
            free_grb (ind, indarr2, val, valarr2, sol, vtype, env, model, lb, result_arr, N);
            flag = 1;

        }
        return flag;
}

/* free all the other stuff we used */     /* Free environment */     /* Free model */
void free_grb (int *ind, int *indarr2, double *val, double *valarr2, double *sol, char *vtype, GRBenv *env,
               GRBmodel *model, double *lb, int **result_arr, int N) {
    int i;
    free (sol);
    free (vtype);
    free (lb);
    free (val);
    free (ind);
    free (valarr2);
    free (indarr2);
    if (result_arr != NULL) {
        for (i = 0; i < N; i++) {
            free (result_arr[i]);
        }
        free (result_arr);
    }
    GRBfreemodel (model);
    GRBfreeenv (env);
}

/*this functions  checks that all memory allocation don properly*/
void memo_and_check ( int **result_arr, double *sol, char *vtype, double *lb, double *val, int *ind,
                      double *valarr2, int *indarr2) {
    if ((result_arr) == NULL) {
        printf ("Error: malloc has failed\n");
        exit (1);
    }
    if (sol == NULL) {
        printf ("Error: calloc has failed\n");
        exit (1);
    }
    if (vtype == NULL) {
        printf ("Error: calloc has failed\n");
        exit (1);
    }
    if (lb == NULL) {
        printf ("Error: malloc has failed\n");
        exit (1);
    }
    if (val == NULL) {
        printf ("Error: malloc has failed\n");
        exit (1);
    }
    if (ind == NULL) {
        printf ("Error: malloc has failed\n");
        exit (1);
    }
    if (valarr2 == NULL) {
        printf ("Error: malloc has failed\n");
        exit (1);
    }
    if (indarr2 == NULL) {
        printf ("Error: malloc has failed\n");
        exit (1);
    }
}
