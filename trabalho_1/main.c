#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include "timer.h"

int MAX_INTEGER = 100;
double ERROR = (double) 1/1000;

double ** input_matrix;
int n_threads;
int matrix_type;
int dimension;

pthread_mutex_t lock;
int line_pointer = 0;
int is_row = 0;

typedef struct 
{
    int t_id;    
} t_args;

typedef struct
{
    int index;
    int amount;
    int is_row;
} t_zeros_count;

t_zeros_count *gt_line = NULL;

/**
 * Count the amount of zeros 
 * in giving matrix's column or row
 */
int count_zeros(double **matrix, int size, int index, int is_row)
{
    int r, c, count = 0;

    for (int i = 0; i < dimension; i++)
    {
        if (is_row)
        {
            r = index;
            c = i;
        }
        else
        {
            r = i;
            c = index;
        }

        if (input_matrix[r][c] == 0)
        {
            count++;
        }
    }
 
    return count;
}

/**
 * Find the column or row
 * with most zeros 
 * in a given matrix
 * sequentially 
 */
t_zeros_count get_zeros_count(double **matrix, int size)
{
    t_zeros_count row = {.amount = 0, .is_row = 1};

    for (int i = 0; i < size; i++)
    {
        int zeros = count_zeros(matrix, size, i, 1);

        if (i == 0 || (zeros > row.amount))
        {
            row.index = i;
            row.amount = zeros;
        }
    }

    t_zeros_count column = {.amount = 0, .is_row = 0};

    for (int j = 0; j < size; j++)
    {
        int zeros = count_zeros(matrix, size, j, 0);

        if (j == 0 || (zeros > column.amount))
        {
            column.index = j;
            column.amount = zeros;
        }
    }

    if(column.amount > row.amount)
    {
        return column;
    }

    return row;
}

/**
 * Task to find 
 * the column or row
 * with most zeros 
 * in a given matrix
 * concurrently
 */
void *count_zeros_task(void * arg) 
{
    t_args * args = (t_args *) arg;

    t_zeros_count * output = malloc(sizeof(t_zeros_count));

    output->index = args->t_id;
    output->is_row = 0;
    output->amount = 0;

    // Holders
    int h_line_pointer, h_is_row;

    while(1)
    {
        pthread_mutex_lock(&lock);
        if (is_row > 1)
        {
            pthread_mutex_unlock(&lock);

            break;
        }

        h_line_pointer = line_pointer;
        h_is_row = is_row;

        if (line_pointer == dimension - 1)
        {
            is_row++;
            line_pointer = 0;
        }
        else
        {
            line_pointer++;
        }
        pthread_mutex_unlock(&lock);

        int count = count_zeros(input_matrix, dimension, h_line_pointer, h_is_row);

        // char *str = h_is_row ? "row" : "column";
        // printf("Thread: %d found %d zeros at %s %d\n", args->t_id, count, str, h_line_pointer + 1);

        if (count > output->amount)
        {
            output->index = h_line_pointer;
            output->amount = count;
            output->is_row = h_is_row;
        }
    }
    
    pthread_exit(output);
}

/**
 * Calculate the sign of the cofactor 
 * based on the row and column
 * of the selected element 
 **/
int get_sign(int row, int column)
{
    if ((row + column) % 2 == 0)
    {
        return 1;
    }

    return -1;
}

/**
 * Function to allocate 
 * memory to a two-dimensional
 * matrix of double elements
 * of a given size
 */ 
double **malloc_matrix(int size)
{
    double **matrix = (double **)malloc(sizeof(double *) * (size));

    for (int i = 0; i < size; i++)
    {
        matrix[i] = (double *)malloc(sizeof(double) * size);
    }

    return matrix;
}

/**
 * Function to free a matrix
 * of a given size
 */ 
void free_matrix(double **matrix, int size)
{
    for (int i = 0; i < size; i++)
    {
        free(matrix[i]);
    }

    free(matrix);
}

