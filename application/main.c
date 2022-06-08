
#include <stdint.h>
#include <stdio.h>
#include <pthread.h>
void ler_op(char *linha);
char state = 'N';

void ler_op(char *linha){
    fgets(linha, sizeof (linha), stdin);
    linha[strcpsn(linha, "\n")] = 0;
}
int main(){
    int n;
    int c=0;
    //char op;
    char linha[100];  

    switch (state)
    {
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
        ler_op(&linha[0]);
        c=0;
        state=linha[0];

        break;
    case 'A':
        int up_offset;
        while(c < n){
            scanf("%d", &up_offset);
            x+=up_offset; //x tem que ser vetor
        }
        ler_op(&linha[0]);
        c=0;
        state=linha[0];
        break;    
    case 'S':
        int down_offset;
            while(c < n){
                scanf("%d", &down_offset);
                x-=down_offset; //x tem que ser vetor
            }
        ler_op(&linha[0]);
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
        scanf("%d",&n);
        state='D';
        break;
    }
    return 0;
}