#include "discreet_logs_count.h"

#pragma hls_top
void count(
  char record[16+1], 
  char query[8], 
  int result[1+1]
) {
  if(record[0] == '\0') {
    result[1]++;
    return;
  }

  #pragma hls_unroll yes
  for(int i=0; i<16/2; i++) {
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