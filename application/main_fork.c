#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdint.h>
#include <pthread.h>
#include <semaphore.h>
#include <inttypes.h>
#include <signal.h>
#include <stdbool.h>
#define THREADSTACKSIZE    1024
#define true 1
#define false 0
#define MAX_MEM    100


sem_t sem[MAX_MEM];

int process_counter = 0;
int k = 0;
typedef struct{
    double tempo;
    char programa[MAX_MEM];
    int n;
    int pc;
    int X[MAX_MEM];
    int indice,valor;
}CPU;

typedef struct{
    pid_t PID[MAX_MEM];
    pthread_t BLOQUEADO[MAX_MEM];
    pthread_t PRONTO[MAX_MEM];
    pthread_t EXECUCAO;
}TABELA;

CPU cpu_structure;
TABELA tabela_structure;

char commands,state;
bool flag = false;
int *c_aux;
int temporario;

pthread_t *create_thread(pthread_t *thread0, int *c);
void simulados();
int ler_op(char *linha, FILE *file);
void estados(char *buffer);

pthread_t *create_thread(pthread_t *thread0, int *c){
    //Parâmetros da thread main
    /*pthread_attr_t      attrsim;
    int                 retcsim;
    pthread_attr_init(&attrsim);
    retcsim |= pthread_attr_setdetachstate(&attrsim, PTHREAD_CREATE_DETACHED);
    retcsim = pthread_create(thread0, &attrsim, &threads, NULL);
    if(*c!=0){
        *c++;
    }
    tabela_structure.TID[*c]=*thread0;
    tabela_structure.PRONTO[*c]=*thread0;
    for(int i=0;i<MAX_MEM;i++){
        cpu_structure.programa[i]='-';
    }*/
    return thread0;
}

int ler_op(char *linha, FILE *file){
    int verify;
    if(fgets(linha, MAX_MEM, file)!=NULL){
        //printf("VERIFY 0");
        verify=0;
    }
    else{
        //printf("VERIFY 1");
        verify=1;
        return verify;
    }
    linha[strcspn(linha, "\n")] = 0;
    cpu_structure.programa[k]=linha[0];
    k++;
    estados(linha);
    return verify;
}

void estados(char *buffer){
    //printf("MAQUINA DE ESTADOS DA THREAD %d\n\n",thread_counter);
    state=buffer[0];
    switch (state){
        case 'N':
            int i;
            sscanf(buffer,"%*[^0123456789]%d",&cpu_structure.n);
            flag=true;
            sem_wait(&sem[process_counter]);
            break;
        case 'D':
            for(i=0;i<cpu_structure.n;i++){
                cpu_structure.X[i]=0;
            }
            for(i=0;i>cpu_structure.n && i<MAX_MEM;i++){
                cpu_structure.X[i]=-1;
            }
            flag=true;
            sem_wait(&sem[process_counter]);
            break;

        case 'V':
            sscanf(buffer,"%*[^0123456789]%d%*[^0123456789]%d",&cpu_structure.indice, &cpu_structure.valor);
            cpu_structure.X[cpu_structure.indice]=cpu_structure.valor;
            flag=true;
            sem_wait(&sem[process_counter]);
            break;

        case 'A':
            sscanf(buffer,"%*[^0123456789]%d%*[^0123456789]%d",&cpu_structure.indice, &cpu_structure.valor);
            cpu_structure.X[cpu_structure.indice]+=cpu_structure.valor;
            flag=true;
            sem_wait(&sem[process_counter]);
            break;   
    
        case 'S':
            sscanf(buffer,"%*[^0123456789]%d%*[^0123456789]%d",&cpu_structure.indice, &cpu_structure.valor);
            cpu_structure.X[cpu_structure.indice]-=cpu_structure.valor;
            flag=true;
            sem_wait(&sem[process_counter]);
            break;

        case 'B':
            if(process_counter==0 && tabela_structure.PRONTO[process_counter+1]!=-1){
                tabela_structure.BLOQUEADO[process_counter]=tabela_structure.EXECUCAO;
                tabela_structure.EXECUCAO=tabela_structure.PRONTO[process_counter+1];
                process_counter++;
                flag=true;
                sem_wait(&sem[process_counter-1]);
            }
            else if(process_counter!=0 && tabela_structure.PRONTO[process_counter+1]!=-1){
                tabela_structure.BLOQUEADO[process_counter]=tabela_structure.EXECUCAO;
                tabela_structure.EXECUCAO=tabela_structure.PRONTO[process_counter+1];
                process_counter++;
                flag=true;
                sem_wait(&sem[process_counter-1]);
            }
            else{
                printf("E R R O\n\n\n");
            }
            /*else if(thread_counter!=0){
                if(thread_counter<MAX_MEM-1 && tabela_structure.PRONTO[thread_counter+1]!=-1){
                    tabela_structure.BLOQUEADO[thread_counter]=tabela_structure.EXECUCAO;
                    tabela_structure.EXECUCAO=tabela_structure.PRONTO[thread_counter+1];
                    thread_counter++;
                    flag=true;
                    sem_wait(&sem[thread_counter-1]);
                }
                else if(thread_counter==MAX_MEM-1){
                    int aux = thread_counter;
                    tabela_structure.BLOQUEADO[thread_counter]=tabela_structure.EXECUCAO;
                    thread_counter=0;
                    tabela_structure.EXECUCAO=tabela_structure.PRONTO[thread_counter];
                    flag=true;
                    sem_wait(&sem[aux]);
                }
                else{
                    printf("ERRO INTERNO\n");
                }

            }
            else{
                printf("ERRO\n");
            }*/
            //sem_timedwait(&sem[thread_counter],)
            break;

        case 'T':
            //Termina
            /*if(TID[thread_counter+1]!=-1){
                thread_counter++;
            }*/
            break;

        case 'F':
            
            
            
        
            flag=true;
            sem_wait(&sem[process_counter]);
            break;

        case 'R':
            break;

        default:
            state='N';
            break;
    }
}

