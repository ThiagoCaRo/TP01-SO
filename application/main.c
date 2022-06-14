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
            pthread_t           thread;    //Parâmetros da thread main
            pthread_attr_t      attrs;
            int                 retc;
            pthread_attr_init(&attrs);
            retc |= pthread_attr_setdetachstate(&attrs, PTHREAD_CREATE_DETACHED);
            retc |= pthread_attr_setstacksize(&attrs, THREADSTACKSIZE);
            retc = pthread_create(&thread, &attrs, &threads, NULL);
            
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
    sleep(50);
    return 0;
}
