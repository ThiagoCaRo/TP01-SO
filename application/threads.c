#include "threads.h"
char state;
int ler_op(char *linha, FILE *file){
    int verify;
    if(fgets(linha, sizeof (linha), file)!=NULL){
        verify=0;
    }
    else{
        verify=1;
    }
    linha[strcspn(linha, "\n")] = 0;
    return verify;
}

void *threads(void *arg0){
    int n;
    sem_t sem;
    int c=0;
    //char op;
    char linha[100];  

    switch (state){
    case 'D':
        int x;
        state='V';
        break;

    case 'V':
        while(c < n){
        scanf("%d",&x); //Tem que ser um vetor com x
        printf("\n");
        c++;
        }
        //ler_op(&linha[0]);
        c=0;
        state=linha[0];
        break;

    case 'A':
        int up_offset;
        while(c < n){
            scanf("%d", &up_offset);
            x+=up_offset; //x tem que ser vetor
        }
        //ler_op(&linha[0]);
        c=0;
        state=linha[0];
        break;   
 
    case 'S':
        int down_offset;
            while(c < n){
                scanf("%d", &down_offset);
                x-=down_offset; //x tem que ser vetor
            }
        //ler_op(&linha[0]);
        c=0;
        state=linha[0];
        break;

    case 'B':
        //Bloqueia
        break;

    case 'T':
        //Termina
        break;

    case 'F':
        break;

    case 'R':
        break;

    default:
        state='N';
        break;
    }
}
