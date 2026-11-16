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
int read_customers(struct banker *b, const char *filename, int numResources);
int validate_commands(const char *filename, int numCustomers,int numResources);
int parse_command(char *line, int numResources, int numCustomers,char *type, int *customer, int *values);
void strip_newline(char *s);
int is_blank(const char *s);
int parse_nonneg_int(const char *tok, int *out);

//algoritmo
void request_resources(struct banker *b, int customer, int *request, FILE *out);
void release_resources(struct banker *b, int customer, int *release, FILE *out);
void print_state(struct banker *b, FILE *out);

#endif
