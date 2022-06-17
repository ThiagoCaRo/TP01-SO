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

char commands = 'O';
char state;
int j=0;
int k;
int c=0;
bool flag = false;
int thread_counter=0;
char instruction_backup[MAX_MEM];
//int mutex=1;
int s[5];
sem_t sem[MAX_MEM];
struct timespec start_r;
struct timespec end_r;

int ler_op_mestre(char *linha, FILE *file);
int ler_op(char *linha, FILE *file);
void estados(char *buffer);

typedef struct{
    double tempo;
    char programa[MAX_MEM];
    int n;
    int pc;
    int X[MAX_MEM];
    int indice,valor;
}CPU;
typedef struct{
    pthread_t TID[MAX_MEM];
    pthread_t BLOQUEADO[MAX_MEM];
    pthread_t PRONTO[MAX_MEM];
    pthread_t EXECUCAO;
}TABELA;

CPU cpu_structure;
TABELA tabela_structure;
/*void up(int *semaphore){
    *semaphore+=1;
}
void down(int *semaphore){
    *semaphore-=1;
}*/

double time_diff(struct timespec *start, struct timespec *end){

    //Função para calcular o tempo em segundos para operações como optime e timeout

    return (end->tv_sec - start->tv_sec) + 1e-9*(end->tv_nsec - start->tv_nsec);
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

void *threads(void *arg0){
    flag=true;
    sem_wait(&sem[c]);
    /*tabela_structure.BLOQUEADO[c]=-1;
    tabela_structure.EXECUCAO=tabela_structure.BLOQUEADO[c];*/
    cpu_structure.pc=0;
    k=0;
    for(int i=0;i<MAX_MEM;i++){
        cpu_structure.programa[i]='-';
    }
    clock_gettime(CLOCK_REALTIME, &start_r);
    clock_gettime(CLOCK_REALTIME, &end_r);
    //int n;
    FILE *fp;
    char nome_arq[61];
    char buffer[MAX_MEM];
    char temp[MAX_MEM];
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

    /*while(1){
        int verify;
        if(fgets(temp, MAX_MEM, fp)!=NULL){
            //printf("VERIFY 0");
            verify=0;
        }
        else{
            verify=1;
            break;
        }
        temp[strcspn(temp, "\n")] = 0;
        cpu_structure.programa[k]=temp[0];
        k++;
    }*/
    /*clock_gettime(CLOCK_REALTIME, &end_r);
    if((time_diff(&start_r, &end_r))>=1.0){
        printf("Antes de parar novamente\n");
        sem_wait(&sem[c]);
    }*
    printf("Depois de parar novamente\n");*/
    while(1){
        if(state=='T'){
            for(int i=0;i<MAX_MEM-1;i++){
                tabela_structure.TID[i]=tabela_structure.TID[i+1];
                if(i==MAX_MEM-2){
                    tabela_structure.TID[MAX_MEM-1]=-1;
                }
                
            }
            pthread_exit(NULL);
        }
        if(ler_op(&buffer[0],fp)!=0){
            break;
        }
    }
    
    fclose(fp);
    
    /*if(c==2){
        exit(1);
    }*/
    //char op;
    


}



void estados(char *buffer){
    /*int n;
    int X[MAX_MEM];
    int indice,valor;*/

    state=buffer[0];
    switch (state){
        case 'N':
            int i;
            sscanf(buffer,"%*[^0123456789]%d",&cpu_structure.n);
            flag=true;
            sem_wait(&sem[thread_counter]);
            break;
        case 'D':
            for(i=0;i<cpu_structure.n;i++){
                cpu_structure.X[i]=0;
            }
            for(i=0;i>cpu_structure.n && i<MAX_MEM;i++){
                cpu_structure.X[i]=-1;
            }
            flag=true;
            sem_wait(&sem[thread_counter]);
            break;

        case 'V':
            sscanf(buffer,"%*[^0123456789]%d%*[^0123456789]%d",&cpu_structure.indice, &cpu_structure.valor);
            cpu_structure.X[cpu_structure.indice]=cpu_structure.valor;
            flag=true;
            sem_wait(&sem[thread_counter]);
            break;

        case 'A':
            sscanf(buffer,"%*[^0123456789]%d%*[^0123456789]%d",&cpu_structure.indice, &cpu_structure.valor);
            cpu_structure.X[cpu_structure.indice]+=cpu_structure.valor;
            //printf("VALOR :%d\n\n",X[indice]);
            flag=true;
            sem_wait(&sem[thread_counter]);
            break;   
    
        case 'S':
            sscanf(buffer,"%*[^0123456789]%d%*[^0123456789]%d",&cpu_structure.indice, &cpu_structure.valor);
            cpu_structure.X[cpu_structure.indice]-=cpu_structure.valor;
            printf("VALOR :X[%d] = %d\n\n",cpu_structure.indice, cpu_structure.X[cpu_structure.indice]);
            flag=true;
            sem_wait(&sem[thread_counter]);
            break;

        case 'B':
            if(thread_counter==0 && tabela_structure.TID[thread_counter+1]!=-1){
                tabela_structure.BLOQUEADO[thread_counter]=tabela_structure.TID[thread_counter];
                tabela_structure.EXECUCAO=tabela_structure.TID[thread_counter+1];
                thread_counter++;
                printf("THREAD COUNTER :%d\n\n\n",thread_counter);
                printf("\n\n\n EXEC APOS TROCA: %lu\n\n\n",tabela_structure.EXECUCAO);
                flag=true;
                sem_wait(&sem[thread_counter-1]);
            }
            else if(thread_counter!=0){
                if(thread_counter<MAX_MEM-1 && tabela_structure.TID[thread_counter+1]!=-1){
                    tabela_structure.BLOQUEADO[thread_counter]=tabela_structure.TID[thread_counter];
                    tabela_structure.EXECUCAO=tabela_structure.TID[thread_counter+1];
                    printf("EXEC :%lu\n\n\n",tabela_structure.EXECUCAO);
                    thread_counter++;
                    flag=true;
                    sem_wait(&sem[thread_counter-1]);
                }
                else if(thread_counter==MAX_MEM-1){
                    int aux = thread_counter;
                    tabela_structure.BLOQUEADO[thread_counter]=tabela_structure.TID[thread_counter];
                    thread_counter=0;
                    tabela_structure.EXECUCAO=tabela_structure.TID[thread_counter];
                    flag=true;
                    sem_wait(&sem[aux]);
                }

            }
            //sem_timedwait(&sem[thread_counter],)
            break;

        case 'T':
            //Termina
            /*if(TID[thread_counter+1]!=-1){
                thread_counter++;
            }*/
            break;

        case 'F':
            //printf("\n\nESTADO B\n\n");
            pthread_t           threadsim;    //Parâmetros da thread main
            pthread_attr_t      attrsim;
            int                 retcsim;
            pthread_attr_init(&attrsim);
            retcsim |= pthread_attr_setdetachstate(&attrsim, PTHREAD_CREATE_DETACHED);
            retcsim |= pthread_attr_setstacksize(&attrsim, THREADSTACKSIZE);
            retcsim = pthread_create(&threadsim, &attrsim, &threads, NULL);
            c++;
            tabela_structure.TID[c]=threadsim;
            
            
            //TID[c]=threadsim;
            flag=true;
            sem_wait(&sem[thread_counter]);
            break;

        case 'R':
            break;

        default:
            state='N';
            break;
    }
}




int ler_op_mestre(char *linha, FILE *file){
    //printf("LER OP AQUI\n");
    int verify;
    if(fgets(linha, sizeof (linha), file)!=NULL){
        //printf("VERIFY 0");
        verify=0;
    }
    else{
        //printf("VERIFY 1");
        verify=1;
        return verify;
    }
    linha[strcspn(linha, "\n")] = 0;
    return verify;
}



int main(){
    for(int i=0;i<MAX_MEM;i++){
        sem_init(&sem[i], 0, 0);
    }
    
    //sem_init(&sem_inicial, 0, 0);
    FILE *f;
    int op;
    int descriptor[2];
    pid_t pid;

    if (pipe(descriptor) == -1) {
        exit(1);
    }
    pid = fork();
    
    if(pid < 0){
        exit(1);
    }

    if(pid == 0){
            char rx[MAX_MEM];
            int counter=0;
            close(descriptor[1]);
            for(int i=0;i<MAX_MEM;i++){
                tabela_structure.TID[i]=-1;
            }
            //printf("GERENCIADOR\n");
            pthread_t           thread;    //Parâmetros da thread main
            pthread_attr_t      attrs;
            int                 retc;
            pthread_attr_init(&attrs);
            retc |= pthread_attr_setdetachstate(&attrs, PTHREAD_CREATE_DETACHED);
            retc |= pthread_attr_setstacksize(&attrs, THREADSTACKSIZE);
            retc = pthread_create(&thread, &attrs, &threads, NULL);
            tabela_structure.TID[0]=thread;
            tabela_structure.BLOQUEADO[0]=thread;
            read(descriptor[0], rx, sizeof(rx));
            /*printf("RX : %s\n\n",rx);
            printf("TAMANHO RX: %lu\n\n",strlen(rx));*/
            
            while(counter<strlen(rx)){
                commands=rx[counter];
                while(1){
                    if(flag==true){
                        flag=false;
                        break;
                    }
                }
                //printf("COMANDO: %c\n\n",commands);
                switch (commands){
                    case 'S':
                        //printf("S\n");
                        //Apenas para liberar a execução inicial
                        sem_post(&sem[thread_counter]);
                        break; 
                    case 'U':
                        printf("\nSEM[%d]\n",thread_counter);
                        sem_post(&sem[thread_counter]);
                        
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
            }
            

    }

    else{
        char tx[MAX_MEM];
        char temp[2];
        f=fopen("arquivo_mestre.txt","r");
        close(descriptor[0]);
        printf("0 - Ler do arquivo           1 - Ler por entrada padrao\n\n");
        scanf("%d",&op);
        if(op==0){
            int i = 0;
            while(1){
                int verify;
                if(fgets(temp, sizeof (temp), f)!=NULL){
                    //printf("VERIFY 0");
                    verify=0;
                }
                else{
                    //printf("VERIFY 1");
                    verify=1;
                    break;
                }
                temp[strcspn(temp, "\n")] = 0;
                strcat(tx,temp);
                i++;
                //commands=tx[0];

                
                /*switch (commands){
                    case 'U':
                        printf("OLA, EU SOU UM U\n");
                        write(descriptor[1],tx,sizeof(tx));
                        break;

                    case 'L':
                        break;

                    case 'I':
                        break;

                    case 'M':
                        break;

                    default:
                        break;
                }*/

            }
            write(descriptor[1],tx,sizeof(tx));
        }
        fclose(f);
        
    }
    
    sleep(50);
    return 0;
}
