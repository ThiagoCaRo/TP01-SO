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
#define PRONTO    'P'
#define EXECUCAO    'E'
#define BLOQUEADO    'O'

typedef struct{
    pid_t pid;
    pid_t ppid;
    int pc;    // Initially started from zero.
    int valor;
    int t_start;
    int t_used;
    char nome_arquivo[MAX_MEM]; 
    char programa[MAX_MEM][MAX_LINHA];
}Processos;

typedef struct{
    pid_t EXEC;
    int pc;
    int n;
    pid_t pid;
    int X[MAX_MEM];
    int indice, valor;
}CPU;

typedef struct fila{
    Processos proc[MAX_MEM];
}FILA;

double time_diff(struct timespec *start, struct timespec *end);
void transfere_tabela(CPU *cpu, Processos *proc);
void transfere_cpu(Processos *proc, CPU *cpu);
int ler_programa(char *file, char programa[MAX_MEM][MAX_LINHA]);
Processos duplica_processo(Processos *proc, pid_t novo);
Processos criar_processo(pid_t pid, pid_t ppid, int pc, int valor, char *nome_arquivo);
void processo_impressao(CPU *cpu, FILA *prontos, FILA *bloqueados, Processos tabela[MAX_MEM]);
void processo_main(int file_descriptor);
void gerenciador_processos(int file_descriptor);
