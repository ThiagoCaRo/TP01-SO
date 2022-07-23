#include "MemoriaP.h"
#include <stdio.h>
#include <stdlib.h>

void inicializa_vazia(MEMORIA_PRINCIPAL *memoriaPrincipal){
    for(int i=1; i<MAX_MEM; i++){ // coloca o indice de todos os vaos como vazios
        memoriaPrincipal->pageframe[i].index = -1;
    }
    memoriaPrincipal->pageframe[0].tamanho = MAX_RAM; // coloca o primeiro vao como sendo a memoria inteira
    memoriaPrincipal->pageframe[0].index = 0;

}

int preenche_pagina(MEMORIA_PRINCIPAL *memoriaPrincipal, int tamanho_pagina, int index_pageframe){
    int index_inicial;

    memoriaPrincipal->pageframe[index_pageframe].tamanho -= tamanho_pagina; // reduz o tamanho do vao de acordo com o que vc quer alocar
    index_inicial = memoriaPrincipal->pageframe[index_pageframe].index;
    
    if (memoriaPrincipal->pageframe[index_pageframe].tamanho == 0){ // caso o vao tenha tamanho igual a 0 ele é excluido
        for(int i = 0; i < MAX_MEM-1; i++){
           if(memoriaPrincipal->pageframe[i].index >= memoriaPrincipal->pageframe[index_pageframe].index){
     
               memoriaPrincipal->pageframe[i] = memoriaPrincipal->pageframe[i+1];
 
           }
        }
    }
    else
    	memoriaPrincipal->pageframe[index_pageframe].index += tamanho_pagina; // caso nao, ele apenas tem seu index inicial mudado

    return index_inicial; // retorna o antigo index inicial
}

int Fit(MEMORIA_PRINCIPAL *memoriaPrincipal, int tamanho_pagina, char tipo, int *next){
	switch (tipo){
		case 'f':
			return FirstFit(memoriaPrincipal, tamanho_pagina);
		case 'n':
			return NextFit(memoriaPrincipal, tamanho_pagina, next);
		case 'w':
			return WorstFit(memoriaPrincipal, tamanho_pagina);
		case 'b':
			return BestFit(memoriaPrincipal, tamanho_pagina);
	}
	exit(-1);
}

int NextFit(MEMORIA_PRINCIPAL *memoriaPrincipal, int tamanho_pagina, int *next){
    int i = *next;
    while(1){
        if(tamanho_pagina <= memoriaPrincipal->pageframe[i].tamanho){ // procura o proximo que esta vazio e preenche or espectivo vao
            *next = i;
            return preenche_pagina(memoriaPrincipal, tamanho_pagina, i);
        }
        i++;
        if (i == MAX_MEM) i = 0;
        if (i == *next) break;
    }
    return -1;

}

int BestFit(MEMORIA_PRINCIPAL *memoriaPrincipal, int tamanho_pagina){
    int min = -1;
    for(int i = 0; i<MAX_MEM; i++){
        if(((min == -1) | (memoriaPrincipal->pageframe[min].tamanho > memoriaPrincipal->pageframe[i].tamanho))
            && tamanho_pagina <= memoriaPrincipal->pageframe[i].tamanho
            && memoriaPrincipal->pageframe[i].index != -1){ //  procura o menor que caiba o que vc quer alocar e preenche o vao
            min = i;
        }
    }
    if (min != -1) return preenche_pagina(memoriaPrincipal, tamanho_pagina, min);
    return -1;

}

int WorstFit(MEMORIA_PRINCIPAL *memoriaPrincipal, int tamanho_pagina){
    int max = -1;
    for(int i = 0; i<MAX_MEM; i++){
        if(((max == -1) | (memoriaPrincipal->pageframe[max].tamanho < memoriaPrincipal->pageframe[i].tamanho))
            && tamanho_pagina <= memoriaPrincipal->pageframe[i].tamanho
            && memoriaPrincipal->pageframe[i].index != -1){ //procura o maior que caiba o que voce quer alocar e preenche o vao
            max = i;
        }
    }
    if (max != -1) return preenche_pagina(memoriaPrincipal, tamanho_pagina, max);
    return -1;

}

