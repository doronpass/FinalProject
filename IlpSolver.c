

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "gurobi_c.h"
#include "Game_board.h"
#include "Game.h"


/* free all the other stuff we used */     /* Free environment */     /* Free model */
void free_grb(int *ind ,int *indarr2,double *val , double *valarr2, double *sol , char *vtype, GRBenv *env,
              GRBmodel *model,double *lb ){

    free(sol);
    free(vtype);
    free(lb);
    free(val);
    free(ind);
    GRBfreemodel(model);
    GRBfreeenv(env);
    free(valarr2);
    free(indarr2);
}



void copy_sol_to_board(int *sol, Game *game) {
    int N = (game->m_mult_n)*(game->m_mult_n);
    int i, j, v;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            for (v = 0; v < N; v++) {
                if (sol[N * N * i + j * N + v] == 1.0) {
                    game->solved_game_board[i][j].value= v+1;
                }
            }
        }
    }
}



int ilp_solver(Game *game) {
    GRBenv *env = NULL;
    GRBmodel *model = NULL;
    char inputline[100];
    int *ind;
    int *indarr2;
    double *val;
    double *valarr2;
    double *lb;
    char *vtype;
    int optimstatus;
    double objval;
    int zero = 0;
    int i, j, v, ig, jg, count;
    int error = 0;
    int N;
    int flag = 0;
    double *sol;
    int temportry_value;
    /* initialize arrays and pointer for later use */

    sol = (double *) malloc(N * N * N * sizeof(double));
    N = game->m_mult_n * game->m_mult_n;
    vtype = (char *) malloc(N * N * N);
    lb = (double *) malloc(N * N * N* sizeof(double));
    val = (double *) malloc(N * sizeof(double));
    ind = (int *) malloc(N * sizeof(int));
    valarr2 = (double *) malloc(N * sizeof(double));
    indarr2 = (int *) malloc(N * sizeof(int));

    /* Create an empty model */

    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            for (v = 0; v < N; v++) {
                if (game->user_game_board[i][j].value == v+1) /* tto make sure that te plus 1 is needed */
                    lb[i * N * N + j * N + v] = 1;
                else
                    lb[i * N * N + j * N + v] = 0;
                vtype[i * N * N + j * N + v] = GRB_BINARY;


            }
        }}

    /* Create environment */

    error = GRBloadenv(&env, "sudoku.log");
    if (error) {
        printf("Error: %s\n",GRBgeterrormsg(env)));
        free_grb(ind ,indarr2,val ,valarr2, sol ,vtype,env,
                 model,lb);
        return flag;
    }

    /* Create new model */

    error = GRBnewmodel(env, &model, "sudoku", N * N * N, NULL, lb, NULL,
                        vtype, NULL);
    if (error) {
        printf("Error: %s\n",GRBgeterrormsg(env)));
        free_grb(ind ,indarr2,val ,valarr2, sol ,vtype,env,
                 model,lb);
        return flag;
    }

    /* Each cell gets a value */

    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            for (v = 0; v < N; v++) {
                ind[v] = i * N * N + j * N + v;
                val[v] = 1.0;
            }

            error = GRBaddconstr(model, N, ind, val, GRB_EQUAL, 1.0, NULL);
            if (error) {
                printf("Error: %s\n",GRBgeterrormsg(env)));
                free_grb(ind ,indarr2,val ,valarr2, sol ,vtype,env,
                         model,lb);
                return flag;
            }
        }
    }
/* this is the problematic one, if there are problem to check here */
    /* firest consraint that we might sould dump assure that the values that come in boars stay in place */