/**
 * Function to fill a 
 * matrix with the same values
 * of the parent matrix
 * except the elements
 * that are either on the exclude_i row
 * or on the exclude_j column
 * Its easy to note that the new matrix
 * has size: N - 1
 */ 
void fill_matrix(double **matrix, double **parent_matrix, int size, int exclude_i, int exclude_j)
{
    int new_i, new_j;

    for (int i = 0; i < size; i++)
    {
        if (i == exclude_i)
        {
            continue;
        }

        new_i = i;

        if (i > exclude_i)
        {
            new_i = i - 1;
        }

        for (int j = 0; j < size; j++)
        {
            if (j == exclude_j)
            {
                continue;
            }

            new_j = j;
        
            if (j > exclude_j)
            {
                new_j = j - 1;
            }

            matrix[new_i][new_j] = parent_matrix[i][j];
        }
    }
}

/**
 * Function created
 * to print an entire 
 * matrix of double values
 */ 
void print_matrix(double **matrix, int size)
{
    printf("[\n");
    for(int j = 0; j < size; j++)
    {
        //first matrix's column
        for (int i = 0; i < size; i++)
        {
            printf(" %lf ", matrix[j][i]);
        }

        printf("\n");
    }

    printf("]\n\n");
}

/**
 * Recursive function 
 * to calculate a determinant
 * of a matrix created from the 
 * passed matrix when the row i and 
 * column j are both excluded.
 */ 
double determinant(double **matrix, int size, int i, int j)
{
    double **new_matrix = malloc_matrix(size - 1);

    fill_matrix(new_matrix, matrix, size, i, j);

    // print_matrix(new_matrix, size - 1);

    double det = 0;

    if (size - 1 == 2)
    {
        det = new_matrix[0][0] * new_matrix[1][1] - new_matrix[1][0] * new_matrix[0][1];

        free_matrix(new_matrix, size - 1);

        return det;
    }

    //find the column or row with most zeros
    t_zeros_count zeros = get_zeros_count(new_matrix, size - 1);

    for (int x = 0; x < size - 1; x++)
    {
        if (zeros.is_row)
        {
            i = zeros.index;
            j = x;
        }
        else
        {
            i = x;
            j = zeros.index;
        }
        
        double a = new_matrix[i][j];

        if (a == 0)
        {
            continue;
        }

        det += get_sign(i, j) * a * determinant(new_matrix, size - 1, i, j);
    }

    free_matrix(new_matrix, size - 1);

    return det;
}

/**
 * Task that each thread
 * is supposed to execute 
 * to find the cofactor of 
 * a giving element
 */
void *task(void *arg)
{
    double *total = malloc(sizeof(double));
    *total = 0;

    int i, j, x;

    double a;

    while(1)
    {
        pthread_mutex_lock(&lock);

        if (line_pointer >= dimension)
        {
            pthread_mutex_unlock(&lock);

            break;
        }

        x = line_pointer;

        line_pointer++;

        pthread_mutex_unlock(&lock);

        if (gt_line->is_row)
        {
            i = gt_line->index;
            j = x;
        }
        else
        {
            i = x;
            j = gt_line->index;
        }

        a = input_matrix[i][j];

        if (a == 0)
        {
            continue;
        }

        *total +=  get_sign(i, j) * a * determinant(input_matrix, dimension, i, j);
    }

    pthread_exit(total);
}

/**
 * Function validate 
 * the result of a determinant
 * of tringular, diagonal or identity
 * matrix.
 */ 
int validate_result(double determinant)
{   
    double det = 1;

    for(int i = 0; i < dimension; i++)
    {
        for (int j = 0; j < dimension; j++)
        {
            if (i == j)
            {
                det *= (double) input_matrix[i][j];
            }
        }
    }

    double err = (double) (determinant - det);

    // printf("found: %lf, correct: %lf\n", determinant, det);

    if (err < 0)
    {
        err *= (double) -1;
    }

    // char *str = err <= 0.0001 ? "TRUE" : "FALSE";
    // printf("(%s) error: %lf, limit: %lf\n", str, err, ERROR);

    return err <= 0.0001;
}

