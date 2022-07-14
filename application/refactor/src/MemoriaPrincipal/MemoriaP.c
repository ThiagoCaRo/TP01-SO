#include "MemoriaP.h"

void inicializa_vazia(MEMORIA_PRINCIPAL *memoriaPrincipal){
    memoriaPrincipal->pageframe[1].index = MAX_MEM;
    memoriaPrincipal->pageframe[0].tamanho = MAX_MEM;
    memoriaPrincipal->pageframe[0].index = 0;
    memoriaPrincipal->pageframe[0].ocupado = false;

}

int preenche_pagina(MEMORIA_PRINCIPAL *memoriaPrincipal, int tamanho_pagina, int index_pageframe, char operador){
    int tamanho_atual, index_atual;

    memoriaPrincipal->pageframe[index_pageframe].tamanho = tamanho_pagina;

    tamanho_atual = memoriaPrincipal->pageframe[index_pageframe].tamanho;

    memoriaPrincipal->pageframe[index_pageframe+1].index = memoriaPrincipal->pageframe[index_pageframe].tamanho + memoriaPrincipal->pageframe[index_pageframe].index;

    index_atual = memoriaPrincipal->pageframe[index_pageframe].index;
    switch (operador) { //Possivelmente o Switch nao sera necessario
        case 'A':
            for(int i = index_atual; i < tamanho_atual; i++){

            }
    }
}

void FirstFit(MEMORIA_PRINCIPAL *memoriaPrincipal, int tamanho_pagina){
    char op = 'A';
    for(int i = 0; i<MAX_MEM; i++){
        if(tamanho_pagina < memoriaPrincipal->pageframe[i].tamanho && memoriaPrincipal->pageframe[i].ocupado == false){
            preenche_pagina(memoriaPrincipal, tamanho_pagina, i, op);
            break;
        }
        
    }

}
