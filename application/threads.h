#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#define THREADSTACKSIZE    1024
void *threads(void *arg0);
int ler_op(char *linha, FILE *file);
