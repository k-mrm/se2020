#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <atomic>
#include <sys/time.h>
#include <pthread.h>
#include "debug.h"
#include "param.h"

//pthread_mutex_t Lock;
typedef struct _OBJ {
  int key;
  int val;
  std::atomic<uint> mutex;
} OBJ;
OBJ Obj[MAX_OBJ];

static constexpr uint LockBit = 0x01;
static constexpr uint UnlockBit = 0x00;

bool my_lock_core(const uint id) 
{
  uint lock = Obj[id].mutex.load();
  if(lock == LockBit) {
    return false;
  }
  return Obj[id].mutex.compare_exchange_strong(lock, LockBit);
}

void my_unlock(const uint id) 
{ 
  Obj[id].mutex.store(UnlockBit);
}

void my_lock(const uint id) 
{
  while(true) {
    bool ok = my_lock_core(id);
    if(ok)
      break;
  }
}

void 
transaction(int wid, int tx)
{
  int history[NUM_ACCESS];

  // Growing phase
  for (int i = 0; i < NUM_ACCESS; i++) {
    int oid = ComWorker[wid].tx[tx].opr[i];
    my_lock(oid);
    Obj[oid].val = oid * 10;
    history[i] = oid;
  }

  // Shrinking phase
  for (int i = 0; i < NUM_ACCESS; i++) {
    int oid = history[i];
    my_unlock(oid);
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
    Obj[i].mutex.store(UnlockBit);
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
