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
#define THREADSTACKSIZE    1024

char commands = 'O';
char state;
int c=0;
int mutex=1;
int s[5];
pthread_t TID[5];
sem_t sem[5];

int ler_op_mestre(char *linha, FILE *file);
int ler_op(char *linha, FILE *file);
void estados(char *buffer);
/*void up(int *semaphore){
    *semaphore+=1;
}
void down(int *semaphore){
    *semaphore-=1;
}*/

void *threadZero(void *arg0){
    //sem_wait(&sem_inicial);
    printf("\nSOU A THREAD 0\n");
    int n;
    FILE *fp;
    char nome_arq[61];
    char buffer[100];
    printf("Entre com o arquivo de comando da Thread\n\n");
    scanf("%s",nome_arq);
    fp=fopen(nome_arq,"r");
    if(fp==NULL){
        printf("Problema no f0\n");
        exit(1);
    }
    while(1){
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

void *threads(void *arg0){
    printf("ANTES DO SEMAPHORE %d\n",c);
    sem_wait(&sem[c]);
    printf("\nSOU UMA THREAD\n");
    c++;
    printf("\n\n\n\nval = %d \n\n\n",c);
    int n;
    FILE *fp;
    char nome_arq[61];
    char buffer[100];
    printf("Entre com o arquivo de comando da Thread\n\n");
    scanf("%s",nome_arq);
    fp=fopen(nome_arq,"r");
    if(fp==NULL){
        printf("Problema no f0\n");
        exit(1);
    }
    while(1){
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

int ler_op(char *linha, FILE *file){
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
    estados(&linha[0]);
    return verify;
}



void estados(char *buffer){
    int c = 0;
    int n;
    state=buffer[0];
    //printf("ESTADO: %c\n",state);
    switch (state){
        case 'D':
            int x;
            state='V';
            break;

        case 'V':
            while(c < n){
            scanf("%d",&x); //Tem que ser um vetor com x
            printf("\n");
            c++;
            }
            //ler_op(&linha[0]);
            /*c=0;
            state=buffer[0];*/
            break;

        case 'A':
            int up_offset;
            while(c < n){
                scanf("%d", &up_offset);
                x+=up_offset; //x tem que ser vetor
            }
            //ler_op(&linha[0]);
            /*c=0;
            state=buffer[0];*/
            break;   
    
        case 'S':
            int down_offset;
                while(c < n){
                    scanf("%d", &down_offset);
                    x-=down_offset; //x tem que ser vetor
                }
            //ler_op(&linha[0]);
            /*c=0;
            state=linha[0];*/
            break;

        case 'B':
            pthread_t           threadsim;    //Parâmetros da thread main
            pthread_attr_t      attrsim;
            int                 retcsim;
            pthread_attr_init(&attrsim);
            retcsim |= pthread_attr_setdetachstate(&attrsim, PTHREAD_CREATE_DETACHED);
            retcsim |= pthread_attr_setstacksize(&attrsim, THREADSTACKSIZE);
            retcsim = pthread_create(&threadsim, &attrsim, &threads, NULL);
            //TID[c]=threadsim;
            break;

        case 'T':
            //Termina
            break;

        case 'F':
            break;

        case 'R':
            break;

        default:
            state='N';
            break;
    }
}

int main(){
    for(int i=0;i<5;i++){
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
            char rx[20];
            close(descriptor[1]);
            //printf("GERENCIADOR\n");
            pthread_t           thread;    //Parâmetros da thread main
            pthread_attr_t      attrs;
            int                 retc;
            pthread_attr_init(&attrs);
            retc |= pthread_attr_setdetachstate(&attrs, PTHREAD_CREATE_DETACHED);
            retc |= pthread_attr_setstacksize(&attrs, THREADSTACKSIZE);
            retc = pthread_create(&thread, &attrs, &threads, NULL);
            read(descriptor[0], rx, sizeof(rx));

            printf("%s\n\n",rx);
            
            //TID[0]=thread;
            /*while(ler_op(&buffer[0],f[0])!=1){    
                state_v=buffer[0];
                if(state_v=='N'){
                    retc[1] = pthread_attr_setschedparam(&attrs[1], &Param[1]);
                    retc[1] |= pthread_attr_setdetachstate(&attrs[1], PTHREAD_CREATE_DETACHED);
                    retc[1] |= pthread_attr_setstacksize(&attrs[1], THREADSTACKSIZE);
                    retc[1] = pthread_create(&thread[1], &attrs[1], threads, NULL);
                    if(retc[1] == 0){
                        printf("Thread 2 aqui\n");
                    }
                }
            }*/
            
            //fclose(f[0]);
            /*pthread_cancel(thread[0]);
            pthread_cancel(thread[1]);*/


    }

    else{
        char tx[20];
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
