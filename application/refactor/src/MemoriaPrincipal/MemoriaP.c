#include "MemoriaP.h"

void inicializa_vazia(MEMORIA_PRINCIPAL *memoriaPrincipal){
    memoriaPrincipal->pageframe[1].index = MAX_MEM;
    memoriaPrincipal->pageframe[0].tamanho = MAX_MEM;
    memoriaPrincipal->pageframe[0].index = 0;

}

int preenche_pagina(int tamanho_pagina){

}

void FirstFit(MEMORIA_PRINCIPAL *memoriaPrincipal, int tamanho_pagina){
    char op = 'A';
    for(int i = 0; i<MAX_MEM; i++){
        if(tamanho_pagina < memoriaPrincipal->pageframe[i].tamanho){

        }
        
    }

}
