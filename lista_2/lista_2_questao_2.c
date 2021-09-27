#include <pthread.h>
#include <stdio.h>
#include <math.h>

/* Variaveis globais */
// int bloqueadas = 0;

// pthread_mutex_t contador_mutex;

pthread_mutex_t x_mutex;
pthread_cond_t x_cond;

pthread_mutex_t y_mutex;
pthread_cond_t y_cond;

int nthreads = 2;
int contador = 0;
int bloqueadas = 0;
// int *vetor;

/* Funcao barreira */
void barreira() 
{
    pthread_mutex_lock(&x_mutex);

    if (bloqueadas == (nthreads-1)) 
    {
        //ultima thread a chegar na barreira

        pthread_cond_broadcast(&x_cond);

        bloqueadas = 0;
    }
    else 
    {
        bloqueadas++;

        pthread_cond_wait(&x_cond, &x_mutex);
    }
    
    pthread_mutex_unlock(&x_mutex);
}

void FazAlgo(int contador)
{
    double boba1 = pow(10, 6);

    double boba2 = -1 * boba1;

    // printf("%lf, %lf\n", boba1, boba2);

    while (boba2 < boba1)
    {
        boba2 += 1;
    }
}

void *T1(void *arg)
{
    while (1)
    {
        FazAlgo(contador);

        contador++;

        if (contador > 0 && contador % 100 == 0)
        {
            // Sinaliza a T2 para que imprima
            pthread_cond_broadcast(&y_cond);


            // Espera a T2 imprimir
            barreira();
        }

    }
}

void *T2(void *arg)
{
    while (1)
    {
        // Espera o T1 achar um contador 
        pthread_cond_wait(&y_cond, &y_mutex);

        printf("FazAlgo executou %d vezes.\n", contador);

        // Libera a T1 para prosseguir
        barreira();
    }
}

/* Funcao principal */
int main(int argc, char *argv[])
{
    pthread_t threads[nthreads];

    pthread_mutex_init(&x_mutex, NULL);
    pthread_mutex_init(&y_mutex, NULL);
    pthread_cond_init(&x_cond, NULL);
    pthread_cond_init(&y_cond, NULL);

    for (int i = 0; i < nthreads; i++)
    {
        if (i == 0)
        {
            pthread_create(&threads[i], NULL, T1, NULL);
        }
        else
        {
            pthread_create(&threads[i], NULL, T2, NULL);
        }
    }

    /* Espera todas as threads completarem */
    for (int i = 0; i < nthreads; i++)
    {
        pthread_join(threads[i], NULL);
    }
    
    return 0;
}