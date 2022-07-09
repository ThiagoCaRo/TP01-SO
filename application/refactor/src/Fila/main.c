#include <stdio.h>
#include <stdlib.h>
#include "Fila.h"

int main(){
	Fila fila;
	
	Processos proc1;
	proc1.pid = 0;
	Processos proc2;
	proc2.pid = 1;
	Processos proc3;
	proc3.pid = 2;
	Processos proc4;
	proc4.pid = 3;
	
	cria_fila(&fila);
	enfileirar(&fila, proc1);
	enfileirar(&fila, proc2);
	desinfileira(&fila, &proc1);
	enfileirar(&fila, proc3);
	enfileirar(&fila, proc1);
	enfileirar(&fila, proc4);
	imprime_fila(fila);
}