/**
 * Returns a random double
 * between 0.55 and 1.55
 * Those values or selected 
 * to generate a low determinante
 * and not overflow the memory
 */ 
double rand_double()
{
    double x = (double) (rand() % MAX_INTEGER);

    return x / 100.0 + 0.55;
}

/**
 * Creates a tringular matrix
 * either upper or lower.
 */ 
void triangular(int is_upper)
{
    for (int i = 0; i < dimension; i++)
    {
        for (int j = 0; j < dimension; j++)
        {
            if (( !is_upper && j > i) || (i > j && is_upper))
            {
                input_matrix[i][j] = 0;

                continue;
            }

            input_matrix[i][j] = rand_double(); 
        }
    }
}

/**
 * Creates a diagonal (or identity) 
 * matrix.
 */ 
void diagonal(int is_identity)
{
    for (int i = 0; i < dimension; i++)
    {
        for (int j = 0; j < dimension; j++)
        {
            if (j != i)
            {
                input_matrix[i][j] = 0;

                continue;
            }

            if (is_identity)
            {
                input_matrix[i][j] = 1;

                continue;
            }

            input_matrix[i][j] = rand_double();
        }
    }
}

/**
 * creates a square
 * matrix with no zero values
 * Used to make the calculation
 * more difficult
 */
void normal()
{
    for (int i = 0; i < dimension; i++)
    {
        for (int j = 0; j < dimension; j++)
        {
            input_matrix[i][j] = rand_double();
        }
    }
}

/**
 * Function created to 
 * print times and configuration
 * passed.
 */ 
void print_times(double init_delta, double exec_delta, double end_delta)
{
    double total = init_delta + exec_delta + end_delta;

    // printf("Threads: %d, Dimension: %d\nStart time: %lfs\nExecution time: %lfs\nFree Time: %lfs\nTotal Time: %lfs\n", n_threads, dimension, init_delta, exec_delta, end_delta, total);
    printf("%d, %d, %lf, %lf, %lf, %lf\n", n_threads, dimension, init_delta, exec_delta, end_delta, total);
}

/**
 * Parallel calculation
 * of the row or column
 * with most zero elements 
 * in the matrix.
 */ 
t_zeros_count *get_most_zeros(pthread_t *t_id, t_args *args)
{
    for (int i = 0; i < n_threads; i++)
    {
        args[i].t_id = i;
        // args[i].is_row = is_row;

        if (pthread_create(&t_id[i], NULL, count_zeros_task, (void *)(args + i)))
        {
            printf("Could not create thread no %d.\n", i);

            return NULL;
        }
    }

    t_zeros_count *most_zeros = malloc(sizeof(t_zeros_count));

    t_zeros_count *holder = malloc(sizeof(t_zeros_count));

    for (int i = 0; i < n_threads; i++)
    {
        pthread_join(t_id[i], (void **) &holder);

        if(i == 0 || holder->amount > most_zeros->amount)
        {
            most_zeros->index = holder->index;
            most_zeros->amount = holder->amount;
            most_zeros->is_row = holder->is_row;
        }
    }

    free(holder);

    return most_zeros;
}

/**
 * Calculating the determinant
 * with cofactor expansion
 * over the column or row 
 * with most zeros
 */
double *calculate_determinant(pthread_t *t_id, t_args *args)
{
    // Set line_pointer to zero
    line_pointer = 0;

    for (int i = 0; i < n_threads; i++)
    {
        args[i].t_id = i;

        if (pthread_create(&t_id[i], NULL, task, (void *)(args + i)))
        {
            printf("Could not create thread no %d.\n", i);

            return NULL;
        }
    }

    double *det = malloc(sizeof(double));
    *det = 0;

    double *holder = malloc(sizeof(double));

    for (int i = 0; i < n_threads; i++)
    {
        pthread_join(t_id[i], (void **) &holder);

        *det += *holder;
    }

    free(holder);

    return det;
}

