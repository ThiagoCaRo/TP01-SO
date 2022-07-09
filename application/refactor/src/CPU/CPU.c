#include "CPU.h"

#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>

double time_diff(struct timespec *start, struct timespec *end) {

    //Função para calcular o tempo decorrido para round robin facilitado

    return (end->tv_sec - start->tv_sec) + 1e-9*(end->tv_nsec - start->tv_nsec);
}

void transfere_tabela(CPU *cpu, Processos *proc) {
    proc->pc = cpu->pc;
    proc->pid = cpu->pid;
    proc->valor = cpu->valor;
    //proc->t_used = cpu->t_remain;
}

void transfere_cpu(Processos *proc, CPU *cpu) {
    cpu->pc = proc->pc;
    cpu->pid = proc->pid;
    cpu->valor = proc->valor;
}

void processo_impressao(CPU *cpu, Fila *prontos, Fila *bloqueados, Processos tabela[MAX_MEM]) {
    FILE *output;
    output = fopen("output.txt", "w");
    if (output == NULL) {
        printf("Failure\n");
        exit(1);
    }
    
    fprintf(output,"               SAIDA DO SIMULADOR:\n\n\n");
    fflush(output);
    fprintf(output,"* Processo atualmente em execucao na CPU:\n\n");
    fflush(output);
    fprintf(output,"                PID: %d\n", cpu->EXEC);

    for(int a = 0; a < MAX_MEM; a++) {
        if (cpu->X[a] == 0) break;

        fprintf(output,"    X[%d] = %d  ", a, cpu->X[a]);
        fflush(output);
    }

    fprintf(output,"\n    Prioridade: %d", cpu->prioridade);
    fflush(output);
    fprintf(output,"\n\n");
    fflush(output);
    fprintf(output,"    CONTADOR DE PROGRAMA (PC): %d\n\n", cpu->pc);
    fflush(output);
    fprintf(output, "* Lista de IDs dos processos em estado PRONTO:\n\n");
    fflush(output);
    
    for (int i = 0; i < prontos->finalFila; i++) {
        fprintf(output, "   | %d |         ", prontos->proc[i].pid);
        fflush(output);
    }
    fprintf(output, "\n");

    for (int i = 0; i < prontos->finalFila; i++) {
        fprintf(output, "PC[%d] = %d        ", i, prontos->proc[i].pc);
        fflush(output);
    }

    fprintf(output,"\n\n");
    fflush(output);
    fprintf(output, "* Lista de IDs dos processos em estado BLOQUEADO:\n\n");
    fflush(output);
    
    for (int i = 0; i < bloqueados->finalFila;i++) {
        fprintf(output, "   | %d | ", bloqueados->proc[i].pid);
        //fflush(output);
    }
    fprintf(output, "\n");
    
    for (int i = 0; i < bloqueados->finalFila;i++) {
        fprintf(output, "PC[%d] = %d        ",i,bloqueados->proc[i].pc);
        fflush(output);
    }
    fprintf(output,"\n\n");
    fflush(output);

    fclose(output);
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
    
}

