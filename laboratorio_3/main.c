#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "timer.h"

float * vector;
int n_threads;
long long int dimension;

typedef struct 
{
    float min;
    float max;
} v_analyze;

void * task(void * arg) 
{
    int * id = (int *) arg;
 
    v_analyze * analysis = malloc(sizeof(v_analyze));

    if (analysis == NULL)
    {
        fprintf(stderr, "couldn't allocate memory analysis object.\n");

        exit(1);
    }

    analysis->max = 0;
    analysis->min = 0;

    long long int block_size = dimension / n_threads;
    long long int start = *id * block_size;
    long long int end;

    if (*id + 1 == n_threads)
    {
        end = dimension;
    }
    else 
    {
        end = start + block_size;
    }

    for (long long int i = start; i < end; i++)
    {
        if (analysis->max < vector[i])
        {
            analysis->max = vector[i];
        }

        if (analysis->min > vector[i])
        {
            analysis->min = vector[i];
        }
    }

    pthread_exit((void *) analysis);
}

int validate_output(v_analyze analysis, float min, float max)
{    
    if (analysis.max == max && analysis.min == min)
    {
        return -1;
    }

    return 0;
}

void print_times(double time)
{
    printf("%d, %lld, %f\n", n_threads, dimension, time);
}

int main(int argc, char * argv[])
{
    double start, end, exec_time;

    float max, min;
    
    pthread_t * t_id;
    int * ids;
    v_analyze analysis = { .max = 0.0, .min = 0.0 }, *thread_analysis;
    
    #pragma region Input verification
    if (argc < 3)
    {
        printf("Insert %s <vector_dimension> <number_of_threads>\n", argv[0]);
        
        return 1;
    }

    dimension = atoll(argv[1]);
    n_threads = atoi(argv[2]);

    if (n_threads > dimension)
    {
        n_threads = dimension;
    }
    #pragma endregion

    #pragma region Memory allocation
    vector = malloc(sizeof(float) * dimension);

    if (vector == NULL)
    {
        fprintf(stderr, "couldn't allocate memory to vector.\n");

        exit(1);
    }
    #pragma endregion

    #pragma region vector initialization
    for (int i = 0; i < dimension; i++)
    {
        vector[i] = 10000.1 / (i + 1);

        if (i == 0)
        {
            max = vector[i];
        }
        else if (i == dimension - 1)
        {
            min = vector[i];
        }
    }
    #pragma endregion

    #pragma region Sequential execution
    if(n_threads == 0)
    {
        GET_TIME(start);

        for (long long int i = 0; i < dimension; i++)
        {
            if (vector[i] > analysis.max)
            {
                analysis.max = vector[i];
            }

            if (vector[i] < analysis.min)
            {
                analysis.min = vector[i];
            }
        }

        GET_TIME(end);

        exec_time = end -start;
        
        if (validate_output(analysis, min, max) < 0)
        {
            printf("sequential analysis is invalid.\n");
            
            exit(4);
        }

        print_times(exec_time);

        return 0;
    }
    #pragma endregion

    #pragma region Starting threads
    GET_TIME(start);

    t_id = (pthread_t *) malloc(sizeof(pthread_t) * n_threads);
    if (t_id == NULL)
    {
        printf("Could not allocate memory for pthread_t.\n");

        exit(2);
    }

    ids = (int *) malloc(sizeof(int) * n_threads);
    if (ids == NULL)
    {
        printf("Could not allocate memory for ids.\n");
        
        exit(2);
    }

    for (int i = 0; i < n_threads; i++)
    {
        ids[i] = i;

        if (pthread_create(&t_id[i], NULL, task, (void *) &ids[i]))
        {
            printf("Could not create thread no %d.\n", i);

            exit(3);
        }
    }

    for (int i = 0; i < n_threads; i++)
    {
        pthread_join(t_id[i], (void **) &thread_analysis);

        if(thread_analysis->max > analysis.max)
        {
            analysis.max = thread_analysis->max;
        }

        if(thread_analysis->min < analysis.min)
        {
            analysis.min = thread_analysis->min;
        }

        free(thread_analysis);
    }

    GET_TIME(end);

    exec_time = end - start;
    #pragma endregion

    #pragma region Validate Result
    if (validate_output(analysis, min, max) < 0)
    {
        printf("concurrent analysis is invalid.\n");
        
        exit(4);
    }
    #pragma endregion

    #pragma region Free memory
    free(t_id);
    free(ids);
    free(vector);

    print_times(exec_time);
    #pragma endregion

    return 0;
}