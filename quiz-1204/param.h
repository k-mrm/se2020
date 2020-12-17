#define MAX_WORKER (3)
#define MAX_OBJ (1000*1000)
#define NUM_ACCESS (10)
#define NUM_TX (20)
#define OPR_FILE "OPERATION_FILE"

typedef struct _COM_TX {
  int opr[NUM_ACCESS]; // access object id
} COM_TX;

typedef struct _COM_WORKER {
  COM_TX tx[NUM_TX];
} COM_WORKER;

COM_WORKER ComWorker[MAX_WORKER];
