#include "discreet_logs_insert.h"

#pragma hls_top
void insert(
  char record[16+1], 
  char query[16], 
  int result[1+1]
) {
  // Return if we already inserted
  if(result[0] == 1) return;

  // Return if record is not blank
  if(record[0] != '\0') {
    record[16+1]++;
    result[1]++;
    return;
  }

  #pragma hls_unroll yes
  for(int i=0; i<16; i++) {
    record[i] = query[i];
  }

  result[0]=1;
}