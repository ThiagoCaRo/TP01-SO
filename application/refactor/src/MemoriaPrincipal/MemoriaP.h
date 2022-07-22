#include <stdbool.h>
#include "../Constantes/Constantes.h"

typedef struct{
    int tamanho;
    int index;
}frames;

typedef struct{
    int RAM[MAX_RAM];
    frames pageframe[MAX_MEM];

}MEMORIA_PRINCIPAL;

void inicializa_vazia(MEMORIA_PRINCIPAL *memoriaPrincipal);
int preenche_pagina(MEMORIA_PRINCIPAL *memoriaPrincipal, int tamanho_pagina, int index_pageframe);
int FirstFit(MEMORIA_PRINCIPAL *memoriaPrincipal, int tamanho_pagina);
int NextFit(MEMORIA_PRINCIPAL *memoriaPrincipal, int tamanho_pagina, int * next);
int WorstFit(MEMORIA_PRINCIPAL *memoriaPrincipal, int tamanho_pagina);
int BestFit(MEMORIA_PRINCIPAL *memoriaPrincipal, int tamanho_pagina);
int Fit(MEMORIA_PRINCIPAL *memoriaPrincipal, int tamanho_pagina, char tipo, int * next);
int removePagina(MEMORIA_PRINCIPAL *memoriaPrincipal, int tamanho_pagina, int index);
