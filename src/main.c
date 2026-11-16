#include "banker.h"
#include <stdio.h>
#include <stdlib.h>
#define LINE_MAX_LEN 1024

int main(int argc, char *argv[]){
    if(argc < 2){
        fprintf(stderr, "Uso: %s <instâncias do recurso 0> <recurso 1>\n", argv[0]);
        return 1;
    }  
    int numResources = argc - 1;
    int *total = malloc(numResources*sizeof(int));
    if(total == NULL){
        fprintf(stderr,"Erro: Memoria insuficiente\n");
        return 1;
    }
    for(int i = 1; i < argc; i++){
        if(parse_nonneg_int(argv[i], &total[i-1]) != 0){
            fprintf(stderr, "Erro: Argumento Inválido (%s)\n", argv[i]);
            free(total);
            return 1;                              
        }
    }

    struct banker b;
    if(read_customers(&b, "customer.txt", numResources) != 0){ 
        free(total);
        return 1;
     }
    for(int j = 0; j < numResources; j++){
        b.available[j]=total[j];
    }
    free(total);

    if(validate_commands("commands.txt", b.numCustomers, b.numResources) != 0){
        banker_free(&b);
        return 1;
    }

    FILE *cmd = fopen("commands.txt", "r");
    FILE *out = fopen("result.txt", "w");
    int *values = malloc(numResources*sizeof(int));
    if(cmd == NULL || out == NULL || values == NULL){
        fprintf(stderr, "Erro: Não foi possivel abrir commands.txt, criar result.txt ou alocar memória\n");
        if(cmd != NULL){
            fclose(cmd);   
        }
        if(out != NULL){
            fclose(out);
        }
        free(values);
        banker_free(&b);
        return 1;
    }

    char line[LINE_MAX_LEN];
    char type;
    int customer;
    while(fgets(line, sizeof line, cmd) != NULL){
        strip_newline(line);
        if(is_blank(line)) continue;
        parse_command(line, numResources, b.numCustomers, &type, &customer, values);
        if(type == '*'){
            print_state(&b, out);
        }      
        else if(type == 'Q'){
            request_resources(&b, customer, values, out);
        }
        else if(type == 'L') 
        release_resources(&b, customer, values, out);
    }
    free(values); fclose(cmd); fclose(out); banker_free(&b);
    return 0;
}