void simulados(){
    flag = true;
    sem_wait(&sem[*c_aux]);
    printf("ALO\n");
    tabela_structure.EXECUCAO=tabela_structure.PRONTO[*c_aux];
    FILE *fp;
    char nome_arq[61];
    char buffer[MAX_MEM];
    char temp[MAX_MEM];
    if(*c_aux==0){
        while(fp==NULL){
            fp=fopen("init.txt","r");
            if(fp!=NULL){
                break;
            }
            printf("Problema ao abrir o arquivo\n");
        }
    }

    else{
        printf("Entre com o arquivo de comando da Thread\n");
        //sem_wait(&sem[0]);
        while(fp==NULL){
            printf("\n");
            scanf("%s",nome_arq);
            fp=fopen(nome_arq,"r");
            if(fp!=NULL){
                break;
            }
            printf("Problema ao abrir o arquivo\n");
        }
    }

    while(1){
        if(state=='T'){
            for(int i=0;i<MAX_MEM-1;i++){
                tabela_structure.PID[i]=tabela_structure.PID[i+1];
                if(i==MAX_MEM-2){
                    tabela_structure.PID[MAX_MEM-1]=-1;
                }
                
            }
            exit(0);
        }
        if(ler_op(&buffer[0],fp)!=0){
            break;
        }
    }
    fclose(fp);
}

int main(){
    bool ctrl=false;
    for(int i=0;i<MAX_MEM;i++){
        sem_init(&sem[i], 1, 0);
    }
    
    //sem_init(&sem_inicial, 0, 0);
    FILE *f;
    int op;
    int descriptor[2];
    pid_t pid;
    pthread_t thread;
    int counter = 0;
    if (pipe(descriptor) == -1) {
        exit(1);
    }
    pid = fork();
    
    if(pid < 0){
        exit(1);
    }

    if(pid>0){
        printf("PAI\n");
        char tx[MAX_MEM];
        char temp[2];
        kill(pid, SIGSTOP);
        f = fopen("arquivo_mestre.txt", "r");
        close(descriptor[0]);
        printf("0 - Ler do arquivo           1 - Ler por entrada padrao\n\n");
        scanf("%d",&op);
        if(op==0){
            while(1){
                if(fgets(temp, sizeof(temp), f) == NULL){
                    break;
                }
                temp[strcspn(temp, "\n")] = 0;
                strcat(tx,temp);
            }
            kill(pid, SIGCONT);
            write(descriptor[1],tx,sizeof(tx));
        }
        fclose(f);
    }

    else{
        pid_t pidsim0;
        /*if(c==0 || state == 'F'){
            printf("NAO FEITO\n");
        }*/
        pidsim0=fork();

        if(pidsim0<0){
            exit(1);
        }
        if(pidsim0>0){
            printf("FILHO DO PAI\n");
            int c = 0;
            c_aux=&c;
            kill(pidsim0 , SIGSTOP);
            char rx[MAX_MEM];
            close(descriptor[1]);
            for(int i=0;i<MAX_MEM;i++){
                tabela_structure.PID[i]=-1;
                tabela_structure.BLOQUEADO[i]=-1;
                tabela_structure.PRONTO[i]=-1;
                
            }
            read(descriptor[0], rx, sizeof(rx));
            kill(pidsim0, SIGCONT);
            printf("RX: %s\n",rx);
            if(flag==true){
                flag=false;
                sleep(5);
                kill(pidsim0, SIGCONT);
            }
            printf("ue\n");
            //create_thread(&thread, c_aux);
            /*if(fork()==0){
                int contador=0;
                tabela_structure.TID[*c_aux]=getpid();
                tabela_structure.PRONTO[*c_aux]=tabela_structure.TID[*c_aux];
                cpu_structure.pc=0;
                simulados();
                printf("X[0] = %d e X[1] = %d\n\n",cpu_structure.X[0],cpu_structure.X[1]);
            }*/
            /*while(counter<strlen(rx)){
                flag = false;
                kill(pidsim0, SIGSTOP);
                commands=rx[counter];
                while(1){
                    if(flag==true){
                        flag=false;
                        break;
                    }
                }
                switch (commands){
                    case 'U':
                        cpu_structure.pc++;
                        //SEMAFORE TA COM PROBLEMA
                        
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
                counter++;
            }*/
        }
        else{
            tabela_structure.PID[0] = getpid();
            tabela_structure.PRONTO[0]=tabela_structure.PID[0];
            flag = true;
            printf("Continuou\n");
        }
        
        
    }
    while(ctrl==false){

    }
    return 0;
}
