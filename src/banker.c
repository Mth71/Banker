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

static int is_safe(struct banker *b){
    int n = b->numCustomers;
    int m =  b->numResources;
    int *work = malloc(m*sizeof(int));
    int *finish = calloc(n, sizeof(int));
    if(work == NULL || finish == NULL){
        free(work);
        free(finish);
        return 0;
    }
    for(int j = 0; j < m; j++){
    work[j] = b->available[j];
    }

    int achou = 1;
    while(achou){
        achou = 0;
        for(int i =0; i < n; i++){
            if(finish[i] == 1){
                continue;
            }
            int cabe = 1;
            for(int j = 0; j<m;j++){
                if(b->need[i][j] > work[j]){
                    cabe = 0;
                    break;
                }
            }
            if(cabe){
                for(int j = 0; j<m ; j++){
                    work[j] += b->allocation[i][j];
                }
                finish[i]=1;
                achou = 1;
            }
        }
    }
    int seguro = 1;
    for(int i = 0; i < n; i++){
        if(finish[i] == 0){
            seguro = 0;
            break;
        }
    }
    free(work);
    free(finish);
    return seguro;
}

//garante a saida igual ao txt enviado, espacos antes de cada numero
static void escreve_antes(FILE *out, int *v, int m){
    for(int j = 0; j<m; j++){
        fprintf(out, " %d", v[j]);
    }
}

//escreve a saida tbm so que com os espacos depois
static void escreve_depois(FILE *out, int *v, int m){
    for(int j = 0; j<m; j++){
        fprintf(out, "%d ", v[j]);
    }
}

void request_resources(struct banker *b, int customer, int *request, FILE *out){
    int m = b->numResources;
    int excede = 0;
    for(int j = 0 ; j < m ; j++){
        if(request[j] > b->need[customer][j]){
            excede = 1;
            break;
        }
    }

    if(excede){
        fprintf(out,"The customer %d request", customer);
        escreve_antes(out, request, m);
        fprintf(out, " was denied because exceed its maximum allocation\n"); 
        return; 
    }
    int falta = 0;
    for(int j =0; j<m;j++){
        if(request[j] >b->available[j]){
            falta = 1;
            break;
        }
    }
    if(falta){
        fprintf(out, "The resources ");
        escreve_depois(out, b->available, m);
        fprintf(out, "are not enough to customer %d request ", customer);
        escreve_depois(out, request, m);
        fprintf(out, "\n");
        return;
    }

    for(int j = 0; j < m; j++){
        b->available[j] -= request[j];
        b->allocation[customer][j] += request[j];
        b->need[customer][j] -= request[j];
    }

    if(is_safe(b)){
        fprintf(out, "Allocate to customer %d the resources", customer);
        escreve_antes(out, request, m);
        fprintf(out, "\n");
    }else{
        for(int j = 0; j < m; j++){
            b->available[j] += request[j];
            b->allocation[customer][j] -= request[j];
            b->need[customer][j] += request[j];
        }
        fprintf(out,"The customer %d request", customer);
        escreve_antes(out, request, m);
        fprintf(out," was denied because result in an unsafe state\n");       
    }
}

void release_resources(struct banker *b, int customer, int *release, FILE *out){
    int m = b->numResources;
    int excede = 0;
    for(int j = 0 ; j< m; j++){
        if(release[j] > b->allocation[customer][j]){
            excede = 1;
            break;
        }
    }
    if(excede){
        fprintf(out, "The customer %d release", customer);
        escreve_antes(out, release, m);
        fprintf(out, " was denied because exceed its allocation\n");
        return;
    }
    for(int j = 0; j < m; j++){
        b->available[j] += release[j];
        b->allocation[customer][j] -= release[j];
        b->need[customer][j] += release[j];
    }

    fprintf(out, "Release from customer %d the resources", customer);
    escreve_antes(out, release, m);
    fprintf(out,"\n");
}

void print_state(struct banker *b, FILE *out){
    int m = b->numResources;
    int espacos_Max = 10 - m *2;
    int espacos_Alloc = 12 - m *2;

    fprintf(out,"MAXIMUM   |   ALLOCATION  |   NEED\n");

    for(int i =0; i< b->numCustomers ; i++){
        escreve_depois(out, b->maximum[i], m);
        for(int k = 0 ; k  < espacos_Max ; k++){
            fprintf(out, " ");
        }
        fprintf(out, "|   ");

        escreve_depois(out, b->allocation[i], m);
        for(int k = 0; k< espacos_Alloc;k++){
            fprintf(out, " ");
        }
        fprintf(out,"|   ");

        escreve_depois(out,b->need[i], m);
        fprintf(out, "\n");     
    }

    fprintf(out, "AVAILABLE ");
    escreve_depois(out, b->available, m);
    fprintf(out, "\n");   
}