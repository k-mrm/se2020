#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>
#include "debug.h"
#include "param.h"

//pthread_mutex_t Lock;
typedef struct _OBJ {
  pthread_mutex_t lock;  
  int key;
  int val;
} OBJ;
OBJ Obj[MAX_OBJ];

void 
transaction(int wid, int tx)
{
  int history[NUM_ACCESS];

  for( int i = 0; i < NUM_ACCESS; i++) {
    //int oid = myid * 1000 + i;
    int oid = ComWorker[wid].tx[tx].opr[i];
    pthread_mutex_lock(&Obj[oid].lock);
    Obj[oid].val = oid * 10;
    //usleep(1);
    history[i] = oid;
  }

  // Shrinking phase
  for( int i = 0; i < NUM_ACCESS; i++) {
    pthread_mutex_unlock(&Obj[history[i]].lock);
  }
}

void *
worker(void *arg)
{
  int wid = *(int *)arg;
  free(arg);
  for (int txid = 0; txid < NUM_TX; txid++) {
    transaction(wid, txid);
  }
  return NULL;
}

void
init(void)
{
  for( int i = 0; i < MAX_OBJ; i++) {
    pthread_mutex_init(&Obj[i].lock, NULL);
  }

  FILE *fpin;
  char buff[BUFSIZ];
  fpin = fopen(OPR_FILE, "r");
  while(true){
    int w, tx, opr, obj;
    if (!fgets(buff, sizeof(buff), fpin)) break;
    sscanf(buff, "%d:%d:%d:%d", &w, &tx, &opr, &obj);
    ComWorker[w].tx[tx].opr[opr] = obj;
  }
  fclose(fpin);
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
  init();
  pthread_t thread[MAX_WORKER];
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
