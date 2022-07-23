#include "../Constantes/Constantes.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../Memoria/Memoria.h"

typedef struct{
    bool naPrincipal; // booleano que determina se esta ou nao na memoria principal
    int index; // endereco na memoria(disco ou ram), caso em nenhum lugar o index é igual a -1
    int ultimoAcesso; // contador do ultimo acesso
} celulas;

typedef struct{
    celulas VIR[MAX_VIR]; // celulas de enderecamento virtual
    MEMORIA disco; // disco
    MEMORIA memoriaPrincipal; // memoria principal
    frames pageframe[MAX_MEM]; // vaos na memoria
    int acessos; // contador de acessos
}MEMORIA_VIRTUAL;

void inicializa_vaziaV(MEMORIA_VIRTUAL *memoriaVirtual);
int preenche_paginaV(MEMORIA_VIRTUAL *memoriaVirtual, int tamanho_pagina, int index_pageframe);
int FitV(MEMORIA_VIRTUAL *memoriaVirtual, int tamanho_pagina);
int removePaginaV(MEMORIA_VIRTUAL *memoriaVirtual, int tamanho_pagina, int index);
int * GetAccess(MEMORIA_VIRTUAL *memoriaVirtual, int endereco, char tipo, int * next);
void LiberaRam(MEMORIA_VIRTUAL * memoriaVirtual, char tipo, int *next);
