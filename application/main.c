#include "threads.h"
char commands = 'O';
char state_v = '_';
int main(){
    FILE *f[2];
    char buffer[100];
    pid_t pid;
    pid = fork();

    if(pid < 0){
        exit(1);
    }

    if(pid == 0){
            pthread_t           thread[2];    //Parâmetros da thread main
            pthread_attr_t      attrs[2];
            struct sched_param  Param[2];
            int                 retc[2];
            pthread_attr_init(&attrs[0]);
            retc[0] = pthread_attr_setschedparam(&attrs[0], &Param[0]);
            retc[0] |= pthread_attr_setdetachstate(&attrs[0], PTHREAD_CREATE_DETACHED);
            retc[0] |= pthread_attr_setstacksize(&attrs[0], THREADSTACKSIZE);
            retc[0] = pthread_create(&thread[0], &attrs[0], threads, NULL);
            if(retc[0] == 0){
                printf("Thread 1 aqui\n");
            }
            f[0] = fopen("teste.txt", "r");
            if(f[0]==NULL){
                printf("Problema no f0\n");
                exit(1);
            }
            while(ler_op(&buffer[0],f[0])!=1){    
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
            }
            
            fclose(f[0]);
            /*pthread_cancel(thread[0]);
            pthread_cancel(thread[1]);*/


    }

    else{
        switch (commands){
        case 'U':

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
    }

    return 0;
}
