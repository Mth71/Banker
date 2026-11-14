#ifndef BANKER_H
#define BANKER_H

#include <stdio.h>

struct banker{
    int numCustomers;
    int numResources;
    int *available;
    int **maximum;
    int **allocation;
    int **need;
};

//alocação e libera;ao
int banker_alloc(struct banker *b, int numCustomers, int numResources);
void banker_free(struct banker *b);

//leitura
int read_costumers(struct banker *b, const char *filename, int numResources);
int validate_commands(const char *filename, int numCustomers,int numResources);

//algoritmo
void request_resources(struct banker *b, int costomer, int *request, FILE *out);
void release_resources(struct banker *b, int costomer, int *release, FILE *out);
void print_state(struct banker *b, FILE *out);

#endif
