#include "discreet_logs_read_all.h"

#pragma hls_top
void readAll(
  char record[READ_ALL_MAX+1], 
  int counter[1+1],
  char result[READ_ALL_COUNT*READ_ALL_COUNT+1]
) {
  // Return if record is blank
  if(record[0] == '\0') {
    record[READ_ALL_COUNT+1]++;
    counter[1]++;
    result[READ_ALL_COUNT*READ_ALL_COUNT+1]++;
    return;
  }

  #pragma hls_unroll yes
  for(int i=0; i<READ_ALL_MAX; i++) {
    result[counter[0]+i] = record[i];
  }

  counter[0]+=READ_ALL_MAX;
  counter[1]++;
}