#include "discreet_logs_count.h"

#pragma hls_top
void count(
  char record[COUNT_MAX+1], 
  char query[COUNT_MAX/2], 
  int result[1+1]
) {
  // Return if record is blank
  if(record[0] == '\0') {
    result[1]++;
    return;
  }

  #pragma hls_unroll yes
  for(int i=0; i<COUNT_MAX/2; i++) {
    if(query[i] != record[i]) {
      if(query[i] == '*') {
        result[0]++;
      } else {
        result[1]++;  // This is meant to alter the ciphertext even though the plaintext doesn't change
      }
      return;
    }
  }

  result[0]++;
}