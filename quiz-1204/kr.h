// Kung-Robinson
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>
#include <list>
#include "debug.h"
#include "param.h"
using namespace std;
typedef enum {READ, WRITE} ACCESS_MODE;
// pthread_mutex_t Lock;

typedef struct _OBJ {
  pthread_mutex_t lock;  
  int key;
  int val;
} OBJ;
OBJ Global_db[MAX_OBJ];

typedef struct _LOCAL_DB {
  ACCESS_MODE mode; // read or write
  OBJ obj;
} LOCAL_DB;

typedef struct _XACT {
  int tid;
  int nwrite;
  int writeset[NUM_ACCESS];
} XACT;

list<XACT> XactHist;
int Tid = 0;
pthread_mutex_t GiantLock;
