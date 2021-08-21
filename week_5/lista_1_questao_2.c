#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct  
{
    int id;
    int n_threads;
    long long int n;
}t_args;

/*
 * Função que calcula cada termo
 * da série de somas 
*/
float func(int n)
{
    int x = 2 * n + 1;

    int y = 1;

    if (n % 2 != 0)
    {
        y = -1;
    }

    return y / x;
}

/*
 * Função que será executada por cada thread
 * como agumento recebe uma estrutura que contém:
 * o id da thread, o número de threads criadas e 
 * o número de termos a serem achados na série 
*/
void * task(void * arg)
{
    t_args * args = (t_args *) arg;

    float *result = malloc(sizeof(float));

    *result = 0;

    /*
     * Cada thread vai calcular um termo da sequência 
     * de forma alternada. Dessa forma, a ideia é 
     * chegar em um tempo de execução concorrente 
     * igual ao tempo sequential divido pelo número de threads
     * (mais o overhead)
    */
    for(int i = args->id; i < args->n; i += args->n_threads)
    {
        *result += func(i);
    }

    pthread_exit((void *) result);
}