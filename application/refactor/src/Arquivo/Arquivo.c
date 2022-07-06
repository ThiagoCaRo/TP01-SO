#include "Arquivo.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int ler_programa(char *file, char programa[MAX_MEM][MAX_LINHA]) {
    char buffer[MAX_LINHA];
    int i = 0;
    FILE *fp = fopen(file, "r");

    if (fp == NULL)
        exit(1);

    for(int x = 0; x < MAX_LINHA; x++) {
        for(int y = 0; y < MAX_MEM; y++) {
            programa[x][y] = '\0';
        }
    }

    while (1) {
        if (fgets(buffer, MAX_LINHA, fp)==NULL) {
            //printf("VERIFY 0");
            break;
        }
        buffer[strcspn(buffer, "\n")] = 0;
        strcpy(programa[i],buffer);
        i++;
    }
    fclose(fp);
    return(0);
}