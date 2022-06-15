#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#define THREADSTACKSIZE    1024
int ler_op(char *linha, FILE *file);
void *threads(void *arg0);
void estados(char *buffer);
