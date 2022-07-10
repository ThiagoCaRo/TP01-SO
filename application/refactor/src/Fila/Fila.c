#include "Fila.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int cria_fila(Fila* fila){
	fila->finalFila = 0;
	return 0;
}

int enfileirar(Fila* fila, Processos proc){
	copiar_proc(&(fila->proc[fila->finalFila]), proc);
	fila->finalFila++;
	return 0;
}

int desinfileira(Fila* fila, Processos* proc){
	copiar_proc(proc, fila->proc[0]);
	for(int x=0; x<fila->finalFila-1;x++) { //Desloca todos os prontos
		copiar_proc(&(fila->proc[x]), fila->proc[x+1]);
	}
	fila->finalFila--;
	return 0;
}

void imprime_fila(Fila fila){
	for(int x = 0; x < fila.finalFila;x++){
		printf("Posicao[%d]\n",x);
		printf("PID: %d\n",fila.proc[x].pid);
	}
}

int isVazia(Fila fila){
	return fila.finalFila == 0;
}

