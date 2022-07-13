#ifndef __MEMORIAP_H__
#define __MEMORIAP_H__

#include "../Fila/Fila.h"

typedef struct{
    int tamanho;
    int index;
}frames;

typedef struct{
    int RAM[MAX_MEM];
    frames page_frame[MAX_MEM];

}MEMORIA_PRINCIPAL;

void inicializa_vazia(MEMORIA_PRINCIPAL *memoriaPrincipal);
int preenche_pagina(MEMORIA_PRINCIPAL *memoriaPrincipal, int tamanho_pagina, char operador);
void FirstFit(MEMORIA_PRINCIPAL *memoriaPrincipal, int tamanho_pagina);
