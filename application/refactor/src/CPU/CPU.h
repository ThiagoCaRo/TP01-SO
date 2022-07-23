#ifndef __CPU_H__
#define __CPU_H__

#include <unistd.h>
#include <inttypes.h>
#include <semaphore.h>
#include <sys/types.h>
#include "../Fila/Fila.h"
#include "../Constantes/Constantes.h"
#include "../MemoriaPrincipal/MemoriaP.h"


// define as prioridades possiveis para a fila
#define PRIORI_0    0
#define PRIORI_1    1
#define PRIORI_2    2
#define PRIORI_3    3

typedef struct CPU{
    pid_t EXEC; // processo atualmente em execucao
    int pc; // linha de comando que esta executando
    int n; // tamanho do programa em execucao
    int X[MAX_MEM]; // sistema antigo de memoria(nao usado)
    int indice, valor; // variaveis auxiliares para a manipulacao dos valores a serem computados
} CPU;

void processo_main(int file_descriptor);
void gerenciador_processos(int file_descriptor);
void transfere_cpu(Processos *proc, CPU *cpu);
void transfere_tabela(CPU *cpu, Processos *proc);
void processo_impressao(CPU *cpu, Fila prontos[4], Fila *bloqueados, Processos tabela[MAX_MEM], MEMORIA_PRINCIPAL memoria);

#endif // __CPU_H__
