#include "discreet_logs_select_index.h"

#pragma hls_top
void count(
  char record[MAX_SIZE], 
  char query[MAX_SIZE/2], 
  int result[2]
) {
  #pragma hls_unroll yes
  for(int i=0; i<MAX_SIZE/2; i++) {
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
