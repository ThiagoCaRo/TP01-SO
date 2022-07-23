#ifndef __CPU_H__
#define __CPU_H__

#include <unistd.h>
#include <inttypes.h>
#include <semaphore.h>
#include <sys/types.h>
#include "../Fila/Fila.h"
#include "../Constantes/Constantes.h"
#include "../MemoriaVirtual/MemoriaV.h"

#define PRIORI_0    0
#define PRIORI_1    1
#define PRIORI_2    2
#define PRIORI_3    3

typedef struct CPU{
    pid_t EXEC;
    int pc;
    int n;
    int X[MAX_MEM];
    int indice, valor;
} CPU;

void processo_main(int file_descriptor);
void gerenciador_processos(int file_descriptor);
void transfere_cpu(Processos *proc, CPU *cpu);
void transfere_tabela(CPU *cpu, Processos *proc);
void processo_impressao(CPU *cpu, Fila prontos[4], Fila *bloqueados, Processos tabela[MAX_MEM], MEMORIA_VIRTUAL memoriaVirtual);

#endif // __CPU_H__
