#include "definitions.h"


double time_diff(struct timespec *start, struct timespec *end){

    //Função para calcular o tempo decorrido para round robin facilitado

    return (end->tv_sec - start->tv_sec) + 1e-9*(end->tv_nsec - start->tv_nsec);
}

void transfere_tabela(CPU *cpu, Processos *proc){
    proc->pc = cpu->pc;
    proc->pid = cpu->pid;
    proc->valor = cpu->valor;
    //proc->t_used = cpu->t_remain;
}

void transfere_cpu(Processos *proc, CPU *cpu){
    cpu->pc = proc->pc;
    cpu->pid = proc->pid;
    cpu->valor = proc->valor;
}

int ler_programa(char *file, char programa[MAX_MEM][MAX_LINHA]){
    char buffer[MAX_LINHA];
    int i=0;
    FILE *fp=fopen(file, "r");

    if(fp==NULL){
        exit(1);
    }

    for(int x=0;x<MAX_LINHA;x++){
        for(int y=0;y<MAX_MEM;y++){
            programa[x][y] = '\0';
        }
    }

    while(1){
        if(fgets(buffer, MAX_LINHA, fp)==NULL){
        //printf("VERIFY 0");
        break;
        }
        buffer[strcspn(buffer, "\n")] = 0;
        strcpy(programa[i],buffer);
        i++;
    }
    fclose(fp);
    return(0);

}

Processos duplica_processo(Processos *proc, pid_t novo){
    static Processos duplicata;
    duplicata.pid = novo;
    duplicata.ppid = proc->pid;
    duplicata.pc = proc->pc;
    duplicata.valor = proc->valor;
    strcpy(duplicata.nome_arquivo, proc->nome_arquivo);
    ler_programa(duplicata.nome_arquivo, duplicata.programa);
    return duplicata;
}
Processos criar_processo(pid_t pid, pid_t ppid, int pc, int valor, char *nome_arquivo){
    static Processos processos;
    processos.pid = pid;
    processos.ppid = ppid;
    processos.pc = pc;
    processos.valor = valor;
    strcpy(processos.nome_arquivo, nome_arquivo);
    ler_programa(processos.nome_arquivo, processos.programa);
    return processos;
}