int FirstFit(MEMORIA_PRINCIPAL *memoriaPrincipal, int tamanho_pagina){
    for(int i = 0; i<MAX_MEM; i++){
        if(tamanho_pagina <= memoriaPrincipal->pageframe[i].tamanho){ // procura o primeiro que caiba e preenche o vao
            return preenche_pagina(memoriaPrincipal, tamanho_pagina, i);
        }
        
    }
    return -1;

}

int removePagina(MEMORIA_PRINCIPAL *memoriaPrincipal, int tamanho_pagina, int index){
    for(int i=0;i<MAX_MEM-1;i++){ // procura a vizinhanca do vao que voce quer remover
        if(memoriaPrincipal->pageframe[i].index + memoriaPrincipal->pageframe[i].tamanho == index && 
            memoriaPrincipal->pageframe[i+1].index == index + tamanho_pagina){ // caso exista uma atras e um na frente
            
                memoriaPrincipal->pageframe[i].tamanho += tamanho_pagina + memoriaPrincipal->pageframe[i+1].tamanho; // o mais atras abrange os dois novos da frente
                for(int j = i+1; j < MAX_MEM-1; j++){ // retira o mais a frente com um shift no vetor
                    memoriaPrincipal->pageframe[j] = memoriaPrincipal->pageframe[j+1];
                }
                break;
        }
        if(memoriaPrincipal->pageframe[i].index + memoriaPrincipal->pageframe[i].tamanho == index){ // caso exista somente atras
            memoriaPrincipal->pageframe[i].tamanho += tamanho_pagina; // o de tras abrange o espaco desalocado
            break;
        }
        if(memoriaPrincipal->pageframe[i+1].index == index + tamanho_pagina){ // caso exista somente na frente
            memoriaPrincipal->pageframe[i+1].index-=tamanho_pagina; // o ponteiro inicial do vao recua com o espaco desalocado
            memoriaPrincipal->pageframe[i+1].tamanho+=tamanho_pagina; // o tamanho do vao aumenta proporcionalmente
            break;
        }
        if (memoriaPrincipal->pageframe[i].index == index + tamanho_pagina && i == 0){ // caso so exista na frente, mas na frente é o primeiro no vetor
            memoriaPrincipal->pageframe[i].index-=tamanho_pagina;
            memoriaPrincipal->pageframe[i].tamanho+=tamanho_pagina;
            break;
        }
        if ((memoriaPrincipal->pageframe[i].index + memoriaPrincipal->pageframe[i].tamanho < index &&
                (memoriaPrincipal->pageframe[i+1].index > index + tamanho_pagina || memoriaPrincipal->pageframe[i+1].index == -1)) ||
                (memoriaPrincipal->pageframe[i].index > index && memoriaPrincipal->pageframe[i+1].index == -1)){ // caso nao exista atras e nem na frente

            frames pagina; // cria a pagina com o tamanho do espaco desalocado e seu respectivo endereco
            pagina.index = index;
            pagina.tamanho = tamanho_pagina;
            bool flag = true; // flag para caso exista nenhuma pagina
            for(int j = MAX_MEM-2; j >= 0; j--){ // shift para a direita no vetor
                if(memoriaPrincipal->pageframe[j].index > pagina.index){

                    memoriaPrincipal->pageframe[j+1] = memoriaPrincipal->pageframe[j];

                }
                else{
                    if(flag && memoriaPrincipal->pageframe[j].index != -1){ // coloca o novo vao no espaco novo
                        flag = false;
                        memoriaPrincipal->pageframe[j+1] = pagina;
                        break;
                    }

                }
            }
            if(flag){
                memoriaPrincipal->pageframe[0] = pagina;
            }
            break;

        }

    }
    for(int i = index; i < index + tamanho_pagina; i++){
        memoriaPrincipal->RAM[i] = 0; // esvazia os valores da ram
    }
    
    return 0;
}
