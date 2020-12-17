#include "kr.h"

bool
validation(const int nread, const int readset[], const int tbegin, const int tend)
{
  // Find the start point
  list<XACT>::iterator xitr;
  for (xitr = XactHist.begin(); xitr != XactHist.end(); xitr++) {
    if (xitr->tid == tbegin) {
      xitr++;
      break;
    }
  }

  // Do Validation
  for (;xitr != XactHist.end() && xitr->tid <= tend; xitr++) {
    for (int wid = 0; wid < xitr->nwrite; wid++) {
      for (int rid = 0; rid < nread; rid++) {
        if (xitr->writeset[wid] == readset[rid]) { // Conflict
          return false;
        }
      }
    }
  }

  return true; // Success
}

void 
transaction(int myid)
{
  LOCAL_DB local_db[NUM_ACCESS]; // local db
  int key_list[NUM_ACCESS];
  int tbegin = Tid;
  int readset[NUM_ACCESS];
  int nread;

  // Set access pattern
  for (int i = 0; i < NUM_ACCESS; i++) {
    key_list[i] = myid * 100 + i ;
  }

 RETRY:
  // Read Phase
  for (int i = 0; i < NUM_ACCESS; i++) {
    local_db[i].obj.key = key_list[i];
    local_db[i].obj.val = Global_db[key_list[i]].val;
    local_db[i].mode = READ; // No write (read-only)
  }

  // Set readset
  nread = 0;
  for (int i = 0; i < NUM_ACCESS; i++) {
    if (local_db[i].mode == READ) {
      readset[nread++] = local_db[i].obj.key;
    }
  }

  // Local modification
  for (int i = 0; i < NUM_ACCESS; i++) {
    if (local_db[i].mode == WRITE) { // Modify
      local_db[i].obj.val += 3;
    }
  }

  // Validation & Write phase
  pthread_mutex_lock(&GiantLock);
  int tend = Tid;  
  if (Tid != 0) {
    bool valid = validation(nread, readset, tbegin, tend);
    if (valid == false){
      //
      // Task! Write your code here
      // [unlock and retry]
      // 
      pthread_mutex_unlock(&GiantLock);
      goto RETRY;
    }
  }

  // Write phase
  for (int i = 0; i < NUM_ACCESS; i++){
    if (local_db[i].mode == WRITE) {
      Global_db[i].val = local_db[i].obj.val;
    }
  }

  // Increment transaction ID
  Tid = Tid + 1;

  // Register myself
  XACT me;
  me.nwrite = 0;
  me.tid = Tid;
  for (int i = 0; i < NUM_ACCESS; i++){
    if (local_db[i].mode == WRITE){
      me.writeset[me.nwrite++] = i;
    }
  }
  XactHist.push_back(me);
  pthread_mutex_unlock(&GiantLock);    
}

void *
worker(void *arg)
{
  int myid = *(int *)arg;
  free(arg);
  for (int i = 0; i < NUM_TX; i++) {
    transaction(myid);
  }
  return NULL;
}

void
init(void)
{
  pthread_mutex_init(&GiantLock, NULL);
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
