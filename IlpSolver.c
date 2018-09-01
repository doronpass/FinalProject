

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "gurobi_c.h"
#include "Game_board.h"
#include "Game.h"


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
    double *val;
    double *lb;
    char *vtype;
    char *names;
    char *namestorage;
    char *cursor;
    int optimstatus;
    double objval;
    int zero = 0;
    int i, j, v, ig, jg, count;
    int error = 0;
    int N;
    int *sol;
    int temportry_value;
    /* initialize arrays and pointer for later use */

    sol = (int *) malloc(N * N * N);
    Cell **user_board = game->user_game_board;
    N = game->m_mult_n * game->m_mult_n;
    namestorage = (char *) malloc(10 * N * N * N);
    vtype = (char *) malloc(N * N * N);
    lb = (double *) malloc(N * N * N);
    val = (double *) malloc(N);
    ind = (int *) malloc(N);
    names = (char *) malloc(N * N * N);

    /* Create an empty model */

    cursor = namestorage;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            for (v = 0; v < N; v++) {
                if (user_board[i][j].value == v) /* to make sure it includes our value in the existiong board */
                    lb[i * N * N + j * N + v] = 1;
                else
                    lb[i * N * N + j * N + v] = 0;
                vtype[i * N * N + j * N + v] = GRB_BINARY;

                names[i * N * N + j * N + v] = cursor;
                sprintf(names[i * N * N + j * N + v], "x[%d,%d,%d]", i, j, v + 1);
                cursor += strlen(names[i * N * N + j * N + v]) + 1;
            }
        }
    }

    /* Create environment */

    error = GRBloadenv(&env, "sudoku.log");
    if (error) goto QUIT;

    /* Create new model */

    error = GRBnewmodel(env, &model, "sudoku", N * N * N, NULL, lb, NULL,
                        vtype, names);
    if (error) goto QUIT;

    /* Each cell gets a value */

    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            for (v = 0; v < N; v++) {
                ind[v] = i * N * N + j * N + v;
                val[v] = 1.0;
            }

            error = GRBaddconstr(model, N, ind, val, GRB_EQUAL, 1.0, NULL);
            if (error) goto QUIT;
        }
    }



    /* firest consraint that we might sould dump assure that the values that come in boars stay in place */
/* and sould check that in line 4 it realy user game board and not solved one */
    for (j = 0; j < N; j++) {
        for (i = 0; i < N; i++) {
            temportry_value = game->user_game_board[j][i].value;
            if (0 != temportry_value) {
                for (v = 0; v < N; ++v) {
                    if (temportry_value == v + 1) {
                        ind[v] = i * N * N + i * N + v;
                        val[v] = 1.0;

                    }
                }
                error = GRBaddconstr(model, size, ind, val, GRB_EQUAL, 1.0, NULL);
                if (error) goto QUIT;

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
            if (error) goto QUIT;
        }
    }

    /* Each value must appear once in each column */

    for (v = 0; v < N; v++) {
        for (i = 0; i < N; i++) {
            for (j = 0; j < N; j++) {
                ind[j] = i * N * N + j * N + v;
                val[j] = 1.0;
            }

            error = GRBaddconstr(model, N, ind, val, GRB_EQUAL, 1.0, NULL);
            if (error) goto QUIT;
        }
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
                if (error) goto QUIT;
            }
        }
    }

    /* Optimize model */

    error = GRBoptimize(model);
    if (error) goto QUIT;

    /* Write model to 'sudoku.lp', int the tuturial they aid not necessary but very helpful */

    error = GRBwrite(model, "sudoku.lp");
    if (error) goto QUIT;

    /* Capture solution information */

    error = GRBgetintattr(model, GRB_INT_ATTR_STATUS,
                          &optimstatus); /*/query the status of the optimization process by retrieving the value of the Status attribute*/
    if (error) goto QUIT;

    /* this call would return a non-zero error result if no solution was found for this model.*/

    error = GRBgetdblattr(model, GRB_DBL_ATTR_OBJVAL,
                          &objval); /* the value of the objective function for the computed solution*/
    if (error) goto QUIT;

    /*this routine retrieves the values of an array-valued attribute. The third and fourth arguments indicate the index of the first array element to be retrieved, and the number of elements to retrieve*/
    error = GRBgetdblattrarray(model, GRB_DBL_ATTR_X, 0, N * N * N, sol); /* func i added from the example */
    if (error) {
        printf("ERROR %d GRBgetdblattrarray(): %s\n", error, GRBgeterrormsg(env));
        return -1;
    }

    printf("\nOptimization complete\n");


    /* the sol is a good one so we thake it to out solver board in game and make it be the same */
    if (optimstatus == GRB_OPTIMAL) {
        copy_to_sol_board(sol, game);
        printf("Optimal objective: %.4e\n", objval);
    } else if (optimstatus == GRB_INF_OR_UNBD)
        printf("Model is infeasible or unbounded\n");
    else
        printf("Optimization was stopped early\n");
    printf("\n");

    QUIT:

    /* Error reporting */

    if (error) {
        printf("ERROR: %s\n", GRBgeterrormsg(env));
        exit(1);
    }

    /* copy sol to user_board */






    void free_ilp() {/* free all the other stuff we used */     /* Free environment */     /* Free model */


        free(sol);
        free(user_board);
        free(namestorage);
        free(vtype);
        free(lb);
        free(val);
        free(ind);
        GRBfreemodel(model);
        GRBfreeenv(env);
    }
}
