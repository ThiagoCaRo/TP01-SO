#include "threads.h"
char state;
int c=0;
int ler_op(char *linha, FILE *file){
    //printf("LER OP AQUI\n");
    int verify;
    if(fgets(linha, sizeof (linha), file)!=NULL){
        //printf("VERIFY 0");
        verify=0;
    }
    else{
        //printf("VERIFY 1");
        verify=1;
        return verify;
    }
    linha[strcspn(linha, "\n")] = 0;
    estados(&linha[0]);
    return verify;
}

void estados(char *buffer){
    int c = 0;
    int n;
    state=buffer[0];
    //printf("ESTADO: %c\n",state);
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
            /*c=0;
            state=buffer[0];*/
            break;

        case 'A':
            int up_offset;
            while(c < n){
                scanf("%d", &up_offset);
                x+=up_offset; //x tem que ser vetor
            }
            //ler_op(&linha[0]);
            /*c=0;
            state=buffer[0];*/
            break;   
    
        case 'S':
            int down_offset;
                while(c < n){
                    scanf("%d", &down_offset);
                    x-=down_offset; //x tem que ser vetor
                }
            //ler_op(&linha[0]);
            /*c=0;
            state=linha[0];*/
            break;

        case 'B':
            pthread_t           threadsim;    //Parâmetros da thread main
            pthread_attr_t      attrsim;
            int                 retcsim;
            pthread_attr_init(&attrsim);
            retcsim |= pthread_attr_setdetachstate(&attrsim, PTHREAD_CREATE_DETACHED);
            retcsim |= pthread_attr_setstacksize(&attrsim, THREADSTACKSIZE);
            retcsim = pthread_create(&threadsim, &attrsim, &threads, NULL);
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
void *threads(void *arg0){
    printf("\nSOU UMA THREAD\n");
    c++;
    printf("\n\n\n\nval = %d \n\n\n",c);
    int n;
    sem_t sem;
    FILE *fp;
    char nome_arq[61];
    char buffer[100];
    printf("Entre com o arquivo de comando da Thread\n\n");
    scanf("%s",nome_arq);
    fp=fopen(nome_arq,"r");
    if(fp==NULL){
        printf("Problema no f0\n");
        exit(1);
    }
    while(1){
        if(ler_op(&buffer[0],fp)!=0){
            break;
        }
    }
    
    
    fclose(fp);
    
    /*if(c==2){
        exit(1);
    }*/
    //char op;
    


}
