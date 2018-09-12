

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "gurobi_c.h"
#include "Game_board.h"
#include "Game.h"
#include "IlpSolver.h"


int ilp_solver(Game *game) {
    GRBenv *env = NULL;
    GRBmodel *model = NULL;
    int *ind;
    int *indarr2;
    double *val;
    double *valarr2;
    double *lb;
    char *vtype;
    int optimstatus;
    double objval;
    int i, j, v, ig, jg, count;
    int error = 0;
    int N;
    int flag = 0;
    double *sol;
    int temportry_value;
    /* initialize arrays and pointer for later use */
    printf("here hello\n");
    N = game->m_mult_n ;/* it was before  this in squeare -------------------------------------- */
    sol = (double *) malloc(N * N * N * sizeof(double));
    if( sol == NULL){
        printf("malloc has a problem\n");
        exit(1);
    }
    N = game->m_mult_n ;
    vtype = (char *) malloc(N * N * N * sizeof(char));
    if( vtype == NULL){
        printf("malloc has a problem\n");
        exit(1);
    }
    lb = (double *) malloc(N * N * N* sizeof(double));
    if( lb == NULL){
        printf("malloc has a problem\n");
        exit(1);
    }
    val = (double *) malloc(N * sizeof(double));
    if( val == NULL){
        printf("malloc has a problem\n");
        exit(1);
    }
    ind = (int *) malloc(N * sizeof(int));
    if( ind == NULL){
        printf("malloc has a problem");
        exit(1);
    }
    valarr2 = (double *) malloc(N * sizeof(double));
    if( valarr2 == NULL){
        printf("malloc has a problem\n");
        exit(1);
    }
    indarr2 = (int *) malloc(N * sizeof(int));
    if( indarr2 == NULL){
        printf("malloc has a problem\n");
        exit(1);
    }

    /* Create an empty model */
    printf("72\n");
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            for (v = 0; v < N; v++) {
                printf("N=%d\n",N);
                printf("i=%d,j=%d,v=%d\n",i,j,v);
                if (game->user_game_board[i][j].value == v+1) /* to make sure that te plus 1 is needed */
                { printf("75\n");
                    lb[i * N * N + j * N + v] = 1;}
                else{
                    printf("78\n");
                    lb[i * N * N + j * N + v] = 0;
                    printf("im at 80");}
                vtype[i * N * N + j * N + v] = GRB_BINARY;


            }
        }}

    /* Create environment */
    printf("87\n");
    printf("87\n");
    printf("this is it \n");
    error = GRBloadenv(&env, "sudoku.log");
    if (error) {
        printf("Error: %s\n",GRBgeterrormsg(env));
        free_grb(ind ,indarr2,val ,valarr2, sol ,vtype,env,
                 model,lb);
        return flag;
    }

    /* Create new model */
    printf("97\n");

    error = GRBnewmodel(env, &model, "sudoku", N * N * N, NULL, lb, NULL,
                        vtype, NULL);
    if (error) {
        printf("Error: %s\n",GRBgeterrormsg(env));
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
                printf("Error: %s\n" , GRBgeterrormsg(env));
                free_grb(ind ,indarr2,val ,valarr2, sol ,vtype,env,
                         model,lb);
                return flag;
            }
        }
    }
/* this is the problematic one, if there are problem to check here */
    /* firest consraint that we might sould dump assure that the values that come in boars stay in place */
