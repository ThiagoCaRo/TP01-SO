#include "MemoriaV.h"

void inicializa_vaziaV(MEMORIA_VIRTUAL *memoriaVirtual){
    for(int i=1; i<MAX_MEM; i++){ // coloca todos os de vao como -1
        memoriaVirtual->pageframe[i].index = -1;
    }
    memoriaVirtual->pageframe[0].tamanho = MAX_VIR; // coloca o primeiro vao como o tamanho total da memoria virtual
    memoriaVirtual->pageframe[0].index = 0;
    inicializa_vazia(&memoriaVirtual->memoriaPrincipal, MAX_RAM); // inicializa a memoria principal vazia com seu respectivo tamanho
    inicializa_vazia(&memoriaVirtual->disco, MAX_DISC); // inicializa o disco vazio com seu respectivo tamanho
}

int preenche_paginaV(MEMORIA_VIRTUAL *memoriaVirtual, int tamanho_pagina, int index_pageframe){ // preenche a pagina virtual
    int index_inicial;

    memoriaVirtual->pageframe[index_pageframe].tamanho -= tamanho_pagina; // retira o tamanho alocado
    index_inicial = memoriaVirtual->pageframe[index_pageframe].index;

    if (memoriaVirtual->pageframe[index_pageframe].tamanho == 0){ // retira do vetor de vaos caso o o vao nao exista
        for(int i = 0; i < MAX_MEM-1; i++){
            if(memoriaVirtual->pageframe[i].index >= memoriaVirtual->pageframe[index_pageframe].index){

                memoriaVirtual->pageframe[i] = memoriaVirtual->pageframe[i+1];

            }
        }
    }
    else
        memoriaVirtual->pageframe[index_pageframe].index += tamanho_pagina;

    return index_inicial;
}

int FitV(MEMORIA_VIRTUAL *memoriaVirtual, int tamanho_pagina){ // coloca na memoria virtual usando o first fit
    for(int i = 0; i<MAX_MEM; i++){
        if(tamanho_pagina <= memoriaVirtual->pageframe[i].tamanho){
            for (int j = 0; j < tamanho_pagina; j++){
                memoriaVirtual->VIR[memoriaVirtual->pageframe[i].index+j].index = -1;
            }
            return preenche_paginaV(memoriaVirtual, tamanho_pagina, i);
        }
    }
    return -1;
}

int removePaginaV(MEMORIA_VIRTUAL *memoriaVirtual, int tamanho_pagina, int index){ // remove a pagina exatamente como é feito nas memorias normais
    for(int i=0;i<MAX_MEM-1;i++){
        if(memoriaVirtual->pageframe[i].index + memoriaVirtual->pageframe[i].tamanho == index &&
                memoriaVirtual->pageframe[i+1].index == index + tamanho_pagina){

            memoriaVirtual->pageframe[i].tamanho += tamanho_pagina + memoriaVirtual->pageframe[i+1].tamanho;
            for(int j = i+1; j < MAX_MEM-1; j++){
                memoriaVirtual->pageframe[j] = memoriaVirtual->pageframe[j+1];
            }
            break;
        }
        if(memoriaVirtual->pageframe[i].index + memoriaVirtual->pageframe[i].tamanho == index){
            memoriaVirtual->pageframe[i].tamanho += tamanho_pagina;
            break;
        }
        if(memoriaVirtual->pageframe[i+1].index == index + tamanho_pagina){
            memoriaVirtual->pageframe[i+1].index-=tamanho_pagina;
            memoriaVirtual->pageframe[i+1].tamanho+=tamanho_pagina;
            break;
        } if (memoriaVirtual->pageframe[i].index == index + tamanho_pagina && i == 0){
            memoriaVirtual->pageframe[i].index-=tamanho_pagina;
            memoriaVirtual->pageframe[i].tamanho+=tamanho_pagina;
            break;
        }
         if ((memoriaVirtual->pageframe[i].index + memoriaVirtual->pageframe[i].tamanho < index &&
                  (memoriaVirtual->pageframe[i+1].index > index + tamanho_pagina || memoriaVirtual->pageframe[i+1].index == -1)) ||
                 (memoriaVirtual->pageframe[i].index > index && memoriaVirtual->pageframe[i+1].index == -1)){

            frames pagina;
            pagina.index = index;
            pagina.tamanho = tamanho_pagina;
            bool flag = true;
            for(int j = MAX_MEM-2; j >= 0; j--){
                if(memoriaVirtual->pageframe[j].index > pagina.index){

                    memoriaVirtual->pageframe[j+1] = memoriaVirtual->pageframe[j];

                }
                else{
                    if(flag && memoriaVirtual->pageframe[j].index != -1){
                        flag = false;
                        memoriaVirtual->pageframe[j+1] = pagina;
                        break;
                    }

                }
            }
            if(flag){
                memoriaVirtual->pageframe[0] = pagina;
            }
            break;

        }
    }
    for(int i = index; i < index + tamanho_pagina; i++){ // remove da memoria fisica os respectivos valores
        if (memoriaVirtual->VIR[i].naPrincipal)
            removePagina(&(memoriaVirtual->memoriaPrincipal),1,memoriaVirtual->VIR[i].index); // na memoria princial
        if (!memoriaVirtual->VIR[i].naPrincipal)
            removePagina(&(memoriaVirtual->disco),1,memoriaVirtual->VIR[i].index); // no disco
        memoriaVirtual->VIR[i].index = -1;
    }
    return 0;
}

