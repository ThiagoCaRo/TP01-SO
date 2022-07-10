#include "CPU/CPU.h"
#include "Fila/Fila.h"
#include "Processos/Processos.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h> 

int main(){
    int descritor[2];
    pid_t pid;
    int iStatus;

    if(pipe(descritor)==-1){
    	perror("broken pipe");
        exit(1);
    }

    pid = fork();

    if(pid<0){
    	perror("broken fork");
        exit(1);
    }

    if(pid>0){
        //Pai
        close(descritor[0]);
        processo_main(descritor[1]);
        while(1)
      	{
		 wait(&iStatus);
		 if(WIFEXITED(iStatus)) break;
         }
    }

    if (pid==0){
        close(descritor[1]);
        gerenciador_processos(descritor[0]);
    }
}
