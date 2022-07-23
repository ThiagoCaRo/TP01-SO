#include "Memoria.h"
#include <stdio.h>
#include <stdlib.h>

void inicializa_vazia(MEMORIA *memoria, int tamanho){
    for(int i=1; i<MAX_MEM; i++){
        memoria->pageframe[i].index = -1;
    }
    memoria->pageframe[0].tamanho = tamanho;
    memoria->pageframe[0].index = 0;

}

int preenche_pagina(MEMORIA *memoria, int tamanho_pagina, int index_pageframe){
    int index_inicial;

    memoria->pageframe[index_pageframe].tamanho -= tamanho_pagina;
    index_inicial = memoria->pageframe[index_pageframe].index;
    
    if (memoria->pageframe[index_pageframe].tamanho == 0){
        for(int i = 0; i < MAX_MEM-1; i++){
           if(memoria->pageframe[i].index >= memoria->pageframe[index_pageframe].index){

               memoria->pageframe[i] = memoria->pageframe[i+1];
 
           }
        }
    }
    else
        memoria->pageframe[index_pageframe].index += tamanho_pagina;

    return index_inicial;
}

int Fit(MEMORIA *memoria, int tamanho_pagina, char tipo, int *next){
	switch (tipo){
		case 'f':
			return FirstFit(memoria, tamanho_pagina);
		case 'n':
			return NextFit(memoria, tamanho_pagina, next);
		case 'w':
			return WorstFit(memoria, tamanho_pagina);
		case 'b':
			return BestFit(memoria, tamanho_pagina);
	}
	exit(-1);
}

int NextFit(MEMORIA *memoria, int tamanho_pagina, int *next){
    int i = *next;
    while(1){
        if(tamanho_pagina <= memoria->pageframe[i].tamanho){
            *next = i;
            return preenche_pagina(memoria, tamanho_pagina, i);
        }
        i++;
        if (i == MAX_MEM) i = 0;
        if (i == *next) break;
    }
    return -1;

}

int BestFit(MEMORIA *memoria, int tamanho_pagina){
    int min = -1;
    for(int i = 0; i<MAX_MEM; i++){
        if(((min == -1) | (memoria->pageframe[min].tamanho > memoria->pageframe[i].tamanho))
            && tamanho_pagina <= memoria->pageframe[i].tamanho
            && memoria->pageframe[i].index != -1){
            min = i;
        }
    }
    if (min != -1) return preenche_pagina(memoria, tamanho_pagina, min);
    return -1;

}

int WorstFit(MEMORIA *memoria, int tamanho_pagina){
    int max = -1;
    for(int i = 0; i<MAX_MEM; i++){
        if(((max == -1) | (memoria->pageframe[max].tamanho < memoria->pageframe[i].tamanho))
            && tamanho_pagina <= memoria->pageframe[i].tamanho
            && memoria->pageframe[i].index != -1){
            max = i;
        }
    }
    if (max != -1) return preenche_pagina(memoria, tamanho_pagina, max);
    return -1;

}

int FirstFit(MEMORIA *memoria, int tamanho_pagina){
    for(int i = 0; i<MAX_MEM; i++){
        if(tamanho_pagina <= memoria->pageframe[i].tamanho){
            return preenche_pagina(memoria, tamanho_pagina, i);
        }
        
    }
    return -1;

}

int removePagina(MEMORIA *memoria, int tamanho_pagina, int index){
    for(int i=0;i<MAX_MEM-1;i++){
        if(memoria->pageframe[i].index + memoria->pageframe[i].tamanho == index &&
                memoria->pageframe[i+1].index == index + tamanho_pagina){

            memoria->pageframe[i].tamanho += tamanho_pagina + memoria->pageframe[i+1].tamanho;
                for(int j = i+1; j < MAX_MEM-1; j++){
                    memoria->pageframe[j] = memoria->pageframe[j+1];
                }
                break;
        }
        if(memoria->pageframe[i].index + memoria->pageframe[i].tamanho == index){
            memoria->pageframe[i].tamanho += tamanho_pagina;
            break;
        }
        if(memoria->pageframe[i+1].index == index + tamanho_pagina){
            memoria->pageframe[i+1].index-=tamanho_pagina;
            memoria->pageframe[i+1].tamanho+=tamanho_pagina;
            break;
        }
        if (memoria->pageframe[i].index == index + tamanho_pagina && i == 0){
            memoria->pageframe[i].index-=tamanho_pagina;
            memoria->pageframe[i].tamanho+=tamanho_pagina;
            break;
        }
        if ((memoria->pageframe[i].index + memoria->pageframe[i].tamanho < index &&
                (memoria->pageframe[i+1].index > index + tamanho_pagina || memoria->pageframe[i+1].index == -1)) ||
                (memoria->pageframe[i].index > index && memoria->pageframe[i+1].index == -1)){

            frames pagina;
            pagina.index = index;
            pagina.tamanho = tamanho_pagina;
            bool flag = true;
            for(int j = MAX_MEM-2; j >= 0; j--){
                if(memoria->pageframe[j].index > pagina.index){

                    memoria->pageframe[j+1] = memoria->pageframe[j];

                }
                else{
                    if(flag && memoria->pageframe[j].index != -1){
                        flag = false;
                        memoria->pageframe[j+1] = pagina;
                        break;
                    }

                }
            }
            if(flag){
                memoria->pageframe[0] = pagina;
            }
            break;

        }

    }
    for(int i = index; i < index + tamanho_pagina; i++){
        memoria->variaveis[i] = 0;
    }
    
    return 0;
}
