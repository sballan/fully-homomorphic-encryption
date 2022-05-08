#include "discreet_logs_insert.h"

#pragma hls_top
void insert(
  char record[16], 
  char query[16], 
  int result[2]
) {
  if(record[0] != 0) {
    result[1]++;
    return;
  }

  #pragma hls_unroll yes
  for(int i=0; i<16; i++) {
    record[i] = query[i];
  }

  result[0]++;
}