/* and sould check that in line 4 it realy user game board and not solved one */
    printf("135\n");
    for (j = 0; j < N; j++) {
        for (i = 0; i < N; i++) {
            temportry_value = game->user_game_board[j][i].value;
            if (0 != temportry_value) {
                valarr2[0] = 1;
                indarr2[0] = N*N*j + i*N+(temportry_value - 1);
                error = GRBaddconstr(model, 1, indarr2, valarr2, GRB_EQUAL, 1.0, NULL);
                if (error) {
                    printf("Error: %s\n",GRBgeterrormsg(env));
                    free_grb(ind ,indarr2,val ,valarr2, sol ,vtype,env,
                             model,lb);
                    return flag;
                }
            }
        }
    }

    /* Each value must appear once in each row */
    printf("155\n");
    for (v = 0; v < N; v++) {
        for (j = 0; j < N; j++) {
            for (i = 0; i < N; i++) {
                ind[i] = i * N * N + j * N + v;
                val[i] = 1.0;
            }

            error = GRBaddconstr(model, N, ind, val, GRB_EQUAL, 1.0, NULL);
            if (error) {
                printf("Error: %s\n",GRBgeterrormsg(env));
                free_grb(ind ,indarr2,val ,valarr2, sol ,vtype,env,
                         model,lb);
                return flag;
            }        }
    }
    printf("169\n");


    /* Each value must appear once in each column */

    for (v = 0; v < N; v++) {
        for (i = 0; i < N; i++) {
            for (j = 0; j < N; j++) {
                ind[j] = i * N * N + j * N + v;
                val[j] = 1.0;
            }

            error = GRBaddconstr(model, N, ind, val, GRB_EQUAL, 1.0, NULL);
            if (error) {
                printf("Error: %s\n",GRBgeterrormsg(env));
                free_grb(ind ,indarr2,val ,valarr2, sol ,vtype,env,
                         model,lb);
                return flag;
            }        }
    }
    printf("189\n");
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

                error = GRBaddconstr(model, N, ind, val, GRB_EQUAL, 1.0, NULL);
                if (error) {
                    printf("Error: %s\n",GRBgeterrormsg(env));
                    free_grb(ind ,indarr2,val ,valarr2, sol ,vtype,env,
                             model,lb);
                    return flag;
                }            }
        }
    }
    printf("213\n");
    /* Optimize model */

    error = GRBoptimize(model);
    if (error) {
        printf("Error: %s\n",GRBgeterrormsg(env));
        free_grb(ind ,indarr2,val ,valarr2, sol ,vtype,env,
                 model,lb);
        return flag;
    }
    printf("223\n");
    /* Write model to 'sudoku.lp', int the tuturial they aid not necessary but very helpful */

    error = GRBwrite(model, "sudoku.lp");
    if (error) {
        printf("Error: %s\n",GRBgeterrormsg(env));
        free_grb(ind ,indarr2,val ,valarr2, sol ,vtype,env,
                 model,lb);
        return flag;
    }
    /* Capture solution information */
    printf("234\n");
    error = GRBgetintattr(model, GRB_INT_ATTR_STATUS,
                          &optimstatus); /*/query the status of the optimization process by retrieving the value of the Status attribute*/
    if (error) {
        printf("Error: %s\n",GRBgeterrormsg(env));
        free_grb(ind ,indarr2,val ,valarr2, sol ,vtype,env,
                 model,lb);
        return flag;
    }
    printf("243\n");
    /* this call would return a non-zero error result if no solution was found for this model.*/

    error = GRBgetdblattr(model, GRB_DBL_ATTR_OBJVAL,
                          &objval); /* the value of the objective function for the computed solution*/
    if (error) {
        printf("Error: %s\n",GRBgeterrormsg(env));
        free_grb(ind ,indarr2,val ,valarr2, sol ,vtype,env,
                 model,lb);
        return flag;
    }
    printf("253\n");

    /*this routine retrieves the values of an array-valued attribute. The third and fourth arguments indicate the index of the first array element to be retrieved, and the number of elements to retrieve*/
    error = GRBgetdblattrarray(model, GRB_DBL_ATTR_X, 0, N * N * N, sol); /* func i added from the example */
    if (error) {
        printf("Error: %s\n",GRBgeterrormsg(env));
        free_grb(ind ,indarr2,val ,valarr2, sol ,vtype,env,
                 model,lb);
        return flag;
    }

    printf("265\n");


    /* the sol is a good one so we thake it to out solver board in game and make it be the same */
    if (optimstatus == GRB_OPTIMAL) {
        printf(" 260\n");
        copy_sol_to_board(sol,game); /*mbaye without the &, to check it out */

        flag = 1 ;}

    printf("275\n");

/*        printf("Optimal objective: %.4e\n", objval);
    } else if (optimstatus == GRB_INF_OR_UNBD)
        printf("Model is infeasible or unbounded\n");
    else
        printf("Optimization was stopped early\n");
    printf("\n"); */


    if (error) {
        printf("Error: %s\n",GRBgeterrormsg(env));
        free_grb(ind ,indarr2,val ,valarr2, sol ,vtype,env,
                model,lb);
        return flag;
    }

    return  flag;

    printf("294\n");


}
void copy_sol_to_board(double *sol, Game *game) {
    int N = (game->m_mult_n);
    int i, j, v;
    printf("301\n");

    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            for (v = 0; v < N; v++) {
                printf("i=%d,j=%d,v=%d\n",i,j,v);
                if (sol[i * N * N + j * N + v] == 1) {
                    printf("307");
                    game->solved_game_board[i][j].value= v+1;
                    printf("309");
                }
                else{ printf("not enter to if");}
            }
        }
    }
    printf("312\n");

}

/* free all the other stuff we used */     /* Free environment */     /* Free model */
void free_grb(int *ind ,int *indarr2,double *val , double *valarr2, double *sol , char *vtype, GRBenv *env,
              GRBmodel *model,double *lb ){
    printf("im inside free grb ");
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