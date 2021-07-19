#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>

#define NTHREADS  2
#define SIZE 10000

int vector[SIZE];

typedef struct 
{
   int id;
} t_args;

void* power_of_two (void *arg) 
{
  t_args *args = (t_args *) arg;

  int index, finish;

  if (args->id == 0)
  {
      index = 0;
      finish = SIZE / 2;
  }
  else 
  {
     index = (SIZE / 2);
     finish = SIZE;
  }

  for(; index < finish; index++)
  {
      vector[index] *= vector[index];
  }

  pthread_exit(NULL);
}

int main() 
{
  pthread_t tid[NTHREADS]; 
  t_args *arg;
  int i;

  for (i = 0; i < SIZE; i++)
  {
      vector[i] = i + 1;
  }

  for (i = 0; i < NTHREADS; i++)
  {
      arg = malloc(sizeof(t_args));
      if (arg == NULL)
      {
          printf("arg malloc failed!\n");

          exit(-1);
      }

      arg->id = i;
      
      if (pthread_create(&tid[i], NULL, power_of_two, (void *)arg))
      {
          printf("pthread_create failed\n");

          exit(-1);
      }
  }

  for (i = 0; i < NTHREADS; i++)
  {
      if (pthread_join(tid[i], NULL))
      {
          printf("pthread_join failed\n");

          exit(-1);
      }
  }

  for (i = 0; i < SIZE; i++)
  {
      if (vector[i] != (i + 1) * (i + 1))
      {
          printf("power_of_two failed at position %d\n", i);
      }

    //   printf("original: %d; to the power of 2: %d\n", i+1, vector[i]);
  }

  printf("Main thread finished\n");

  pthread_exit(NULL);
}
