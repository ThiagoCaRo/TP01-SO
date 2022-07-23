#include <stdbool.h>
#include "../Constantes/Constantes.h"

typedef struct{
    int tamanho;
    int index;
}frames;

typedef struct{
    int variaveis[MAX_MEM];
    frames pageframe[MAX_MEM];
}MEMORIA;

void inicializa_vazia(MEMORIA *memoria, int tamanho);
int preenche_pagina(MEMORIA *memoria, int tamanho_pagina, int index_pageframe);
int FirstFit(MEMORIA *memoria, int tamanho_pagina);
int NextFit(MEMORIA *memoria, int tamanho_pagina, int * next);
int WorstFit(MEMORIA *memoria, int tamanho_pagina);
int BestFit(MEMORIA *memoria, int tamanho_pagina);
int Fit(MEMORIA *memoria, int tamanho_pagina, char tipo, int * next);
int removePagina(MEMORIA *memoria, int tamanho_pagina, int index);
