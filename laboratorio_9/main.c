#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define N 5
#define P 10
#define C 10

int Buffer[N];

int count = 0, in = 0;
int chegaram = 0;

sem_t mutex, cond_empty, cond_full;

void init_buffer()
{
    for (int i = 0; i < N; i++)
    {
        Buffer[i] = 0;
    }
}

void print_buffer()
{
    char str[N*3];
    char buff[N*3];

    strcpy(str, "");

    for (int i = 0; i < N; i++)
    {
        sprintf(buff, "%d ", Buffer[i]);

        strcat(str, buff);
    }

    printf("%s \n", str);
}

// Producer
void insert(int item, int id)
{
    // sem_wait(&mutex);

    printf("P[%d] waits for space.\n", id);
    
    sem_wait(&cond_empty);

    printf("P[%d] continues.\n", id);

    sem_wait(&mutex);

    Buffer[in] = item;

    in = (in + 1) % N;

    count++;

    printf("P[%d] inserted an item.\n", id);

    print_buffer();

    if (count == N)
    {
        printf("P[%d] freed a consumer\n", id);

        sem_post(&cond_full);
    }

    sem_post(&mutex);
}

void remove_all(int id)
{
    sem_wait(&mutex);

    printf("C[%d] wants to remove\n", id);

    if(count < N) // while its not full 
    {
        printf("C[%d] blocked\n", id);

        sem_post(&mutex);
        
        sem_wait(&cond_full);

        printf("C[%d] freed\n", id);
    }

    init_buffer(); // remove all values;
    
    print_buffer();

    count = 0;

    printf("C[%d] removed all\n", id);

    for (int i = 0; i < N; i++)
    {
        sem_post(&cond_empty);

        printf("C[%d] freed a space (x%d)\n", id, i);
    }

    sem_post(&mutex);
}

void *producer(void *arg)
{
    int *id = (int *)arg;

    printf("producer no. %d has started.\n", *id);

    while(1)
    {
        insert(*id, *id);

        sleep(1);
    }

    free(arg);

    pthread_exit(NULL);
}

void *consumer(void *arg)
{
    int *id = (int *)arg;

    printf("consumer no. %d has started.\n", *id);

    while(1)
    {
        remove_all(*id);

        sleep(1);
    }

    free(arg);

    pthread_exit(NULL);
}

int main()
{
    sem_init(&mutex, 0, 1);
    sem_init(&cond_empty, 0, N);
    sem_init(&cond_full, 0, 0);

    pthread_t tid[P + C];
    int *id[P + C];

    init_buffer();
    print_buffer();

    for (int i = 0; i < P + C; i++)
    {
        id[i] = malloc(sizeof(int));
        *id[i] = i + 1;
    }

    for (int i = 0; i < P; i++)
    {
        if (pthread_create(&tid[i], NULL, producer, (void *)id[i]) < 0)
        {
            exit(-1);
        }
    }

    for (int i = 0; i < C; i++)
    {
        if (pthread_create(&tid[i + P], NULL, consumer, (void *)id[i + P]) < 0)
        {
            exit(-1);
        }
    }
    

    pthread_exit(NULL);

    return 1;
}
