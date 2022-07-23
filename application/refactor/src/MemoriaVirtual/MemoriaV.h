#include "../Constantes/Constantes.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../Memoria/Memoria.h"

typedef struct{
    bool naPrincipal;
    int index;
    int ultimoAcesso;
} celulas;

typedef struct{
    celulas VIR[MAX_VIR];
    MEMORIA disco;
    MEMORIA memoriaPrincipal;
    frames pageframe[MAX_MEM];
    int acessos;
}MEMORIA_VIRTUAL;

void inicializa_vaziaV(MEMORIA_VIRTUAL *memoriaVirtual);
int preenche_paginaV(MEMORIA_VIRTUAL *memoriaVirtual, int tamanho_pagina, int index_pageframe);
int FitV(MEMORIA_VIRTUAL *memoriaVirtual, int tamanho_pagina);
int removePaginaV(MEMORIA_VIRTUAL *memoriaVirtual, int tamanho_pagina, int index);
int * GetAccess(MEMORIA_VIRTUAL *memoriaVirtual, int endereco, char tipo, int * next);
void LiberaRam(MEMORIA_VIRTUAL * memoriaVirtual, char tipo, int *next);
