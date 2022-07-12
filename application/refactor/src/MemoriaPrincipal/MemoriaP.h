#ifndef __MEMORIAP_H__
#define __MEMORIAP_H__

#include "../Fila/Fila.h"

typedef struct{
    int RAM[MAX_MEM];
    int page_frame[MAX_MEM];

}MEMORIA_PRINCIPAL;

void preenche_pagina(int tamanho_pagina);
