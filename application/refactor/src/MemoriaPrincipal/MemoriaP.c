#include "MemoriaP.h"

void inicializa_vazia(MEMORIA_PRINCIPAL *memoriaPrincipal){
    for(int i=1; i<MAX_MEM; i++){
        memoriaPrincipal->pageframe[i].index = -1;
    }
    memoriaPrincipal->pageframe[0].tamanho = MAX_MEM;
    memoriaPrincipal->pageframe[0].index = 0;

}

int preenche_pagina(MEMORIA_PRINCIPAL *memoriaPrincipal, int tamanho_pagina, int index_pageframe, char operador){
    int tamanho_atual, index_inicial;

    //memoriaPrincipal->pageframe[index_pageframe].tamanho = tamanho_pagina;

    memoriaPrincipal->pageframe[index_pageframe].tamanho -= tamanho_pagina;
    index_inicial = memoriaPrincipal->pageframe[index_pageframe].index;
    memoriaPrincipal->pageframe[index_pageframe].index += tamanho_pagina;

    return index_inicial;
}

int FirstFit(MEMORIA_PRINCIPAL *memoriaPrincipal, int tamanho_pagina){
    char op = 'A';
    printf("TAMANHO VAO %d\n\n\n\n\n\n\n\n\n", memoriaPrincipal->pageframe[0].tamanho);
    for(int i = 0; i<MAX_MEM; i++){
        if(tamanho_pagina <= memoriaPrincipal->pageframe[i].tamanho){
            return preenche_pagina(memoriaPrincipal, tamanho_pagina, i, op);
        }
        
    }
    return -1;

}

int removePagina(MEMORIA_PRINCIPAL *memoriaPrincipal, int tamanho_pagina, int index){
    for(int i=0;i<MAX_MEM-1;i++){
        if(memoriaPrincipal->pageframe[i].index + memoriaPrincipal->pageframe[i].tamanho == index && 
            memoriaPrincipal->pageframe[i+1].index == index + tamanho_pagina){
            
                memoriaPrincipal->pageframe[i].tamanho += tamanho_pagina + memoriaPrincipal->pageframe[i+1].tamanho;
                memoriaPrincipal->pageframe[i].index = -1;
        }

        else{
            if(memoriaPrincipal->pageframe[i].index + memoriaPrincipal->pageframe[i].tamanho == index){
                memoriaPrincipal->pageframe[i].tamanho += tamanho_pagina;
            }
            else if(memoriaPrincipal->pageframe[i+1].index == index + tamanho_pagina){
                memoriaPrincipal->pageframe[i+1].index-=tamanho_pagina;
                memoriaPrincipal->pageframe[i+1].tamanho+=tamanho_pagina;
            }

            else{
                frames pagina;
                pagina.index = index;
                pagina.tamanho = tamanho_pagina;
                bool flag = true;
                for(int i = MAX_MEM-2; i >= 0; i--){
                    if(memoriaPrincipal->pageframe[i].index > pagina.index){
                    
                        memoriaPrincipal->pageframe[i+1] = memoriaPrincipal->pageframe[i];

                    }
                    else{
                        if(flag){
                            flag = false;
                            memoriaPrincipal->pageframe[i+1] = pagina;
                        }
                        
                    }
                }
                if(flag){
                    memoriaPrincipal->pageframe[0] = pagina;
                }

            }

        }
        
    }
    printf("TAMANHO PAGINA %d\n\n\n\n\n" , tamanho_pagina);
    printf("TAMANHO INDEX %d\n\n\n\n\n\n", index);
    for(int i = index; i < index + tamanho_pagina; i++){
        memoriaPrincipal->RAM[i] = 0;
    }
    
    return 0;
}
