#include "discreet_logs_read_all.h"

#pragma hls_top
void readAll(
  char record[16+1], 
  int counter[1+1],
  char result[32+1]
) {
  if(record[0] == '\0') {
    record[16+1]++;
    counter[1]++;
    result[32+1]++;
    return;
  }

  #pragma hls_unroll yes
  for(int i=0; i<16; i++) {
    result[counter[0]+i] = record[i];
  }

  counter[0]+=16;
  counter[1]++;
}