/**
 * Validate the zeros count
 * in a tringular, diagonal or
 * identity matrix.
 */ 
int validate_most_zeros(t_zeros_count result)
{
    return result.amount == dimension - 1;
}

int main(int argc, char *argv[])
{
    // Function to make random integers more random :D
    srand(time(NULL));

    double start, end, init_delta, exec_delta, end_delta;

    pthread_t *t_id;
    t_args *args;

    // Input verification
    if (argc < 4)
    {
        printf("Insert %s <matrix_dimension> <matrix_type> <number_of_threads>\n", argv[0]);
        
        return 1;
    }

    dimension = atoi(argv[1]);
    matrix_type = atoi(argv[2]);
    n_threads = atoi(argv[3]);

    if (n_threads > dimension)
    {
        n_threads = dimension;
    }

    // Initialzie lock 
    if (pthread_mutex_init(&lock, NULL) != 0)
    {
        printf("Failed to initialize any lock.\n");

        return 1;
    }

    GET_TIME(start);

    // Memory allocation for matrix
    input_matrix = malloc_matrix(dimension);

    switch (matrix_type)
    {
        // upper triangular
        case 1:
            triangular(1);

            break;

        // lower triangular
        case 2:
            triangular(0);

            break;

        // identity matrix
        case 3:
            diagonal(1);

            break;

        // diagonal matrix
        case 4:
            diagonal(0);

            break;

        default:
            normal();

            break;
    }

    // Uncomment it to see the matrix created.
    // print_matrix(input_matrix, dimension);

    GET_TIME(end);

    init_delta = end - start;

    if (n_threads == 0)
    {
        GET_TIME(start);
        
        t_zeros_count zeros = get_zeros_count(input_matrix, dimension);

        // Uncomment to validate tringular, diagonal or identity matrix
        // char *str = zeros.is_row ? "row" : "column";
        // char *result = validate_most_zeros(zeros) ? "TRUE" : "FALSE";
        // printf("(%s) Found %d zeros at %s no. %d\n", result, zeros.amount, str, zeros.index + 1);

        double total = 0;

        int i, j;

        for (int x = 0; x < dimension; x++)
        {
            if (zeros.is_row)
            {
                i = zeros.index;
                j = x;
            }
            else
            {
                i = x;
                j = zeros.index;
            }

            double a = input_matrix[i][j];

            if (a == 0)
            {
                continue;
            }

            total +=  get_sign(i, j) * a * determinant(input_matrix, dimension, i, j);
        }

        // Uncomment to validate tringular, diagonal or identity matrix
        // result = validate_result(total) ? "TRUE" : "FALSE";
        // printf("(%s) Determinant: %lf\n", result, total);

        GET_TIME(end);

        exec_delta = end - start;

        end_delta = 0;

        print_times(init_delta, exec_delta, end_delta);

        return 0;
    }

    // Starting threads
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

    gt_line = get_most_zeros(t_id, args);

    // Uncomment to validate tringular, diagonal or identity matrix
    // char *str = gt_line->is_row ? "row" : "column";
    // char *result = validate_most_zeros(*gt_line) ? "TRUE" : "FALSE";
    // printf("(%s) Found %d zeros at %s no. %d\n", result, gt_line->amount, str, gt_line->index + 1);

    double *det = calculate_determinant(t_id, args);

    // Uncomment to validate tringular, diagonal or identity matrix
    // result = validate_result(*det) ? "TRUE" : "FALSE";
    // printf("(%s) Determinant: %lf\n", result, *det);

    GET_TIME(end);

    exec_delta = end - start;

    // Free memory
    GET_TIME(start);

    free_matrix(input_matrix, dimension);
    free(args);
    free(t_id);
    free(gt_line);

    pthread_mutex_destroy(&lock);

    GET_TIME(end);

    end_delta = end - start;

    print_times(init_delta, exec_delta, end_delta);

    return 0;
}