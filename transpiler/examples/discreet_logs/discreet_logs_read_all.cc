#include "discreet_logs_read_all.h"

#pragma hls_top
void readAll(
  char record[16], 
  int counter[2],
  char result[32]
) {
  #pragma hls_unroll yes
  for(int i=0; i<16; i++) {
    result[counter[0]+i] = record[i];
  }

  counter[0]+=16;
  counter[1]++;
}