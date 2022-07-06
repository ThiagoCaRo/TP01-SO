#include "Processos.h"

#include <stdio.h>
#include <string.h>

Processos duplica_processo(Processos *proc, pid_t novo){
    static Processos duplicata;
    duplicata.pid = novo;
    duplicata.ppid = proc->pid;
    duplicata.pc = proc->pc;
    duplicata.prioridade = proc->prioridade;
    duplicata.valor = proc->valor;
    strcpy(duplicata.nome_arquivo, proc->nome_arquivo);
    ler_programa(duplicata.nome_arquivo, duplicata.programa);
    return duplicata;
}

Processos criar_processo(pid_t pid, pid_t ppid, int pc, int valor, int prioridade, char *nome_arquivo){
    static Processos processos;
    processos.pid = pid;
    processos.ppid = ppid;
    processos.pc = pc;
    processos.valor = valor;
    processos.prioridade = prioridade;
    strcpy(processos.nome_arquivo, nome_arquivo);
    ler_programa(processos.nome_arquivo, processos.programa);
    return processos;
}