#include <stdio.h>
#include <stdlib.h>
#include "param.h"

int
main(void)
{
  FILE *fpout;
  fpout = fopen(OPR_FILE, "w+");

  for( int w = 0; w < MAX_WORKER; w++) {
    for( int tx = 0; tx < NUM_TX; tx++) {
      for( int opr = 0; opr < NUM_ACCESS; opr++) {
        int obj = rand() % MAX_OBJ;
        fprintf(fpout, "%d:%d:%d:%d\n", w, tx, opr, obj);
      }
    }
  }
  fclose(fpout);

  return 0;
}
