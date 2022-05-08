#include "discreet_logs_insert.h"

#pragma hls_top
void insert(
  char record[INSERT_MAX+1], 
  char query[INSERT_MAX], 
  int result[1+1]
) {
  // Return if we already inserted
  if(result[0] == 1) {
    record[INSERT_MAX+1]++;
    result[1]++;
    return;
  }

  // Return if record is not blank
  if(record[0] != '\0') {
    record[INSERT_MAX+1]++;
    result[1]++;
    return;
  }

  #pragma hls_unroll yes
  for(int i=0; i<INSERT_MAX; i++) {
    record[i] = query[i];
  }

  result[0]=1;
}