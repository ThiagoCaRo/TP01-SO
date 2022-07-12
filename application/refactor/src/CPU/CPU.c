#include "CPU.h"

#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <math.h>
#define verde(c) printf("\33[0;32m%c\33[0m",c)

void transfere_tabela(CPU *cpu, Processos *proc) {
    proc->pc = cpu->pc;
    proc->pid = cpu->EXEC;
    proc->valor = cpu->valor;
    //proc->t_used = cpu->t_remain;
}

void transfere_cpu(Processos *proc, CPU *cpu) {
    cpu->pc = proc->pc;
    cpu->EXEC = proc->pid;
    cpu->valor = proc->valor;
}

void processo_impressao(CPU *cpu, Fila prontos[4], Fila *bloqueados,Processos tabela[MAX_MEM]) {
    
    printf("======================SIMULADOR====================\n\n\n");
    printf("* Processo atualmente em execucao na CPU:\n\n");
    printf("                PID: %d\n", cpu->EXEC);

    for(int a = 0; a < MAX_MEM; a++) {
        if (cpu->X[a] == 0) break;

        printf("    X[%d] = %d  ", a, cpu->X[a]);
    }

    printf("\n    Prioridade: %d", tabela[cpu->EXEC].prioridade);
    printf("\n\n");
    printf("    CONTADOR DE PROGRAMA (PC): %d\n\n", cpu->pc);
	for (int j = 0; j < 4; j++){
		printf("\n\n* Lista de IDs dos processos em estado PRONTO com prioridade %d:\n\n",j);

		for (int i = 0; i < prontos[j].finalFila; i++) {
			printf("   | %d |         ", prontos[j].proc[i].pid);
		}
		printf("\n");

		for (int i = 0; i < prontos[j].finalFila; i++) {
			printf("PC[%d] = %d        ", i, prontos[j].proc[i].pc);
    }
	}
    printf("\n\n");
    printf("* Lista de IDs dos processos em estado BLOQUEADO:\n\n");
    
    for (int i = 0; i < bloqueados->finalFila;i++) {
        printf("   | %d | ", bloqueados->proc[i].pid);
    }
    printf("\n");
    
    for (int i = 0; i < bloqueados->finalFila;i++) {
        printf("PC[%d] = %d        ",i,bloqueados->proc[i].pc);
    }
    printf("\n\n");
    return;
    
}

void processo_main(int file_descriptor) {
    FILE *f;
    int op;
    char tx[MAX_MEM];
    char temp[2];
    f=fopen("arquivo_mestre.txt","r");
    printf("0 - Ler do arquivo           1 - Ler por entrada padrao\n\n");
    scanf("%d",&op);
    if (op == 0) {
        while (1) {
            if (fgets(temp, sizeof (temp), f) == NULL) {
                break;
            }
            temp[strcspn(temp, "\n")] = 0;
            strcat(tx,temp);
        }
    }

    else{
        printf("Entre com a sequencia de comandos no formato de string - ONLY CAPITAL LETTERS\n");
        scanf("%s",tx);
    }
    
    write(file_descriptor, tx, sizeof(tx));
    fclose(f);
    printf("\n===========\nMandando via pipe = %s\n===========\n",tx);
    
}

