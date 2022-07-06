#include "CPU/CPU.h"
#include "Fila/Fila.h"
#include "Processos/Processos.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
