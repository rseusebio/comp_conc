#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "timer.h"

float ** matrix_1;
float ** matrix_2;
float ** output;
int n_threads;
int dimension;

typedef struct 
{
    int t_id;
    int dimension;
} t_args;

void matrix_multiplication(int dimension, int init,  int step)
{
    // first matrix's line
    for(int i = init; i < dimension; i += step)
    {
        // second matrix's column
        for(int z = 0; z < dimension; z++)
        {
            //first matrix's column
            for (int j = 0; j < dimension; j++)
            {
                output[i][z] += matrix_1[i][j] * matrix_2[j][z];
            }
        }
    }
}

void * task(void * arg) 
{
    t_args * args = (t_args *) arg;

    matrix_multiplication(args->dimension, args->t_id, n_threads);

    pthread_exit(NULL);
}

void print_matrix(int dimension, int matrix_id)
{
    float ** matrix = NULL; 

    switch (matrix_id)
    {
        case 1:
            matrix = matrix_1;
            printf("matrix_1: \n");
            break;
        case 2:
            matrix = matrix_2;
            printf("matrix_2: \n");
            break;
        case 3:
            matrix = output;
            printf("output: \n");
            break;
        default:
            printf("Invalid matrix id.\n");
            break;
    }

    if (matrix == NULL)
    {
        return;
    }
    
    for(int j = 0; j < dimension; j++)
    {
        printf("[ ");

        //first matrix's column
        for (int i = 0; i < dimension; i++)
        {
            printf("%f ", matrix[j][i]);
        }

        printf("]\n");
    }

    printf("\n");
}

int validate_output(int dimension)
{    
    for(int i = 0; i < dimension; i++)
    {
        //first matrix's column
        for (int j = 0; j < dimension; j++)
        {
            if (output[i][j] != dimension)
            {
                return -1;
            }
        }
    }

    return 0;
}

void print_times(double init_delta, double exec_delta, double end_delta)
{
    float total = init_delta + exec_delta + end_delta;

    // printf("Initialization time: %f seconds.\n", init_delta);

    // printf("Execution time: %f seconds.\n", exec_delta);

    // printf("Termination time: %f seconds.\n", end_delta);

    // printf("Total time: %f seconds.\n", total);

    printf("%d, %d, %f, %f, %f, %f\n", n_threads, dimension, init_delta, exec_delta, end_delta, total);
}

int main(int argc, char * argv[])
{
    double start, end, init_delta, exec_delta, end_delta;
    
    pthread_t * t_id;
    t_args * args;
    
    #pragma region Input verification
    if (argc < 3)
    {
        printf("Insert %s <matrix_dimension> <number_of_threads>\n", argv[0]);
        
        return 1;
    }

    dimension = atoi(argv[1]);
    n_threads = atoi(argv[2]);

    if (n_threads > dimension)
    {
        n_threads = dimension;
    }
    #pragma endregion

    #pragma region Memory allocation
    matrix_1 = (float **) malloc(sizeof(float **) * dimension);
    matrix_2 = (float **) malloc(sizeof(float **) * dimension);
    output = (float **) malloc(sizeof(float **) * dimension);

    for (int i = 0; i < dimension; i++)
    {
        matrix_1[i] = (float *) malloc(sizeof(float *) * dimension);
        matrix_2[i] = (float *) malloc(sizeof(float *) * dimension);
        output[i] = (float *) malloc(sizeof(float *) * dimension);
    }
    #pragma endregion

    #pragma region Matrix and clock initialization
    GET_TIME(start);
    for (int i = 0; i < dimension; i++)
    {
        for (int j = 0; j < dimension; j++)
        {
            matrix_1[i][j] += 1.0;
            matrix_2[i][j] += 1.0;
            output[i][j] = 0.0;
        }
    }

    GET_TIME(end);
    init_delta = end - start;

    #pragma endregion

    #pragma region Sequential execution
    if(n_threads == 0)
    {
        GET_TIME(start);
        matrix_multiplication(dimension, 0, 1);
        GET_TIME(end);
        exec_delta = end - start;

        GET_TIME(start);

        if (validate_output(dimension) < 0)
        {
            printf("output matrix is invalid.\n");
            
            return 4;
        }

        GET_TIME(end);
        end_delta = end - start;

        print_times(init_delta, exec_delta, end_delta);

        return 0;
    }
    #pragma endregion

    #pragma region Starting threads
    GET_TIME(start);

    t_id = (pthread_t *) malloc(sizeof(pthread_t) * n_threads);

    if (t_id == NULL)
    {
        printf("Could not allocate memory for pthread_t.\n");

        return 2;
    }

    args = (t_args *) malloc(sizeof(t_args) * n_threads);

    if (args == NULL)
    {
        printf("Could not allocate memory for t_args.\n");
        
        return 2;
    }

    for (int i = 0; i < n_threads; i++)
    {
        args[i].t_id = i;
        args[i].dimension = dimension;

        if (pthread_create(&t_id[i], NULL, task, (void *)(args + i)))
        {
            printf("Could not create thread no %d.\n", i);

            return 3;
        }
    }

    for (int i = 0; i < n_threads; i++)
    {
        pthread_join(t_id[i], NULL);
    }

    GET_TIME(end);
    exec_delta = end - start;
    #pragma endregion

    #pragma region Validate Result
    GET_TIME(start);

    if (validate_output(dimension) < 0)
    {
        printf("output matrix is invalid.\n");
        
        return 4;
    }
    #pragma endregion

    #pragma region Free memory
    for (int i = 0; i < dimension; i++)
    {
        free(matrix_1[i]);
        free(matrix_2[i]);
        free(output[i]);
    }

    free(matrix_1);
    free(matrix_2);
    free(output);
    free(args);
    free(t_id);

    GET_TIME(end);
    end_delta = end - start;

    print_times(init_delta, exec_delta, end_delta);

    #pragma endregion

    return 0;
}