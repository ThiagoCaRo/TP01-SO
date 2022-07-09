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

void copiar_proc(Processos *proc, Processos procB){
	strcpy(proc->nome_arquivo, procB.nome_arquivo);
	proc->pc = procB.pc;
	proc->pid = procB.pid;
	proc->ppid = procB.ppid;
	proc->prioridade = procB.prioridade;
	proc->valor = procB.valor;
	for(int j=0;j<MAX_MEM;j++) {
		strcpy(proc->programa[j] , procB.programa[j]);
	}
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
