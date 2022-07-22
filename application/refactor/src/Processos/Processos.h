#ifndef __PROCESSOS_H__
#define __PROCESSOS_H__

#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>
#include "../Constantes/Constantes.h"

typedef struct Processos {
    pid_t pid;
    pid_t ppid;
    int pc;    // Initially started from zero.
    int valor;
    int inicialMEM;
    int tamanho;
    int prioridade;
    char nome_arquivo[MAX_MEM]; 
    char programa[MAX_MEM][MAX_LINHA];
} Processos;

Processos duplica_processo(Processos *proc, pid_t novo);
Processos criar_processo(pid_t pid, pid_t ppid, int pc, int valor, int prioridade, char *nome_arquivo);
void copiar_proc(Processos *proc, Processos procB);

#endif // __PROCESSOS_H__