void gerenciador_processos(int file_descriptor) {

    struct timespec start_r;
    struct timespec end_r;
    struct timespec pristart_r; //Para o escalonamento com prioridades, ver tabela de tempo em definitions.h
    struct timespec priend_r;

    char rx[MAX_MEM];
    char file_name[MAX_MEM];
    bool flag = false;
    char state, command;
    int process_counter = 0;
    Processos tabela[MAX_MEM];
    CPU cpu;
    Fila PRONTOS;
    Fila BLOQUEADOS;

    read(file_descriptor,rx,sizeof(rx));
    cpu.pc = 0;
    cpu.pid = 0;
    cpu.valor = 0;
    cpu.prioridade = PRIORI_0;
    tabela[cpu.pid] = criar_processo(process_counter+1, -1, cpu.pc, cpu.valor, cpu.prioridade, "init.txt");
    
    //Inicializa as Filas
    cria_fila(&PRONTOS);
    cria_fila(&BLOQUEADOS);

    cpu.EXEC=cpu.pid;
    //strcpy(cpu.programa,tabela[cpu.pid].programa)

    clock_gettime(CLOCK_REALTIME, &start_r);
    clock_gettime(CLOCK_REALTIME, &end_r);
    clock_gettime(CLOCK_REALTIME, &pristart_r);
    clock_gettime(CLOCK_REALTIME, &priend_r);
    for (int counter = 0; counter<strlen(rx); counter++) {
        command = rx[counter];
        if (flag == true) {
            flag=false;
            command = 'L';
        }
        switch (command) {
            case 'U':
                state = tabela[cpu.pid].programa[cpu.pc][0];
                if (tabela[cpu.pid].prioridade == PRIORI_0) {
                    if (time_diff(&pristart_r, &priend_r)*1000>10) {
                        tabela[cpu.pid].prioridade++;
                    }
                }
                else if (tabela[cpu.pid].prioridade == PRIORI_1) {
                    if (time_diff(&pristart_r, &priend_r)*1000>15) {
                        tabela[cpu.pid].prioridade++;
                    }
                }
                else if (tabela[cpu.pid].prioridade == PRIORI_2) {
                    if (time_diff(&pristart_r, &priend_r)*1000>25) {
                        tabela[cpu.pid].prioridade++;
                    }
                }
                else if (tabela[cpu.pid].prioridade == PRIORI_3) {
                    if (time_diff(&start_r, &end_r)*1000>50) { //Se o processo demorar mais que 50ms
                        flag = true;
                        state = 'B';
                    }
                }
            
                switch(state) {
                    case 'N':
                        sscanf(tabela[cpu.pid].programa[cpu.pc],"%*[^0123456789]%d",&cpu.n);
                        break;
                    case 'D':
                        for(int x=0;x<cpu.n;x++) {
                            cpu.X[x] = 0;
                        }
                        break;
                    case 'V':
                        sscanf(tabela[cpu.pid].programa[cpu.pc],"%*[^0123456789]%d%*[^0123456789]%d",&cpu.indice, &cpu.valor);
                        cpu.X[cpu.indice] = cpu.valor;
                        break;
                    case 'A':
                        sscanf(tabela[cpu.pid].programa[cpu.pc],"%*[^0123456789]%d%*[^0123456789]%d",&cpu.indice, &cpu.valor);
                        cpu.X[cpu.indice]+=cpu.valor;
                        break;
                    case 'S':
                        sscanf(tabela[cpu.pid].programa[cpu.pc],"%*[^0123456789]%d%*[^0123456789]%d",&cpu.indice, &cpu.valor);
                        cpu.X[cpu.indice]-=cpu.valor;
                        break;
                    case 'B':
                        if (tabela[cpu.pid].prioridade != PRIORI_0) {
                            tabela[cpu.pid].prioridade--;
                        }
                        clock_gettime(CLOCK_REALTIME, &start_r);
                        clock_gettime(CLOCK_REALTIME, &pristart_r);
                        if (!isVazia(PRONTOS)) {
                        	enfileirar(&BLOQUEADOS, tabela[cpu.pid]);

				cpu.pid++;

				desinfileira(&PRONTOS, &tabela[cpu.pid]);
				
				cpu.EXEC = tabela[cpu.pid].pid;
				cpu.prioridade = tabela[cpu.pid].prioridade;


                        }
                        break;
                    case 'T':
                        if (!isVazia(PRONTOS)) {
                        	desinfileira(&PRONTOS, &tabela[cpu.pid]);
				cpu.EXEC = tabela[cpu.pid].pid;
				cpu.prioridade = tabela[cpu.pid].prioridade;
                        }

                        break;
		case 'F':
	                process_counter++;
	                printf("%d\n",process_counter);
	                transfere_tabela(&cpu, &tabela[cpu.pid]);
	                tabela[process_counter]  = duplica_processo(&tabela[cpu.pid], process_counter);

			enfileirar(&PRONTOS, tabela[process_counter]);

                        break;
                    case 'R':
                        
                        strncpy(file_name,&tabela[cpu.pid].programa[cpu.pc][2], MAX_MEM - 1);
                        cpu.pc = 0;
                        cpu.valor = 0;
                        strcpy(tabela[cpu.pid].nome_arquivo, file_name);
                        ler_programa(file_name, tabela[cpu.pid].programa);
                        break;
                    case '\0':
                        break;
                }
                clock_gettime(CLOCK_REALTIME, &priend_r);
                clock_gettime(CLOCK_REALTIME, &end_r);
                cpu.pc++;
                
                break;

            case 'L':

                if (isVazia(BLOQUEADOS)) {
                    break;
                }
                
                Processos procaux;
                desinfileira(&BLOQUEADOS, &procaux);
                enfileirar(&PRONTOS, procaux);

                break;
            case 'I':
                fflush(stdout);
                processo_impressao(&cpu, &PRONTOS, &BLOQUEADOS, tabela);
                break;

            case 'M':
                printf("Saindo do simulador\n");
                exit(0);
                break;

            default:
                break;
            
        }
        //cpu.pc++;
    }
}
