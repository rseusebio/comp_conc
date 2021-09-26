/* Disciplina: Computacao Concorrente */
/* Aluno: Rafael Santos Eusébio */
/* Laboratório: 8 */

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

#define NTHREADS  4

/* Variaveis globais */
int *ids[NTHREADS];
sem_t condt1, condt2, condt4;

void *func (void *t) 
{
  int *id = (int *) t;

  switch(*id)
  {
    case 1:
    {
      
      sem_wait(&condt2);

      printf("Fique a vontade.\n");

      sem_post(&condt1);

      break;
    }
    case 2:
    {
      printf("Seja bem-vindo!\n");

      sem_post(&condt2); // libera a thread 1 e 4 (nao importa a ordem)
      sem_post(&condt2);

      break;
    }
    case 3:
    {
      
      sem_wait(&condt1);

      sem_wait(&condt4);

      printf("Volte sempre!\n");

      break;
    }
    case 4:
    {
      sem_wait(&condt2);

      printf("Sente-se por favor.\n");

      sem_post(&condt4);

      break;
    }
    default:
    {
      exit(1);

      break;
    }
  }

  pthread_exit(NULL);
}

/* Funcao principal */
int main(int argc, char *argv[]) 
{
  int i; 
  pthread_t threads[NTHREADS];

  sem_init(&condt1, 0, 0);
  sem_init(&condt2, 0, 0);
  sem_init(&condt4, 0, 0);
  // printf("here\n");

  for (i = 0; i < NTHREADS; i++)
  {
    ids[i] = malloc(sizeof(int));

    *ids[i] = i + 1;
  }

  // printf("here 2\n");

  /* Cria as threads */
  for (i = 0; i < NTHREADS; i++)
  {
    pthread_create(&threads[i], NULL, func, (void *) ids[i]);
  }

  // printf("here 3\n");

  /* Espera todas as threads completarem */
  for (i = 0; i < NTHREADS; i++) 
  {
    pthread_join(threads[i], NULL);
  }

  printf ("\nFIM\n");

  /* Desaloca variaveis e termina */
  sem_destroy(&condt1);
  sem_destroy(&condt2);
  sem_destroy(&condt4);
}