/* and sould check that in line 4 it realy user game board and not solved one */
    for (j = 0; j < N; j++) {
        for (i = 0; i < N; i++) {
            temportry_value = game->user_game_board[j][i].value;
            if (0 != temportry_value) {
                valarr2[0] = 1;
                indarr2[0] = N*N*j + i*N+(temportry_value - 1);
                error = GRBaddconstr(model, 1, indarr2, valarr2, GRB_EQUAL, 1.0, NULL);
                if (error) {
                    printf("Error: %s\n",GRBgeterrormsg(env)));
                    free_grb(ind ,indarr2,val ,valarr2, sol ,vtype,env,
                             model,lb);
                    return flag;
                }
            }
        }
    }

    /* Each value must appear once in each row */

    for (v = 0; v < N; v++) {
        for (j = 0; j < N; j++) {
            for (i = 0; i < N; i++) {
                ind[i] = i * N * N + j * N + v;
                val[i] = 1.0;
            }

            error = GRBaddconstr(model, N, ind, val, GRB_EQUAL, 1.0, NULL);
            if (error) {
                printf("Error: %s\n",GRBgeterrormsg(env)));
                free_grb(ind ,indarr2,val ,valarr2, sol ,vtype,env,
                         model,lb);
                return flag;
            }        }
    }

    /* Each value must appear once in each column */

    for (v = 0; v < N; v++) {
        for (i = 0; i < N; i++) {
            for (j = 0; j < N; j++) {
                ind[j] = i * N * N + j * N + v;
                val[j] = 1.0;
            }

            error = GRBaddconstr(model, N, ind, val, GRB_EQUAL, 1.0, NULL);
            if (error) {
                printf("Error: %s\n",GRBgeterrormsg(env)));
                free_grb(ind ,indarr2,val ,valarr2, sol ,vtype,env,
                         model,lb);
                return flag;
            }        }
    }

    /* Each value must appear once in each subgrid */

    for (v = 0; v < N; v++) {
        for (ig = 0; ig < game->m_block_rows; ig++) {
            for (jg = 0; jg < game->n_block_cols; jg++) {
                count = 0;
                for (i = ig * game->m_block_rows; i < (ig + 1) * game->m_block_rows; i++) {
                    for (j = jg * game->n_block_cols; j < (jg + 1) * game->n_block_cols; j++) {
                        ind[count] = i * N * N + j * N + v;
                        val[count] = 1.0;
                        count++;
                    }
                }

                error = GRBaddconstr(model, DIM, ind, val, GRB_EQUAL, 1.0, NULL);
                if (error) {
                    printf("Error: %s\n",GRBgeterrormsg(env)));
                    free_grb(ind ,indarr2,val ,valarr2, sol ,vtype,env,
                             model,lb);
                    return flag;
                }            }
        }
    }

    /* Optimize model */

    error = GRBoptimize(model);
    if (error) {
        printf("Error: %s\n",GRBgeterrormsg(env)));
        free_grb(ind ,indarr2,val ,valarr2, sol ,vtype,env,
                 model,lb);
        return flag;
    }
    /* Write model to 'sudoku.lp', int the tuturial they aid not necessary but very helpful */

    error = GRBwrite(model, "sudoku.lp");
    if (error) {
        printf("Error: %s\n",GRBgeterrormsg(env)));
        free_grb(ind ,indarr2,val ,valarr2, sol ,vtype,env,
                 model,lb);
        return flag;
    }
    /* Capture solution information */

    error = GRBgetintattr(model, GRB_INT_ATTR_STATUS,
                          &optimstatus); /*/query the status of the optimization process by retrieving the value of the Status attribute*/
    if (error) {
        printf("Error: %s\n",GRBgeterrormsg(env)));
        free_grb(ind ,indarr2,val ,valarr2, sol ,vtype,env,
                 model,lb);
        return flag;
    }
    /* this call would return a non-zero error result if no solution was found for this model.*/

    error = GRBgetdblattr(model, GRB_DBL_ATTR_OBJVAL,
                          &objval); /* the value of the objective function for the computed solution*/
    if (error) {
        printf("Error: %s\n",GRBgeterrormsg(env)));
        free_grb(ind ,indarr2,val ,valarr2, sol ,vtype,env,
                 model,lb);
        return flag;
    }
    /*this routine retrieves the values of an array-valued attribute. The third and fourth arguments indicate the index of the first array element to be retrieved, and the number of elements to retrieve*/
    error = GRBgetdblattrarray(model, GRB_DBL_ATTR_X, 0, N * N * N, sol); /* func i added from the example */
    if (error) {
        printf("Error: %s\n",GRBgeterrormsg(env)));
        free_grb(ind ,indarr2,val ,valarr2, sol ,vtype,env,
                 model,lb);
        return flag;
    }

    /* the sol is a good one so we thake it to out solver board in game and make it be the same */
    if (optimstatus == GRB_OPTIMAL) {
        copy_to_sol_board(sol, game);
        flag = 1 ;
        printf("Optimal objective: %.4e\n", objval);
    } else if (optimstatus == GRB_INF_OR_UNBD)
        printf("Model is infeasible or unbounded\n");
    else
        printf("Optimization was stopped early\n");
    printf("\n");


    if (error) {
        printf("Error: %s\n",GRBgeterrormsg(env)));
        free_grb(ind ,indarr2,val ,valarr2, sol ,vtype,env,
                model,lb);
        return flag;
    }

    return  flag;

    /* copy sol to game->user_game_board */


}
