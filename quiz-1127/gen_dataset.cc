#include <stdio.h>
#include <stdlib.h>
#include "param.h"

int
main(void)
{
  FILE *fpout;
  fpout = fopen(OPR_FILE, "w+");
  int key;
  for (int w = 0; w < MAX_WORKER; w++) {
    for (int tx = 0; tx < NUM_TX; tx++) {
      key = 0;
      for (int opr = 0; opr < NUM_ACCESS; opr++) {
        key += rand() % 10 + 1;
        fprintf(fpout, "%d:%d:%d:%d\n", w, tx, opr, key);
      }
    }
  }
  fclose(fpout);

  return 0;
}
