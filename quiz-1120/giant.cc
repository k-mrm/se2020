#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>
#include "debug.h"

#include "param.h"

pthread_mutex_t Lock;
typedef struct _OBJ {
  int key;
  int val;
} OBJ;

OBJ Obj[MAX_OBJ];

void 
transaction(void)
{
  pthread_mutex_lock(&Lock);
  for (int i = 0; i < NUM_ACCESS; i++) {
    int oid = i;
    Obj[oid].val = oid * 10;
  }
  pthread_mutex_unlock(&Lock);
}

void *
worker(void *arg)
{
  int myid = *(int *)arg;
  free(arg);
  for (int i = 0; i < NUM_TX; i++) {
    transaction();
  }
  return NULL;
}

void
print_diff(struct timeval begin, struct timeval end)
{
  long diff = (end.tv_sec - begin.tv_sec) * 1000 * 1000 + (end.tv_usec - begin.tv_usec);
  printf("%ld us\n", diff);
}

int
main(void)
{
  pthread_t thread[MAX_WORKER];
  pthread_mutex_init(&Lock, NULL);
  struct timeval begin, end;

  gettimeofday(&begin, NULL);
  for (int i = 0; i < MAX_WORKER; i++) {
    int *id = (int *)calloc(1, sizeof(int)); if(!id)ERR; *id = i;
    pthread_create(&thread[i], NULL, worker, id);
  }
  for (int i = 0; i < MAX_WORKER; i++) {
    pthread_join(thread[i], NULL);
  }
  gettimeofday(&end, NULL);
  print_diff(begin, end);

  return 0;
}
