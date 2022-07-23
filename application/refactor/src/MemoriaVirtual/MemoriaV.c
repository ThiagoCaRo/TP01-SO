#include "MemoriaV.h"

void inicializa_vaziaV(MEMORIA_VIRTUAL *memoriaVirtual){
    for(int i=1; i<MAX_MEM; i++){
        memoriaVirtual->pageframe[i].index = -1;
    }
    memoriaVirtual->pageframe[0].tamanho = MAX_VIR;
    memoriaVirtual->pageframe[0].index = 0;
    inicializa_vazia(&memoriaVirtual->memoriaPrincipal, MAX_RAM);
    inicializa_vazia(&memoriaVirtual->disco, MAX_DISC);
}

int preenche_paginaV(MEMORIA_VIRTUAL *memoriaVirtual, int tamanho_pagina, int index_pageframe){
    int index_inicial;

    memoriaVirtual->pageframe[index_pageframe].tamanho -= tamanho_pagina;
    index_inicial = memoriaVirtual->pageframe[index_pageframe].index;

    if (memoriaVirtual->pageframe[index_pageframe].tamanho == 0){
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

int FitV(MEMORIA_VIRTUAL *memoriaVirtual, int tamanho_pagina){
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

int removePaginaV(MEMORIA_VIRTUAL *memoriaVirtual, int tamanho_pagina, int index){
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
    for(int i = index; i < index + tamanho_pagina; i++){
        if (memoriaVirtual->VIR[i].naPrincipal)
            removePagina(&(memoriaVirtual->memoriaPrincipal),1,memoriaVirtual->VIR[i].index);
        if (!memoriaVirtual->VIR[i].naPrincipal)
            removePagina(&(memoriaVirtual->disco),1,memoriaVirtual->VIR[i].index);
        memoriaVirtual->VIR[i].index = -1;
    }
    return 0;
}

void LiberaRam(MEMORIA_VIRTUAL * memoriaVirtual, char tipo, int *next){
    int minAc = -1;
    for (int i = 0; i < MAX_VIR; i++){
        if (memoriaVirtual->VIR[i].index != -1 &&
            memoriaVirtual->VIR[i].naPrincipal == 1 &&
            ((minAc == -1) | (memoriaVirtual->VIR[i].ultimoAcesso < memoriaVirtual->VIR[minAc].ultimoAcesso))){
            minAc = i;
        }
    }
    memoriaVirtual->VIR[minAc].naPrincipal = 0;
    int ram_i = memoriaVirtual->VIR[minAc].index;
    memoriaVirtual->VIR[minAc].index = Fit(&(memoriaVirtual->disco),1,tipo,next);
    memoriaVirtual->disco.variaveis[memoriaVirtual->VIR[minAc].index] = memoriaVirtual->memoriaPrincipal.variaveis[ram_i];
    removePagina(&(memoriaVirtual->memoriaPrincipal),1,ram_i);
}

int * GetAccess(MEMORIA_VIRTUAL *memoriaVirtual, int endereco, char tipo, int *next){
    memoriaVirtual->acessos++;
    if (memoriaVirtual->VIR[endereco].index == -1) {
        int index = Fit(&memoriaVirtual->memoriaPrincipal,1, tipo, next);
        if (index == -1) {
            LiberaRam(memoriaVirtual, tipo, next);
            index = Fit(&memoriaVirtual->memoriaPrincipal,1, tipo, next);
        }
        memoriaVirtual->VIR[endereco].index = index;
        memoriaVirtual->VIR[endereco].ultimoAcesso = memoriaVirtual->acessos;
        memoriaVirtual->VIR[endereco].naPrincipal = true;
        return &memoriaVirtual->memoriaPrincipal.variaveis[index];
    }
    else {
        if(memoriaVirtual->VIR[endereco].naPrincipal){
            memoriaVirtual->VIR[endereco].ultimoAcesso = memoriaVirtual->acessos;
            return &memoriaVirtual->memoriaPrincipal.variaveis[memoriaVirtual->VIR[endereco].index];
        }
        else {
            int disco_index = memoriaVirtual->VIR[endereco].index;
            int index = Fit(&memoriaVirtual->memoriaPrincipal,1, tipo, next);

            if (index == -1) {
                LiberaRam(memoriaVirtual, tipo, next);
                index = Fit(&memoriaVirtual->memoriaPrincipal,1, tipo, next);
            }
            memoriaVirtual->VIR[endereco].index = index;
            memoriaVirtual->VIR[endereco].ultimoAcesso = memoriaVirtual->acessos;
            memoriaVirtual->VIR[endereco].naPrincipal = true;

            memoriaVirtual->memoriaPrincipal.variaveis[index] = memoriaVirtual->disco.variaveis[disco_index];

            removePagina(&(memoriaVirtual->disco),1,disco_index);

            return &memoriaVirtual->memoriaPrincipal.variaveis[index];
        }
    }
}