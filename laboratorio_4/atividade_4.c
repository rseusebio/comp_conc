/* Disciplina: Computacao Concorrente */
/* Aluno: Rafael Santos Eusébio */
/* Laboratório: 4 */
/* Atividade: 4 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NTHREADS  4

/* Variaveis globais */
int *ids[NTHREADS];
int x = 0, y = 0, z = 0;
pthread_mutex_t x_mutex, y_mutex, z_mutex;
pthread_cond_t first_cond;  // fazer uma lista
pthread_cond_t last_cond_1; 
pthread_cond_t last_cond_2;

void *func (void *t) 
{
  int *id = (int *) t;

  switch(*id)
  {
    case 1:
    {
      pthread_mutex_lock(&x_mutex);
      if (x == 0)
      {
        pthread_cond_wait(&first_cond, &x_mutex);
      }
      pthread_mutex_unlock(&x_mutex);
    
      printf("Fique a vontade.\n");

      pthread_cond_signal(&last_cond_1);

      pthread_mutex_lock(&y_mutex);
      y++;
      pthread_mutex_unlock(&y_mutex);

      break;
    }
    case 2:
    {
      printf("Seja bem-vindo!\n");

      pthread_mutex_lock(&x_mutex);
      x++;
      pthread_mutex_unlock(&x_mutex);

      pthread_cond_broadcast(&first_cond);

      break;
    }
    case 3:
    {
      pthread_mutex_unlock(&y_mutex);
      if (y == 0)
      {
        pthread_cond_wait(&last_cond_1, &y_mutex);
      }
      pthread_mutex_unlock(&y_mutex);

      pthread_mutex_unlock(&z_mutex);
      if (z == 0)
      {
        pthread_cond_wait(&last_cond_2, &z_mutex);
      }
      pthread_mutex_unlock(&z_mutex);

      printf("Volte sempre!\n");

      break;
    }
    case 4:
    {
      pthread_mutex_lock(&x_mutex);

      if (x == 0)
      {
        pthread_cond_wait(&first_cond, &x_mutex);
      }

      pthread_mutex_unlock(&x_mutex);
    
      printf("Sente-se por favor.\n");

      pthread_mutex_lock(&z_mutex);
      z++;
      pthread_mutex_unlock(&z_mutex);
      
      pthread_cond_signal(&last_cond_2);

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

  /* Inicilaiza o mutex (lock de exclusao mutua) e a variavel de condicao */
  pthread_mutex_init(&x_mutex, NULL);
  pthread_mutex_init(&y_mutex, NULL);
  pthread_mutex_init(&z_mutex, NULL);

  pthread_cond_init(&first_cond, NULL);
  pthread_cond_init(&last_cond_1, NULL);
  pthread_cond_init(&last_cond_2, NULL);

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
  pthread_mutex_destroy(&x_mutex);
  pthread_mutex_destroy(&y_mutex);
  pthread_mutex_destroy(&z_mutex);

  pthread_cond_destroy(&first_cond);
  pthread_cond_destroy(&last_cond_1);
  pthread_cond_destroy(&last_cond_2);
}
