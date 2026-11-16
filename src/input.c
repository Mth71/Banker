#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>    
#include <limits.h>   
#include "banker.h"

#define LINE_MAX_LEN 1024


static void strip_newline(char *s){
    size_t tamanho = strlen(s);
    while(tamanho > 0 && (s[tamanho - 1] == '\n' || s[tamanho - 1] == '\r')){
        s[tamanho - 1] = '\0';
        tamanho--;
    }
}
static int is_blank(const char *s){
    for(int i = 0 ; s[i] != '\0'; i++){
        if(!(isspace((unsigned char)s[i]))){
            return 0;
        }
    }
    return 1;
}

static int parse_nonneg_int(const char *tok, int *out){
    char *end;
    long v = strtol(tok, &end, 10);
    if(end == tok || *end != '\0' || v < 0 || v> INT_MAX){
        return -1;
    }
    *out = (int)v;
    return 0;
}

static int parse_customer_line(char *line, int m, int *row){
    int virgulas = 0;
    for(int i = 0 ; line[i] != '\0';i++){
        if(line[i] == ','){
            virgulas++;
        }
    }
    if(virgulas + 1 != m){
        return -2;
    }

    int j =0;
    char *tok = strtok(line, ",");
    while(tok != NULL){
        int v;
        if(parse_nonneg_int(tok, &v)){
            return -1;
        }

        if(row != NULL){
            row[j]=v;
        }
        j++;
        tok=strtok(NULL, ",");
    }
    if(j != m){
        return -1;
    }
    return 0;
}

int read_customers(struct banker *b, const char *filename, int numResources){
    FILE *f = fopen(filename, "r");
    if(f == NULL){
        fprintf(stderr,"Erro: não foi possível abrir %s\n", filename);           
        return -1;
    }

    char line[LINE_MAX_LEN];
    int lineno = 0, count = 0;

    // valida as linhas e conta os clientes
    while(fgets(line, sizeof line, f) != NULL){
        lineno++;
        strip_newline(line);
        if(is_blank(line)) continue;

        int r = parse_customer_line(line, numResources, NULL); 
        if(r == -2){
            fprintf(stderr, "Erro: %s linha %d tem quantidade de recursos diferente dos argumentos (%d)\n", filename, lineno,numResources); 
            fclose(f);
            return -1;
        }
        if(r == -1){
            fprintf(stderr, "Erro: %s linha %d em formato inválido\n",filename, lineno);
            fclose(f);
            return -1;
        }
        count++;
    }
    if(count == 0){ 
        fprintf(stderr, "Erro: %s sem clientes\n", filename);
        fclose(f);
        return -1;
     }              

    // depois de saber quantos clientes tem alloca
    if(banker_alloc(b, count, numResources) != 0){
        fprintf(stderr, "Erro: memória insuficiente\n");
        fclose(f);
        return -1;
    } 

    rewind(f);
    int i = 0;
    while(fgets(line, sizeof line, f) != NULL){
        strip_newline(line);
        if(is_blank(line)) continue;
        parse_customer_line(line, numResources, b->maximum[i]);  
        for(int j = 0; j < numResources; j++){
            b->need[i][j] = b->maximum[i][j];                        
        }
        i++;
    }

    fclose(f);
    return 0;
}

int parse_command(char *line, int numResources, int numCustomers,char *type, int *customer, int *values){
    char *tok = strtok(line, " \t");
    if(tok == NULL){
     return -1;
    }
        

    if(strcmp(tok, "*") == 0){
        if(strtok(NULL, " \t") != NULL) return -1;
        *type = '*';
        return 0;
    }

    //RQ ou RL
    if(strcmp(tok, "RQ") == 0){
        *type = 'Q';
    } 
    else if(strcmp(tok, "RL") == 0){
        *type = 'L';
    } 
    else return -1;                 

    //cliente
    tok = strtok(NULL, " \t");
    if(tok == NULL || parse_nonneg_int(tok, customer) != 0){
        return -1;
    } 
    if(*customer >= numCustomers){ //cliente nao existe
        return -1; 
    } 

    // recursos
    int j = 0;
    tok = strtok(NULL, " \t");
    while(tok != NULL){
        if(j == numResources){
            return -2;
        }
        if(parse_nonneg_int(tok, &values[j]) != 0){
            return -1;
        }
        j++;
        tok = strtok(NULL, " \t");
    }
    if(j < numResources){
        return -2;
    }
    return 0;
}

int validate_commands(const char *filename, int numCustomers, int numResources){
    FILE *f = fopen(filename, "r");
    if(f == NULL){ 
        fprintf(stderr, "Erro: Não foi possivel abrir o arquivo %s\n", filename);
        return -1;
     }                      

    int *values = malloc(numResources*sizeof(int));
    if(values == NULL){
        fprintf(stderr, "Erro: Memória insuficiente\n");
        fclose(f);
        return -1;
    }

    char line[LINE_MAX_LEN];
    int lineno = 0;
    char type;
    int customer;

    while(fgets(line, sizeof line, f) != NULL){
        lineno++;
        strip_newline(line);
        if(is_blank(line)) continue;

        int r = parse_command(line, numResources, numCustomers, &type, &customer, values);
        if(r == -2){
            fprintf(stderr, "Erro: %s linha %d tem quantidade de recursos diferente dos argumentos (%d)\n", filename , lineno, numResources);
            free(values);
            fclose(f);
            return -1;
        }
        if(r == -1){
            fprintf(stderr, "Erro: %s linha %d em formato inválido\n", filename, lineno);
            free(values);
            fclose(f);
            return -1;
        }   
    }

    free(values);
    fclose(f);
    return 0;
}