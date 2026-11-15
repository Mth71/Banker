#include <stdlib.h>
#include "banker.h"

static int **alloc_matrix(int rows, int cols){
    int **m=calloc(rows,sizeof(int*));
    if(m == NULL){
        return NULL;
    }
    for(int i = 0; i<rows;i++){
        m[i]=calloc(cols, sizeof(int));
        if(m[i] == NULL){
            for(int k = 0; k<i ; k++){
                free(m[k]);
            }
            free(m);
            return NULL;
        }
    }
    return m;
}

static void free_matrix(int **m, int rows){
    if(m == NULL){
        return;
    }
    for(int i =0; i<rows; i++){
        free(m[i]);
    }
    free(m);
}

int banker_alloc(struct banker *b, int numCustomers, int numResources){
    b->numCustomers = numCustomers;
    b->numResources = numResources;
    b->available = NULL;
    b->maximum = NULL;
    b->need = NULL;
    b->allocation = NULL;
    b->available = calloc(numResources, sizeof(int));
    b->maximum = alloc_matrix(numCustomers, numResources);
    b->allocation = alloc_matrix(numCustomers, numResources);
    b->need = alloc_matrix(numCustomers, numResources);
    if(b->available == NULL || b->maximum == NULL || b->need == NULL || b->allocation == NULL){
        banker_free(b);
        return -1;
    }
    return 0;
}

void banker_free(struct banker *b){
    free_matrix(b->maximum, b->numCustomers);
    free_matrix(b->allocation, b->numCustomers);
    free_matrix(b->need, b->numCustomers);
    free(b->available);
    b->available = NULL;
    b->maximum = NULL;
    b->need = NULL;
    b->allocation = NULL;
}
