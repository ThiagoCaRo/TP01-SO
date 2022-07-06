#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdint.h>
#include <pthread.h>
#include <semaphore.h>
#include <inttypes.h>
#include <signal.h>
#include <stdbool.h>
#define THREADSTACKSIZE    1024
#define true    1
#define false    0
#define MAX_MEM    100
#define MAX_LINHA    50  
#define PRIORI_0    0
#define PRIORI_1    1
#define PRIORI_2    2
#define PRIORI_3    3

/******************************************************************************
                                TABELA DE TEMPO
*******************************************************************************/

/*Prioridade 0:|                            10ms                                |
/*Prioridade 1:|                            15ms                                |
/*Prioridade 2:|                            25ms                                |
/*Prioridade 3:|                            50ms    Mesclado com round robin    |

/******************************************************************************
*******************************************************************************/

typedef struct{
    pid_t pid;
    pid_t ppid;
    int pc;    // Initially started from zero.
    int valor;
    int prioridade;
    char nome_arquivo[MAX_MEM]; 
    char programa[MAX_MEM][MAX_LINHA];
}Processos;

typedef struct{
    pid_t EXEC;
    int pc;
    int n;
    pid_t pid;
    int X[MAX_MEM];
    int indice, prioridade, valor;
}CPU;

typedef struct fila{
    Processos proc[MAX_MEM];
}FILA;

double time_diff(struct timespec *start, struct timespec *end);


int ler_programa(char *file, char programa[MAX_MEM][MAX_LINHA]);

void processo_main(int file_descriptor);
void gerenciador_processos(int file_descriptor);
