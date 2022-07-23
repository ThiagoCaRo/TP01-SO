#include "CPU.h"
#include "../Arquivo/Arquivo.h"

#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <math.h>
#define verde(c) printf("\33[0;%dm%c\33[0m",32,c)

void transfere_tabela(CPU *cpu, Processos *proc) {
    proc->pc = cpu->pc;
    proc->pid = cpu->EXEC;
    proc->valor = cpu->valor;
}

void transfere_cpu(Processos *proc, CPU *cpu) {
    cpu->pc = proc->pc;
    cpu->EXEC = proc->pid;
	cpu->n = proc->tamanho;
    cpu->valor = proc->valor;
	cpu->indice = proc->inicialMEM;
}

void processo_impressao(CPU *cpu, Fila prontos[4], Fila *bloqueados,Processos tabela[MAX_MEM], MEMORIA_VIRTUAL memoriaVirtual) {
    
    printf("======================SIMULADOR====================\n\n\n");
    printf("* Processo atualmente em execucao na CPU:\n\n");
    printf("                PID: \33[0;%dm%d\33[0m\n", cpu->EXEC+33, cpu->EXEC);

    for(int a = 0; a < MAX_MEM; a++) {
        if (cpu->X[a] == 0) break;

        printf("    X[%d] = %d  ", a, cpu->X[a]);
    }

    if (cpu->EXEC != -1){
        printf("\n    Prioridade: %d", tabela[cpu->EXEC].prioridade);
        printf("\n\n");
        printf("    CONTADOR DE PROGRAMA (PC): %d\n\n", cpu->pc);
    }
	for (int j = 0; j < 4; j++){
		printf("\n\n* Lista de IDs dos processos em estado PRONTO com prioridade %d:\n\n",j);

		for (int i = 0; i < prontos[j].finalFila; i++) {
			printf("   | \33[0;%dm%d\33[0m |         ", 33+prontos[j].proc[i].pid,prontos[j].proc[i].pid);
		}
		printf("\n");

		for (int i = 0; i < prontos[j].finalFila; i++) {
			printf("PC[%d] = %d        ", i, prontos[j].proc[i].pc);
    }
	}
    printf("\n\n");
    printf("* Lista de IDs dos processos em estado BLOQUEADO:\n\n");
    
    for (int i = 0; i < bloqueados->finalFila;i++) {
        printf("   | \33[0;%dm%d\33[0m |         ", 33+bloqueados->proc[i].pid,bloqueados->proc[i].pid);
    }
    printf("\n");
    
    for (int i = 0; i < bloqueados->finalFila;i++) {
        printf("PC[%d] = %d        ",i,bloqueados->proc[i].pc);
    }
    printf("\n\n");

    printf("Memoria principal:\n");
	for(int i = 0; i < MAX_RAM; i++){
        int process = -1;
        for (int j = 0; j < MAX_MEM; j++){
            if (tabela[j].inicialMEM != -1){
                for (int k = tabela[j].inicialMEM; k < tabela[j].tamanho + tabela[j].inicialMEM; k++) {
                    if (memoriaVirtual.VIR[k].index == i &&
                        memoriaVirtual.VIR[k].naPrincipal == 1) {
                        process = j;
                        break;
                    }
                }
            }
        }
        if (process == -1) printf("%d ", memoriaVirtual.memoriaPrincipal.variaveis[i]);
        else {
            printf("\33[0;%dm%d\33[0m ",33+process,memoriaVirtual.memoriaPrincipal.variaveis[i]);
        }
	}
	printf("\n\n");

    for(int i = 0; i < MAX_MEM; i++){
        if(memoriaVirtual.memoriaPrincipal.pageframe[i].index != -1){
            printf("I: %d  TAMANHO DO VÃO %d   INDICE %d\n",i,memoriaVirtual.memoriaPrincipal.pageframe[i].tamanho,memoriaVirtual.memoriaPrincipal.pageframe[i].index);
        }
    }
    printf("\n\n");

    printf("Memoria Secundaria:\n");
    for(int i = 0; i < MAX_DISC; i++){
        int process = -1;
        for (int j = 0; j < MAX_MEM; j++){
            if (tabela[j].inicialMEM != -1){
                for (int k = tabela[j].inicialMEM; k < tabela[j].tamanho + tabela[j].inicialMEM; k++) {
                    if (memoriaVirtual.VIR[k].index == i &&
                        memoriaVirtual.VIR[k].naPrincipal == 0) {
                        process = j;
                        break;
                    }
                }
            }
        }
        if (process == -1) printf("%d ", memoriaVirtual.disco.variaveis[i]);
        else {
            printf("\33[0;%dm%d\33[0m ",33+process,memoriaVirtual.disco.variaveis[i]);
        }
    }
    printf("\n\n");

    for(int i = 0; i < MAX_MEM; i++){
        if(memoriaVirtual.disco.pageframe[i].index != -1){
            printf("I: %d  TAMANHO DO VÃO %d   INDICE %d\n",i,memoriaVirtual.disco.pageframe[i].tamanho,memoriaVirtual.disco.pageframe[i].index);
        }
    }
    printf("\n\n");

    printf("Memoria virtual:\n");
    printf("Contador de acesso: %d\n", memoriaVirtual.acessos);
    printf("Valores     |");
    for(int i = 0; i < MAX_VIR; i++){
        int process = -1;
        for (int j = 0; j < MAX_MEM; j++){
            if (tabela[j].inicialMEM + tabela[j].tamanho > i && tabela[j].inicialMEM <= i){
                process = j;
                break;
            }
        }

        if (process == -1) printf("X|");
        else {
            if(memoriaVirtual.VIR[i].index != -1){
                if (memoriaVirtual.VIR[i].naPrincipal)
                    printf("\33[0;%dm%4d\33[0m|",33+process,memoriaVirtual.memoriaPrincipal.variaveis[memoriaVirtual.VIR[i].index]);
                if (!memoriaVirtual.VIR[i].naPrincipal)
                    printf("\33[0;%dm%4d\33[0m|",33+process,memoriaVirtual.disco.variaveis[memoriaVirtual.VIR[i].index]);
                    //printf("\33[0;%dm%d\33[0m ",33+process,memoriaVirtual.memoriaPrincipal.variaveis[memoriaVirtual.VIR[i].index]);
            }
            else
                printf("\33[0;%dm%4d\33[0m|",33+process,0);
        }
    }
    printf("\nEndereco    |");
    for(int i = 0; i < MAX_VIR; i++){
        int process = -1;
        for (int j = 0; j < MAX_MEM; j++){
            if (tabela[j].inicialMEM + tabela[j].tamanho > i && tabela[j].inicialMEM <= i){
                process = j;
                break;
            }
        }

        if (process == -1) printf("X|");
        else {
            if(memoriaVirtual.VIR[i].index != -1)
                printf("\33[0;%dm%4d\33[0m|",33+process,memoriaVirtual.VIR[i].index);
            else
                printf("\33[0;%dmXXXX\33[0m|",33+process);
        }
    }
    printf("\nNaPrincipal |");
    for(int i = 0; i < MAX_VIR; i++) {
        int process = -1;
        for (int j = 0; j < MAX_MEM; j++) {
            if (tabela[j].inicialMEM + tabela[j].tamanho > i && tabela[j].inicialMEM <= i) {
                process = j;
                break;
            }
        }

        if (process == -1) printf("X|");
        else {
            if (memoriaVirtual.VIR[i].index != -1) {
                printf("\33[0;%dm%4d\33[0m|", 33 + process, memoriaVirtual.VIR[i].naPrincipal);
            }else
                printf("\33[0;%dmXXXX\33[0m|",33+process);
        }
    }
    printf("\nUltimoAcesso|");
    for(int i = 0; i < MAX_VIR; i++) {
        int process = -1;
        for (int j = 0; j < MAX_MEM; j++) {
            if (tabela[j].inicialMEM + tabela[j].tamanho > i && tabela[j].inicialMEM <= i) {
                process = j;
                break;
            }
        }

        if (process == -1) printf("X|");
        else {
            if (memoriaVirtual.VIR[i].index != -1) {
                printf("\33[0;%dm%4d\33[0m|", 33 + process, memoriaVirtual.VIR[i].ultimoAcesso);
            }else
                printf("\33[0;%dmXXXX\33[0m|",33+process);
        }
    }
    printf("\n\n");

    for(int i = 0; i < MAX_MEM; i++){
        if(memoriaVirtual.pageframe[i].index != -1){
            printf("I: %d  TAMANHO DO VÃO %d   INDICE %d\n",i,memoriaVirtual.pageframe[i].tamanho,memoriaVirtual.pageframe[i].index);
        }
    }
    printf("\n\n");
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
    MEMORIA_VIRTUAL memoriaVirtual; // agora usando memoria virtual

	
	read(file_descriptor,rx,sizeof(rx));
	printf("\n===========\nRecebendo via pipe = %s\n===========\n",rx);
	
	char tipoFit;
	
	printf("Escolha o tipo de técnica para alocar memória:\nf: First fit\nn: Next fit\nb: Best fit\nw: Worst fit\n");
	scanf(" %c",&tipoFit);
	int next = 0;
	
	inicializa_vaziaV(&memoriaVirtual); // inicializa a memoria virtual vazia
	cpu.pc = 0;
	cpu.EXEC = 0;
	cpu.valor = 0;
	tabela[cpu.EXEC] = criar_processo(0, -1, 0, 0, PRIORI_0, "init.txt");

	for(int i = 0; i < MAX_MEM; i++){
		tabela[i].inicialMEM = -1;
	}

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
		if (rx[counter] != 'U')
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
				printf("  STATE: %c\n", state);
				switch(state) {
				    case 'N':
						
						if(tabela[cpu.EXEC].inicialMEM != -1){
							removePaginaV(&memoriaVirtual, cpu.n, tabela[cpu.EXEC].inicialMEM); // agora remove a pagina virtual
						}
						sscanf(tabela[cpu.EXEC].programa[cpu.pc],"%*[^0123456789]%d",&cpu.n);

						tabela[cpu.EXEC].inicialMEM = FitV(&memoriaVirtual, cpu.n); // obtem seu endereco virtual
						tabela[cpu.EXEC].tamanho = cpu.n;
						
						
					break;

				    case 'V':
						sscanf(tabela[cpu.EXEC].programa[cpu.pc],"%*[^0123456789]%d%*[^0123456789]%d",&cpu.indice, &cpu.valor);
                        *GetAccess(&memoriaVirtual,tabela[cpu.EXEC].inicialMEM + cpu.indice,tipoFit,&next) = cpu.valor; // acessa o ponteiro de memoria da sua variavel e faz ela receber seu valor
					break;

				    case 'A':
						sscanf(tabela[cpu.EXEC].programa[cpu.pc],"%*[^0123456789]%d%*[^0123456789]%d",&cpu.indice, &cpu.valor);
                        *GetAccess(&memoriaVirtual,tabela[cpu.EXEC].inicialMEM + cpu.indice,tipoFit,&next) += cpu.valor; // acessa o poteniro de memoria da sua variavel e soma um valor
						//cpu.X[cpu.indice]+=cpu.valor;
					break;

				    case 'S':
						sscanf(tabela[cpu.EXEC].programa[cpu.pc],"%*[^0123456789]%d%*[^0123456789]%d",&cpu.indice, &cpu.valor);
                        *GetAccess(&memoriaVirtual,tabela[cpu.EXEC].inicialMEM + cpu.indice,tipoFit,&next) -= cpu.valor; // acessa o ponteiro da memoria da sua variavel e subtrai um valor
						//cpu.X[cpu.indice]-=cpu.valor;
					break;

				    case 'B':
						if (tabela[cpu.EXEC].prioridade != PRIORI_0) {
							tabela[cpu.EXEC].prioridade--;
						}
                        cpu.pc++;
						enfileirar(&BLOQUEADOS, tabela[cpu.EXEC]);
						cpu.EXEC = -1;
						tempo = 0;
						for (int j = 3; j >= 0; j--){
							if (!isVazia(PRONTOS[j])) {
								Processos procaux;
								desinfileira(&(PRONTOS[j]), &procaux);
								transfere_cpu(&procaux, &cpu);
                                cpu.pc--;
                                break;
							} 
						}
					break;

				    case 'T':
                        removePaginaV(&memoriaVirtual, cpu.n, tabela[cpu.EXEC].inicialMEM); // remove a pagina virtual
						cpu.EXEC = -1;
						for (int j = 3; j >= 0; j--){
							if (!isVazia(PRONTOS[j])) {
								Processos procaux;
								desinfileira(&(PRONTOS[j]), &procaux);
								tempo = 0;
								
								transfere_cpu(&procaux, &cpu);
								
							}
						}
						

					break;

					case 'F':
						process_counter++;
						transfere_tabela(&cpu, &tabela[cpu.EXEC]);
						tabela[process_counter]  = duplica_processo(&tabela[cpu.EXEC], process_counter);
						
						tabela[process_counter].inicialMEM = FitV(&memoriaVirtual, cpu.n); // coloca o processo duplicado na memoria virtual
						tabela[process_counter].tamanho = cpu.n;
						for(int i=0; i < cpu.n; i++){ // acessa variavel por variavel do processo antigo e coloca no novo
                            *GetAccess(&memoriaVirtual,i+tabela[process_counter].inicialMEM,tipoFit,&next) =
                                    *GetAccess(&memoriaVirtual,i+tabela[cpu.EXEC].inicialMEM,tipoFit,&next);
						}
						enfileirar(&(PRONTOS[tabela[process_counter].prioridade]), tabela[process_counter]);

					break;

				    case 'R':
						strncpy(file_name,&tabela[cpu.EXEC].programa[cpu.pc][2], MAX_MEM - 1);
						cpu.pc = -1;
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
			else 
				printf("\n");
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
				processo_impressao(&cpu, PRONTOS, &BLOQUEADOS,tabela, memoriaVirtual);
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
