#include "CPU/CPU.h"
#include "Fila/Fila.h"
#include "Processos/Processos.h"

/*void transfere_tabela(CPU *cpu, Processos *proc){
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
}
Processos criar_processo(pid_t pid, pid_t ppid, int pc, int valor, char *nome_arquivo){
    static Processos processos;
    processos.pid = pid;
    processos.ppid = ppid;
    processos.pc = pc;
    processos.valor = valor;
    strcpy(processos.nome_arquivo, nome_arquivo);
    ler_programa(processos.nome_arquivo, processos.programa);
}


void processo_main(int file_descriptor){
    FILE *f;
    int op;
    char tx[MAX_MEM];
    char temp[2];
    f=fopen("arquivo_mestre.txt","r");
    printf("0 - Ler do arquivo           1 - Ler por entrada padrao\n\n");
    scanf("%d",&op);
    while(1){
        int verify;
        if(fgets(temp, sizeof (temp), f)==NULL){
            break;
        }
        temp[strcspn(temp, "\n")] = 0;
        strcat(tx,temp);
    }
    write(file_descriptor,tx,sizeof(tx));
    fclose(f);
    
}

void gerenciador_processos(int file_descriptor){
    char rx[MAX_MEM];
    int i,j,x,y;
    int process_counter, ready_counter;
    Processos tabela[MAX_MEM];
    CPU cpu;
    FILA PRONTOS;
    FILA BLOQUEADOS;

    read(file_descriptor,rx,sizeof(rx));
    process_counter, ready_counter = 0;
    cpu.pc = 0;
    cpu.pid = 0;
    cpu.valor = 0;

    tabela[cpu.pid] = criar_processo(process_counter++, -1, cpu.pc, cpu.valor, "init.txt");
    printf("%s\n",tabela[cpu.pid].programa[0]);
    cpu.EXEC=cpu.pid;
    //strcpy(cpu.programa,tabela[cpu.pid].programa)

    i = 0;
    j = 0;
    while(1){
        if(tabela[cpu.pid].programa[cpu.pc][0] == '\0'){
            break;
        }
        switch (rx[i]){
            case 'U':
                printf("FIM DE UMA UNIDADE DE TEMPO\n");
                cpu.pc++;
                
                break;

            case 'L':
                break;

            case 'I':
                break;

            case 'M':
                break;

            default:
                break;
        }
        i++;

        switch(tabela[cpu.pid].programa[cpu.pc][0]){
            case 'N':
                printf("N\n");
                sscanf(tabela[cpu.pid].programa[cpu.pc],"%*[^0123456789]%d",&cpu.n);
                break;
            case 'D':
                printf("D\n");
                for(x=0;x<cpu.n;x++){
                    cpu.X[x] = 0;
                }
                for(x=0;x>=cpu.n && x<MAX_MEM; x++){
                    cpu.X[i] = -1;
                }
                break;
            case 'V':
                printf("V\n");
                sscanf(tabela[cpu.pid].programa[cpu.pc],"%*[^0123456789]%d%*[^0123456789]%d",&cpu.indice, &cpu.valor);
                cpu.X[cpu.indice] = cpu.valor;
                break;
            case 'A':
                printf("A\n");
                sscanf(tabela[cpu.pid].programa[cpu.pc],"%*[^0123456789]%d%*[^0123456789]%d",&cpu.indice, &cpu.valor);
                cpu.X[cpu.indice]+=cpu.valor;
                break;
            case 'S':
                printf("S\n");
                sscanf(tabela[cpu.pid].programa[cpu.pc],"%*[^0123456789]%d%*[^0123456789]%d",&cpu.indice, &cpu.valor);
                cpu.X[cpu.indice]-=cpu.valor;
                break;
            case 'B':
                printf("B\n");
                if(PRONTOS.proc[ready_counter].pid != 0){

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
                        PRONTOS.proc[i] = PRONTOS.proc[i+1];
                        if(i==MAX_MEM-2){
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
                process_counter++;

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
        //cpu.pc++;

    }
    printf("RX: %s\n",rx);
    printf("X[0] = %d",cpu.X[0]);
}*/
int main(){
    int descritor[2];
    pid_t pid;

    if(pipe(descritor)==-1){
        exit(1);
    }

    pid = fork();

    if(pid<0){
        exit(1);
    }

    else if(pid>0){
        //Pai
        close(descritor[0]);
        processo_main(descritor[1]);
    }

    else{
        close(descritor[1]);
        gerenciador_processos(descritor[0]);
    }
}