void LiberaRam(MEMORIA_VIRTUAL * memoriaVirtual, char tipo, int *next){ // libera a ram de acordo com o endereco de memoria referenciado a mais tempo
    int minAc = -1;
    for (int i = 0; i < MAX_VIR; i++){
        if (memoriaVirtual->VIR[i].index != -1 &&
            memoriaVirtual->VIR[i].naPrincipal == 1 &&
            ((minAc == -1) | (memoriaVirtual->VIR[i].ultimoAcesso < memoriaVirtual->VIR[minAc].ultimoAcesso))){ // procura o endereco referenciado a mais tempo
            minAc = i;
        }
    }
    memoriaVirtual->VIR[minAc].naPrincipal = 0; // coloca a celula como referenciando algo fora da memoria principal
    int ram_i = memoriaVirtual->VIR[minAc].index; // salva o index antigo na ram
    memoriaVirtual->VIR[minAc].index = Fit(&(memoriaVirtual->disco),1,tipo,next); // aloca no disco a variavel
    memoriaVirtual->disco.variaveis[memoriaVirtual->VIR[minAc].index] = memoriaVirtual->memoriaPrincipal.variaveis[ram_i]; // transfere seu valor para o disco
    removePagina(&(memoriaVirtual->memoriaPrincipal),1,ram_i); // remove o endereco antigo da ram
}

int * GetAccess(MEMORIA_VIRTUAL *memoriaVirtual, int endereco, char tipo, int *next){
    memoriaVirtual->acessos++; // incrementa o contador de acessos
    if (memoriaVirtual->VIR[endereco].index == -1) { // caso nunca tenha tido um endereco antes
        int index = Fit(&memoriaVirtual->memoriaPrincipal,1, tipo, next); // tenta colocar na memoria principal
        if (index == -1) { // caso nao seja caiba, libera a ram e entao coloca
            LiberaRam(memoriaVirtual, tipo, next);
            index = Fit(&memoriaVirtual->memoriaPrincipal,1, tipo, next);
        }
        memoriaVirtual->VIR[endereco].index = index; // coloca seu novo endereco na ram
        memoriaVirtual->VIR[endereco].ultimoAcesso = memoriaVirtual->acessos; // atualiza o ultimo acesso
        memoriaVirtual->VIR[endereco].naPrincipal = true;
        return &memoriaVirtual->memoriaPrincipal.variaveis[index]; // retorna o endereco da variavel na ram
    }
    else {
        if(memoriaVirtual->VIR[endereco].naPrincipal){ // caso ja esteja na ram
            memoriaVirtual->VIR[endereco].ultimoAcesso = memoriaVirtual->acessos; // atualiza o ultimo acesso
            return &memoriaVirtual->memoriaPrincipal.variaveis[memoriaVirtual->VIR[endereco].index]; // retorna o endereco da variavel na ram
        }
        else { // caso esteja no disco
            int disco_index = memoriaVirtual->VIR[endereco].index; // obtem o endereco no disco
            int index = Fit(&memoriaVirtual->memoriaPrincipal,1, tipo, next); // aloca na ram

            if (index == -1) { // caso nao tenha espaco libera a ram e por fim aloca
                LiberaRam(memoriaVirtual, tipo, next);
                index = Fit(&memoriaVirtual->memoriaPrincipal,1, tipo, next);
            }
            memoriaVirtual->VIR[endereco].index = index; //  coloca seu novo endereco na ram
            memoriaVirtual->VIR[endereco].ultimoAcesso = memoriaVirtual->acessos; // atualiza o contador de acesso
            memoriaVirtual->VIR[endereco].naPrincipal = true;

            memoriaVirtual->memoriaPrincipal.variaveis[index] = memoriaVirtual->disco.variaveis[disco_index]; // coloca seu valor do disco na ram

            removePagina(&(memoriaVirtual->disco),1,disco_index); // libera a variavel do disco

            return &memoriaVirtual->memoriaPrincipal.variaveis[index]; // retorna o endereco da variavel na ram
        }
    }
}