void gerenciador_processos(int file_descriptor) {
	char rx[MAX_MEM];
	char file_name[MAX_MEM];
	char state; 
	char command;
	int tempo = 0;
	int process_counter = 0;
	Processos tabela[MAX_MEM];
	CPU cpu;
	Fila PRONTOS[4];
	Fila BLOQUEADOS;

	
	read(file_descriptor,rx,sizeof(rx));
	printf("\n===========\nRecebendo via pipe = %s\n===========\n",rx);
	cpu.pc = 0;
	cpu.EXEC = 0;
	cpu.valor = 0;
	tabela[cpu.EXEC] = criar_processo(0, -1, 0, 0, PRIORI_0, "init.txt");

	//Inicializa as Filas
	for (int i = 0; i < 4; i++)
		cria_fila(&(PRONTOS[i]));
	cria_fila(&BLOQUEADOS);

	//strcpy(cpu.programa,tabela[cpu.pid].programa)

	int counter = 0;
	while (counter<strlen(rx)) {
		for (int i = 0; i < strlen(rx); i++){
			if (i != counter) printf("%c",rx[i]);
			else verde(rx[i]);
		}
		printf("\n");
		transfere_tabela(&cpu, &tabela[cpu.EXEC]);
		command = rx[counter];
		switch (command) {
		    case 'U':
		    	if (cpu.EXEC != -1) tempo++;
		    	else {
				for (int j = 3; j >= 0; j--){
					if (!isVazia(PRONTOS[j])) {
						Processos procaux;
						desinfileira(&(PRONTOS[j]), &procaux);
						tempo = 1;
						
						transfere_cpu(&procaux, &cpu);
					} 
				}
		    	}
		
			if (tempo>pow(2,tabela[cpu.EXEC].prioridade)) {
				if(tabela[cpu.EXEC].prioridade != PRIORI_3) 
					tabela[cpu.EXEC].prioridade++;
				enfileirar(&(PRONTOS[tabela[cpu.EXEC].prioridade]),tabela[cpu.EXEC]);
				cpu.EXEC = -1;
				for (int j = 3; j >= 0; j--){
					if (!isVazia(PRONTOS[j])) {
						Processos procaux;
						desinfileira(&(PRONTOS[j]), &procaux);
						tempo = 1;
						
						transfere_cpu(&procaux, &cpu);
						break;
					} 
				}
			}
			
			state = tabela[cpu.EXEC].programa[cpu.pc][0];
		    	if (cpu.EXEC != -1){
				switch(state) {
				    case 'N':
					sscanf(tabela[cpu.EXEC].programa[cpu.pc],"%*[^0123456789]%d",&cpu.n);
					break;
				    case 'D':
					for(int x=0;x<cpu.n;x++) {
					    cpu.X[x] = 0;
					}
					break;
				    case 'V':
					sscanf(tabela[cpu.EXEC].programa[cpu.pc],"%*[^0123456789]%d%*[^0123456789]%d",&cpu.indice, &cpu.valor);
					cpu.X[cpu.indice] = cpu.valor;
					break;
				    case 'A':
					sscanf(tabela[cpu.EXEC].programa[cpu.pc],"%*[^0123456789]%d%*[^0123456789]%d",&cpu.indice, &cpu.valor);
					cpu.X[cpu.indice]+=cpu.valor;
					break;
				    case 'S':
					sscanf(tabela[cpu.EXEC].programa[cpu.pc],"%*[^0123456789]%d%*[^0123456789]%d",&cpu.indice, &cpu.valor);
					cpu.X[cpu.indice]-=cpu.valor;
					break;
				    case 'B':
					if (tabela[cpu.EXEC].prioridade != PRIORI_0) {
					    tabela[cpu.EXEC].prioridade--;
					}
					enfileirar(&BLOQUEADOS, tabela[cpu.EXEC]);
					cpu.EXEC = -1;
					tempo = 0;
					for (int j = 3; j >= 0; j--){
						if (!isVazia(PRONTOS[j])) {
							Processos procaux;
							desinfileira(&(PRONTOS[j]), &procaux);
							tempo = 1;
							
							transfere_cpu(&procaux, &cpu);
						} 
					}
					break;
				    case 'T':
					for (int j = 3; j >= 0; j--){
						if (!isVazia(PRONTOS[j])) {
							desinfileira(&(PRONTOS[j]), &tabela[cpu.EXEC]);
						}
					}

					break;
				case 'F':
					process_counter++;
					transfere_tabela(&cpu, &tabela[cpu.EXEC]);
					tabela[process_counter]  = duplica_processo(&tabela[cpu.EXEC], process_counter);

					enfileirar(&(PRONTOS[tabela[process_counter].prioridade]), tabela[process_counter]);

					break;
				    case 'R':
					
					strncpy(file_name,&tabela[cpu.EXEC].programa[cpu.pc][2], MAX_MEM - 1);
					cpu.pc = 0;
					cpu.valor = 0;
					strcpy(tabela[cpu.EXEC].nome_arquivo, file_name);
					ler_programa(file_name, tabela[cpu.EXEC].programa);
					tempo = 0;
					break;
				    case '\0':
					break;
				}
				cpu.pc++;
			}
			break;
		    case 'L':

			if (isVazia(BLOQUEADOS)) {
			    break;
			}
			
			Processos procaux;
			desinfileira(&BLOQUEADOS, &procaux);
			enfileirar(&(PRONTOS[procaux.prioridade]), procaux);

			break;
		    case 'I':
			fflush(stdout);
			processo_impressao(&cpu, PRONTOS, &BLOQUEADOS,tabela);
			break;

		    case 'M':
			printf("Saindo do simulador\n");
			exit(0);
			break;

		    default:
			break;
		    
		}
		counter++;
	}
}