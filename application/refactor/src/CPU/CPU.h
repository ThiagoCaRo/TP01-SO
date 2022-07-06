#ifndef __CPU_H__
#define __CPU_H__

#include <unistd.h>
#include <inttypes.h>
#include <semaphore.h>
#include <sys/types.h>
#include "../Fila/Fila.h"
#include "../Arquivo/Arquivo.h"

#define true    1
#define false    0
#define PRIORI_0    0
#define PRIORI_1    1
#define PRIORI_2    2
#define PRIORI_3    3
#define THREADSTACKSIZE    1024

typedef struct CPU{
    pid_t EXEC;
    int pc;
    int n;
    pid_t pid;
    int X[MAX_MEM];
    int indice, prioridade, valor;
} CPU;

void processo_main(int file_descriptor);
void gerenciador_processos(int file_descriptor);
void transfere_cpu(Processos *proc, CPU *cpu);
void transfere_tabela(CPU *cpu, Processos *proc);
double time_diff(struct timespec *start, struct timespec *end);
void processo_impressao(CPU *cpu, Fila *prontos, Fila *bloqueados, Processos tabela[MAX_MEM]);

#endif // __CPU_H__