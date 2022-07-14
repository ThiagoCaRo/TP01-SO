#ifndef __MEMORIAP_H__
#define __MEMORIAP_H__

#include "../Fila/Fila.h"
#define true    1
#define false    0

typedef struct{
    int tamanho;
    int index;
}frames;

typedef struct{
    int RAM[MAX_MEM];
    frames pageframe[MAX_MEM];

}MEMORIA_PRINCIPAL;

void inicializa_vazia(MEMORIA_PRINCIPAL *memoriaPrincipal);
int preenche_pagina(MEMORIA_PRINCIPAL *memoriaPrincipal, int tamanho_pagina, int index_pageframe, char operador);
int FirstFit(MEMORIA_PRINCIPAL *memoriaPrincipal, int tamanho_pagina);
int removePagina(MEMORIA_PRINCIPAL *memoriaPrincipal, int tamanho_pagina, int index);

#endif // __MEMORIAP_H__
