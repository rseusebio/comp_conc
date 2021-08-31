#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

int N;
int *vector;
int blocked = 0;
pthread_mutex_t x_mutex;
pthread_cond_t x_cond;

void barrier(int nthreads)
{
    pthread_mutex_lock(&x_mutex); //init critical section

    if (blocked == (nthreads - 1))
    {
        pthread_cond_broadcast(&x_cond);

        blocked = 0;
    }
    else
    {
        blocked++;

        pthread_cond_wait(&x_cond, &x_mutex);
    }

    pthread_mutex_unlock(&x_mutex); //end critical section
}


int rand_int()
{
    return rand() % 9;
}

void init_vector()
{
    for (int i = 0; i < N; i++)
    {
        vector[i] = rand_int();
    }
}

int get_vector_sum()
{
    int s = 0;

    for (int j = 0; j < N; j++)
    {
        s += vector[j];
    }

    return s;
}

void *task(void *arg)
{
    int *id = (int *)arg;

    int *acc = malloc(sizeof(int));
    *acc = 0;

    for (int i = 0; i < N; i++)
    {
        *acc += get_vector_sum();

        printf("Thread %d finished SUM for the %d time.\n", *id, i + 1);

        barrier(N);

        vector[*id] = rand_int();

        printf("Thread %d CHANGED vector for the %d time.\n", *id, i + 1);

        barrier(N);
    }

    pthread_exit(acc);
}

int main(int argc, char *argv[])
{
    srand(time(NULL));

    if (argc < 2)
    {
        printf("Execute ./lab_5 <number_of_threads>\n");

        exit(-1);
    }

    N = atoi(argv[1]);

    vector = (int *)malloc(sizeof(int) * N);
    if (vector == NULL)
    {
        printf("Couldn't allocate memory for vector.\n");

        exit(-1);
    }

    pthread_t tids[N];
    int ids[N];

    for (int i = 0; i < N; i++)
    {
        ids[i] = i + 1;
        if (pthread_create(&tids[i], NULL, task, (void *)&ids[i]))
        {
            printf("Couldn't create thread no. %d.\n", i);

            exit(-1);
        }    
    }

    int *result  = malloc(sizeof(int));

    int last_value_found;

    for (int i = 0; i < N; i++)
    {
        if (pthread_join(tids[i], (void **)&result))
        {
            printf("Couldn't join thread no. %d\n", i);

            exit(-1);
        }

        if (i == 0)
        {
            last_value_found = *result;
        }
        else
        {
            if (last_value_found != *result)
            {
                printf("Thread no. %d found %d but Thread no. %d found %d.\n", i, *result, i - 1, last_value_found);

                exit(-1);
            }
        }

        printf("Thread no. %d found %d.\n", i, *result);
    }

    return 0;
}