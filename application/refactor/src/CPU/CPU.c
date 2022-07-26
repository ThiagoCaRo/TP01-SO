#include "CPU.h"
#include "../Arquivo/Arquivo.h"

#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <math.h>
#define verde(c) printf("\33[0;%dm%c\33[0m",32,c)

void transfere_tabela(CPU *cpu, Processos *proc) { // transfere da CPU para alguma variavel processo
    proc->pc = cpu->pc;
    proc->pid = cpu->EXEC;
    proc->valor = cpu->valor;
}

void transfere_cpu(Processos *proc, CPU *cpu) { // coloca um determinado processo como o padrão dentro da cpu
    cpu->pc = proc->pc;
    cpu->EXEC = proc->pid;
	cpu->n = proc->tamanho;
    cpu->valor = proc->valor;
	cpu->indice = proc->inicialMEM;
}

void processo_impressao(CPU *cpu, Fila prontos[4], Fila *bloqueados,Processos tabela[MAX_MEM], MEMORIA_PRINCIPAL memoria,
                        int contador_vaos, int contador_tempo, int contador_nos, int contador_alocacoes, int alocacao_negada) {
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
        printf("   | %d | ", bloqueados->proc[i].pid);
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
            if (tabela[j].inicialMEM != -1 && tabela[j].inicialMEM + tabela[j].tamanho > i && tabela[j].inicialMEM <= i){
                process = j;
                break;
            }
        }
        if (process == -1) printf("%d ", memoria.RAM[i]);
        else {
            printf("\33[0;%dm%d\33[0m ",33+process,memoria.RAM[i]);
        }
	}
	printf("\n\n");

    for(int i = 0; i < MAX_MEM; i++){

        if(memoria.pageframe[i].index != -1){
            printf("I: %d  TAMANHO DO VÃO %d   INDICE %d\n",i,memoria.pageframe[i].tamanho,memoria.pageframe[i].index);
        }

    }

    printf("Numero médio de Fragmentos Externos: %lf\n", ((float)contador_vaos)/contador_tempo);
    printf("Tempo médio de Alocações: %lf\n", ((float)contador_nos/contador_alocacoes));
    printf("Percentual de alocações negadas: %.2f%%\n", ((float)alocacao_negada/contador_alocacoes)*100);
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
    int contador_nos=0;
    int contador_alocacoes=0;
    int alocacao_negada=0;
	Processos tabela[MAX_MEM];
	CPU cpu;
	Fila PRONTOS[4]; // filas de prioridade, uma pra cada prioridade
	Fila BLOQUEADOS;
	MEMORIA_PRINCIPAL memoria;

	
	read(file_descriptor,rx,sizeof(rx));
	printf("\n===========\nRecebendo via pipe = %s\n===========\n",rx);
	
	char tipoFit;
	
	printf("Escolha o tipo de técnica para alocar memória:\nf: First fit\nn: Next fit\nb: Best fit\nw: Worst fit\n");
	scanf(" %c",&tipoFit); // o usuario escolhe o tipo de algortimo de fit
	int next = 0;
	
	inicializa_vazia(&memoria); // inicializa a memoria vazia
	
	//inicializa a cpu vazia e coloca o primeiro processo em execução
	cpu.pc = 0;
	cpu.EXEC = 0;
	cpu.valor = 0;
	tabela[cpu.EXEC] = criar_processo(0, -1, 0, 0, PRIORI_0, "init.txt");

	for(int i = 0; i < MAX_MEM; i++){
		tabela[i].inicialMEM = -1; // coloca a memoria alocada para cada processo inexistente
	}

	//Inicializa as Filas
	for (int i = 0; i < 4; i++)
		cria_fila(&(PRONTOS[i]));
	cria_fila(&BLOQUEADOS);

	//strcpy(cpu.programa,tabela[cpu.pid].programa)

	int counter = 0, contador_vaos=0, contador_tempo = 0;
	while (counter<strlen(rx)) { // varre todos os comandos do arquivo mestre ou digitados pelo usuario
		for (int i = 0; i < strlen(rx); i++){ // mostra todos os comandos que ainda tem pra executar
			if (i != counter) printf("%c",rx[i]);
			else verde(rx[i]); // em verde aquele que esta em execução
		}
		if (rx[counter] != 'U')
			printf("\n");
		
		transfere_tabela(&cpu, &tabela[cpu.EXEC]);
		command = rx[counter];// recebe o comando atual
	
		switch (command) {
		    case 'U': // caso o comando seja uma unidade de tempo


		    	if (cpu.EXEC != -1) tempo++; // o tempo de processo em execução é incrementado caso haja um processo em execução
		    	else { // caso nao ele tira um da fila de prontos caso exista
					for (int j = 3; j >= 0; j--){
						if (!isVazia(PRONTOS[j])) {
							Processos procaux;
							desinfileira(&(PRONTOS[j]), &procaux);
							tempo = 1;
							
							transfere_cpu(&procaux, &cpu);
						} 
					}
		    	}
		
			if (tempo>pow(2,tabela[cpu.EXEC].prioridade)) { // caso seu quantum já tenha sido satisfeito
				if(tabela[cpu.EXEC].prioridade != PRIORI_3) 
					tabela[cpu.EXEC].prioridade++;
				enfileirar(&(PRONTOS[tabela[cpu.EXEC].prioridade]),tabela[cpu.EXEC]); // coloca na fila de prontos
				cpu.EXEC = -1; // esvazia a cpu
				for (int j = 3; j >= 0; j--){ // coloca o proximo processo na cpu
					if (!isVazia(PRONTOS[j])) {
						Processos procaux;
						desinfileira(&(PRONTOS[j]), &procaux);
						tempo = 1;
						
						transfere_cpu(&procaux, &cpu);
						break;
					} 
				}
			}
			
			state = tabela[cpu.EXEC].programa[cpu.pc][0]; // extrai o comando atual do programa
            if (cpu.EXEC != -1){ // caso haja um processo na cpu
				printf("  STATE: %c\n", state);
				switch(state) {
					int auxMemCopy;
				    case 'N': // caso o comando seja de declaracao do numero de variaveis
						
						if(tabela[cpu.EXEC].inicialMEM != -1){ // desaloca caso já tenha sido alocado para esse processo
							removePagina(&memoria, cpu.n, tabela[cpu.EXEC].inicialMEM);
						}
						sscanf(tabela[cpu.EXEC].programa[cpu.pc],"%*[^0123456789]%d",&cpu.n); // obtem seu tamanho
						

						tabela[cpu.EXEC].inicialMEM = Fit(&memoria, cpu.n,tipoFit,&next, &contador_nos); // coloca na memoria principal e o processo recebe seu ponto de partida
                        contador_alocacoes +=1;
						if(tabela[cpu.EXEC].inicialMEM == -1){ // caso nao tenha espaco aloca na fila de prontos
                            alocacao_negada +=1;
							enfileirar(&(PRONTOS[tabela[cpu.EXEC].prioridade]),tabela[cpu.EXEC]);
							cpu.EXEC = -1;
							break;
						}
						tabela[cpu.EXEC].tamanho = cpu.n;
						
						
					break;


				    case 'V':
						sscanf(tabela[cpu.EXEC].programa[cpu.pc],"%*[^0123456789]%d%*[^0123456789]%d",&cpu.indice, &cpu.valor);
						memoria.RAM[tabela[cpu.EXEC].inicialMEM + cpu.indice] = cpu.valor; // coloca o valor na posicao da ram pertencente a aquele processo
					break;

				    case 'A':
						sscanf(tabela[cpu.EXEC].programa[cpu.pc],"%*[^0123456789]%d%*[^0123456789]%d",&cpu.indice, &cpu.valor);
						memoria.RAM[tabela[cpu.EXEC].inicialMEM + cpu.indice]+=cpu.valor; // soma o valor na posicao da ram que pertence a aquele processo
						//cpu.X[cpu.indice]+=cpu.valor;
					break;

				    case 'S':
						sscanf(tabela[cpu.EXEC].programa[cpu.pc],"%*[^0123456789]%d%*[^0123456789]%d",&cpu.indice, &cpu.valor);
						memoria.RAM[tabela[cpu.EXEC].inicialMEM + cpu.indice]-=cpu.valor; // subtrai o valor na posicao da ram que pertence a aquele processo
						//cpu.X[cpu.indice]-=cpu.valor;
					break;

				    case 'B': // bloqueio
						if (tabela[cpu.EXEC].prioridade != PRIORI_0) { // caso o quantum nao tenha sido atendido
							tabela[cpu.EXEC].prioridade--;
						}
						enfileirar(&BLOQUEADOS, tabela[cpu.EXEC]); // bloqueia o processo colocando na fila de bloqueados
						cpu.EXEC = -1; // esvazia a cpuu
						tempo = 0;
						for (int j = 3; j >= 0; j--){ // coloca outro processo das filas de prontos caso exista
							if (!isVazia(PRONTOS[j])) {
								Processos procaux;
								desinfileira(&(PRONTOS[j]), &procaux);
								tempo = 1;
								
								transfere_cpu(&procaux, &cpu);
							} 
						}
					break;

				    case 'T': // termina o processo
						removePagina(&memoria, cpu.n, tabela[cpu.EXEC].inicialMEM); // remove a sua fracao da memoria
						cpu.EXEC = -1; // esvazia a cpu
						for (int j = 3; j >= 0; j--){ // retira outro processo
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
						tabela[process_counter]  = duplica_processo(&tabela[cpu.EXEC], process_counter); // duplica o processo
						
						auxMemCopy = Fit(&memoria, cpu.n,tipoFit, &next,&contador_nos); // coloca o novo processo na memoria
                        			contador_alocacoes +=1;
						if(auxMemCopy == -1){ // caso nao a memoria suficiente para aquela simulação
							fprintf(stderr, "%s","Memoria principal cheia para processo duplicado\n");
							exit(1);
							//enfileirar(&(PRONTOS[tabela[process_counter].prioridade]),tabela[process_counter]);
						}
						
						tabela[process_counter].inicialMEM = auxMemCopy; // coloca no processo seu endereco de memoria inicial
						tabela[process_counter].tamanho = cpu.n;
						for(int i=0; i < cpu.n; i++){ // copia os valores de um processo para o outro
							memoria.RAM[i+tabela[process_counter].inicialMEM] = memoria.RAM[i+tabela[cpu.EXEC].inicialMEM];
						}
						enfileirar(&(PRONTOS[tabela[process_counter].prioridade]), tabela[process_counter]); // coloca o novo processo na fila de prontos

					break;

				    case 'R':
						strncpy(file_name,&tabela[cpu.EXEC].programa[cpu.pc][2], MAX_MEM - 1); // troca de programa
						cpu.pc = -1;
						cpu.valor = 0;
						strcpy(tabela[cpu.EXEC].nome_arquivo, file_name);
						ler_programa(file_name, tabela[cpu.EXEC].programa);
						tempo = 0;
					break;

					case '\0':
					break;
				}
				cpu.pc++; // incrementa o contador de instrucoes do programa em execucao
				
			}
			else 
				printf("\n");

            for(int i = 0; i < MAX_MEM; i++){

                if(memoria.pageframe[i].index != -1){
                    contador_vaos +=1;
                }

            }
            contador_tempo+=1;

			break;

		    case 'L': // desbloqueia da fila de bloqueados

				if (isVazia(BLOQUEADOS)) { // verifica se a fila de bloqueados esta vazia
					break;
				}
				
				Processos procaux;
				desinfileira(&BLOQUEADOS, &procaux);
				enfileirar(&(PRONTOS[procaux.prioridade]), procaux); // enfileira na fila de prontos

			break;

		    case 'I':
				fflush(stdout);
				processo_impressao(&cpu, PRONTOS, &BLOQUEADOS,tabela, memoria,contador_vaos, contador_tempo, contador_nos, contador_alocacoes, alocacao_negada); // imprime o estado atual da simulacao
			break;

		    case 'M':
				printf("Saindo do simulador\n"); // sai da simulacao
				exit(0);
			break;

		    default:
			break;
		    
		}
		counter++; // incrementa o contador de comandos do simulador
	}
}