void processo_impressao(CPU *cpu, FILA *prontos, FILA *bloqueados, Processos tabela[MAX_MEM]){
    printf("chegou\n");
    FILE *output;
    int i;
    output = fopen("output.txt", "w");
    if(output == NULL){
        printf("Failure\n");
        exit(1);
    }
    fprintf(output,"               SAIDA DO SIMULADOR:\n\n\n");
    fflush(output);
    fprintf(output,"* Processo atualmente em execucao na CPU:\n\n");
    fflush(output);
    fprintf(output,"                PID: %d\n",cpu->EXEC);
    for(int a=0;a<MAX_MEM;a++){
        if(cpu->X[a]==0){
            break;
        }
        fprintf(output,"    X[%d] = %d  ",a,cpu->X[a]);
        fflush(output);
    }
    fprintf(output,"\n\n");
    fflush(output);
    fprintf(output,"    CONTADOR DE PROGRAMA (PC): %d\n\n",cpu->pc);
    fflush(output);
    fprintf(output, "* Lista de IDs dos processos em estado PRONTO:\n\n");
    fflush(output);
    i=0;
    while(i<MAX_MEM){
        if(prontos->proc[i].pid == -1){
            break;
        }
        fprintf(output, "   | %d |         ", prontos->proc[i].pid);
        fflush(output);
        i++;
    }
    i=0;
    fprintf(output, "\n");
    while(i<MAX_MEM){
        if(prontos->proc[i].pid == -1){
            break;
        }
        fprintf(output, "PC[%d] = %d        ",i,prontos->proc[i].pc);
        fflush(output);
        i++;
    }
    fprintf(output,"\n\n");
    fflush(output);
    fprintf(output, "* Lista de IDs dos processos em estado BLOQUEADO:\n\n");
    fflush(output);
    i=0;
    while(i<MAX_MEM){
        if(bloqueados->proc[i].pid == -1){
            break;
        }
        fprintf(output, "   | %d | ", bloqueados->proc[i].pid);
        i++;
        //fflush(output);
    }
    fprintf(output, "\n");
    i=0;
    while(i<MAX_MEM){
        if(bloqueados->proc[i].pid == -1){
            break;
        }
        fprintf(output, "PC[%d] = %d        ",i,bloqueados->proc[i].pc);
        fflush(output);
        i++;
    }
    fprintf(output,"\n\n");
    fflush(output);

    fclose(output);
    return;
    
}
void processo_main(int file_descriptor){
    FILE *f;
    int op;
    char tx[MAX_MEM];
    char temp[2];
    f=fopen("arquivo_mestre.txt","r");
    printf("0 - Ler do arquivo           1 - Ler por entrada padrao\n\n");
    scanf("%d",&op);
    if(op == 0){
        while(1){
            if(fgets(temp, sizeof (temp), f)==NULL){
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
    
    write(file_descriptor,tx,sizeof(tx));
    fclose(f);
    
}

void gerenciador_processos(int file_descriptor){
    struct timespec start_r;
    struct timespec end_r;
    char rx[MAX_MEM];
    bool flag = false;
    char state, command;
    int i,j,x,y;
    int counter = 0;
    int process_counter, ready_counter;
    Processos tabela[MAX_MEM];
    CPU cpu;
    FILA PRONTOS;
    FILA BLOQUEADOS;

    read(file_descriptor,rx,sizeof(rx));
    printf("RX: %s\n\n",rx);
    process_counter, ready_counter = 0;
    cpu.pc = 0;
    cpu.pid = 0;
    cpu.valor = 0;
    int deb_a,deb_b=0;
    tabela[cpu.pid] = criar_processo(process_counter+1, -1, cpu.pc, cpu.valor, "init.txt");
    

    //Inicializa as filas
    for(int b = 0; b<MAX_MEM; b++){
        PRONTOS.proc[b].pid = -1;
        PRONTOS.proc[b].ppid = -1;
        PRONTOS.proc[b].valor = 0;
        strcpy(PRONTOS.proc[b].nome_arquivo, "");

        BLOQUEADOS.proc[b].pid = -1;
        BLOQUEADOS.proc[b].ppid = -1;
        BLOQUEADOS.proc[b].valor = 0;
        strcpy(BLOQUEADOS.proc[b].nome_arquivo, "");
    }

    cpu.EXEC=cpu.pid;
    printf("EM EXECUCAO ANTES: %d\n",cpu.EXEC);
    //strcpy(cpu.programa,tabela[cpu.pid].programa)

    i = 0;
    j = 0;
    clock_gettime(CLOCK_REALTIME, &start_r);
    while(counter<strlen(rx)){
        /*if(tabela[cpu.pid].programa[cpu.pc][0] == '\0' && command == 'U'){
            printf("saiu antes\n");
            break;
        }*/
        command = rx[counter];
        if(flag==true){
            flag=false;
            command = 'L';
        }
        switch (command){
            case 'U':
                state = tabela[cpu.pid].programa[cpu.pc][0];
                if(time_diff(&start_r, &end_r)*1000>50){ //Se o processo demorar mais que 900ms
                    flag = true;
                    state = 'B';
                }
                switch(state){
                    case 'N':
                        printf("\nN\n");
                        sscanf(tabela[cpu.pid].programa[cpu.pc],"%*[^0123456789]%d",&cpu.n);
                        break;
                    case 'D':
                        printf("\nD\n");
                        for(x=0;x<cpu.n;x++){
                            cpu.X[x] = 0;
                        }
                        for(x=0;x>=cpu.n && x<MAX_MEM; x++){
                            cpu.X[i] = -1;
                        }
                        break;
                    case 'V':
                        printf("\nV\n");
                        sscanf(tabela[cpu.pid].programa[cpu.pc],"%*[^0123456789]%d%*[^0123456789]%d",&cpu.indice, &cpu.valor);
                        cpu.X[cpu.indice] = cpu.valor;
                        break;
                    case 'A':
                        printf("\nA\n");
                        sscanf(tabela[cpu.pid].programa[cpu.pc],"%*[^0123456789]%d%*[^0123456789]%d",&cpu.indice, &cpu.valor);
                        cpu.X[cpu.indice]+=cpu.valor;
                        break;
                    case 'S':
                        printf("\nS\n");
                        sscanf(tabela[cpu.pid].programa[cpu.pc],"%*[^0123456789]%d%*[^0123456789]%d",&cpu.indice, &cpu.valor);
                        cpu.X[cpu.indice]-=cpu.valor;
                        break;
                    case 'B':
                        printf("\nB\n");
                        clock_gettime(CLOCK_REALTIME, &start_r);
                        if(PRONTOS.proc[ready_counter].pid != -1){
                            strcpy(BLOQUEADOS.proc[cpu.pid].nome_arquivo , tabela[cpu.pid].nome_arquivo);
                            BLOQUEADOS.proc[cpu.pid].pc = tabela[cpu.pid].pc;
                            BLOQUEADOS.proc[cpu.pid].pid = tabela[cpu.pid].pid;
                            BLOQUEADOS.proc[cpu.pid].ppid = tabela[cpu.pid].ppid;
                            for(int j=0;j<MAX_MEM;j++){
                                strcpy(BLOQUEADOS.proc[cpu.pid].programa[j], tabela[cpu.pid].programa[j]);
                                
                            }
                            BLOQUEADOS.proc[cpu.pid].valor = tabela[cpu.pid].valor;

                            cpu.pid++;

                            strcpy(tabela[cpu.pid].nome_arquivo, PRONTOS.proc[ready_counter].nome_arquivo);
                            tabela[cpu.pid].pc = PRONTOS.proc[ready_counter].pc;
                            tabela[cpu.pid].pid = PRONTOS.proc[ready_counter].pid;
                            tabela[cpu.pid].ppid = PRONTOS.proc[ready_counter].ppid;
                            for(int j=0;j<MAX_MEM;j++){
                                strcpy(tabela[cpu.pid].programa[j] , PRONTOS.proc[ready_counter].programa[j]);
                                
                            }
                            tabela[cpu.pid].valor = PRONTOS.proc[ready_counter].valor;
                            cpu.EXEC = tabela[cpu.pid].pid;
                            for(x=0; x<MAX_MEM-1;x++){ //Desloca todos os prontos
                                PRONTOS.proc[x] = PRONTOS.proc[x+1];
                                if(x==MAX_MEM-2){
                                    strcpy(PRONTOS.proc[MAX_MEM-1].nome_arquivo, "");
                                    PRONTOS.proc[MAX_MEM-1].pc = 0;
                                    PRONTOS.proc[MAX_MEM-1].pid = 0;
                                    PRONTOS.proc[MAX_MEM-1].ppid = 0;
                                    for(int j=0;j<MAX_MEM;j++){
                                        for(int k=0;k<MAX_LINHA;k++){
                                            PRONTOS.proc[MAX_MEM-1].programa[j][k] = '\0';
                                        }
                                    }
                                    
                                    PRONTOS.proc[MAX_MEM-1].valor = 0;

                                }
                            }

                    
                        }
                        break;
                    case 'T':
                        printf("\nT\n");
                        if(PRONTOS.proc[ready_counter].pid != 0){
                            strcpy(tabela[cpu.pid].nome_arquivo, PRONTOS.proc[ready_counter].nome_arquivo);
                            tabela[cpu.pid].pc = PRONTOS.proc[ready_counter].pc;
                            tabela[cpu.pid].pid = PRONTOS.proc[ready_counter].pid;
                            tabela[cpu.pid].ppid = PRONTOS.proc[ready_counter].ppid;
                            for(int j=0;j<MAX_MEM;j++){
                                strcpy(tabela[cpu.pid].programa[j] , PRONTOS.proc[ready_counter].programa[j]);
                                
                            }
                            tabela[cpu.pid].valor = PRONTOS.proc[ready_counter].valor;
                            cpu.EXEC = tabela[cpu.pid].pid;
                        }

                        break;
                    case 'F':
                        printf("\nF\n");
                        process_counter++;
                        transfere_tabela(&cpu, &tabela[cpu.pid]);
                        tabela[process_counter]  = duplica_processo(&tabela[cpu.pid], process_counter);

                        strcpy(PRONTOS.proc[process_counter-1].nome_arquivo , tabela[process_counter].nome_arquivo);
                        
                        PRONTOS.proc[process_counter-1].pc = tabela[process_counter].pc;
                        PRONTOS.proc[process_counter-1].pid = tabela[process_counter].pid;
                        PRONTOS.proc[process_counter-1].ppid = tabela[process_counter].ppid;
                        for(int j=0;j<MAX_MEM;j++){
                            strcpy(PRONTOS.proc[process_counter-1].programa[j], tabela[process_counter].programa[j]);
                            
                        }
                        PRONTOS.proc[process_counter-1].valor = tabela[process_counter].valor;
                        break;
                    case 'R':
                        printf("\nR\n");
                        char file_name[MAX_MEM];
                        scanf("%s",file_name);
                        cpu.pc = 0;
                        cpu.valor = 0;
                        strcpy(tabela[cpu.pid].nome_arquivo, file_name);
                        ler_programa(file_name, tabela[cpu.pid].programa);
                        break;
                    case '\0':
                        break;
                }
                clock_gettime(CLOCK_REALTIME, &end_r);
                cpu.pc++;
                
                break;

            case 'L':

                if(BLOQUEADOS.proc[0].pid == -1){
                    break;
                }
                
                for(int b=0;b<MAX_MEM; b++){
                    if(PRONTOS.proc[b].pid == -1){
                        PRONTOS.proc[b].pid = BLOQUEADOS.proc[0].pid;
                        PRONTOS.proc[b].ppid = BLOQUEADOS.proc[0].ppid;
                        PRONTOS.proc[b].valor = BLOQUEADOS.proc[0].valor;
                        strcpy(PRONTOS.proc[b].nome_arquivo, BLOQUEADOS.proc[0].nome_arquivo);
                        ler_programa(PRONTOS.proc[b].nome_arquivo, PRONTOS.proc[b].programa);

                        for(x=0; x<MAX_MEM-1;x++){ //Desloca todos os bloqueados
                            BLOQUEADOS.proc[x] = BLOQUEADOS.proc[x+1];
                            if(x==MAX_MEM-2){
                                strcpy(BLOQUEADOS.proc[MAX_MEM-1].nome_arquivo, "");
                                BLOQUEADOS.proc[MAX_MEM-1].pc = 0;
                                BLOQUEADOS.proc[MAX_MEM-1].pid = 0;
                                BLOQUEADOS.proc[MAX_MEM-1].ppid = 0;
                                for(int j=0;j<MAX_MEM;j++){
                                    for(int k=0;k<MAX_LINHA;k++){
                                        BLOQUEADOS.proc[MAX_MEM-1].programa[j][k] = '\0';
                                    }
                                }
                                
                                BLOQUEADOS.proc[MAX_MEM-1].valor = 0;

                            }
                        }
                        break;
                    }
                }
                break;

            case 'I':
                fflush(stdout);
                processo_impressao(&cpu, &PRONTOS, &BLOQUEADOS, tabela);
                break;

            case 'M':
                break;

            default:
                break;
            
            
        }
        counter++;
        //cpu.pc++;

    }

}
