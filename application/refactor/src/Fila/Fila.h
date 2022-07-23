#ifndef __FILA_H__
#define __FILA_H__

#include "../Processos/Processos.h"

typedef struct Fila {
    Processos proc[MAX_MEM]; // vetor onde serao armazenados os processos
    int finalFila; // marcador de fim da fila
} Fila;

int enfileirar(Fila* fila, Processos proc);
int desinfileira(Fila* fila, Processos* proc);
int isVazia(Fila fila);
void imprime_fila(Fila fila);
int cria_fila(Fila* fila);

#endif // __FILA_H__
