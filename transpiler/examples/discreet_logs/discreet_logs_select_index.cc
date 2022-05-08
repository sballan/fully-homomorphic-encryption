#include "discreet_logs_select_index.h"

#pragma hls_top
void selectIndex(
  char record[MAX_SIZE], 
  char query[MAX_SIZE], 
  char result[MAX_SIZE/2 + 1]
) {
  if(result[0] != '\0'){
    result[9]++;         // This is meant to alter the ciphertext even though the plaintext doesn't change
    return;
  } 

  #pragma hls_unroll yes
  for(int i=0; i<MAX_SIZE/2; i++) {
    if(query[i] != record[i]) {
      result[9]++; 
      return;
    }
  }

  #pragma hls_unroll yes
  for(int i=MAX_SIZE/2; i<MAX_SIZE; i++) {
    result[i - MAX_SIZE/2] = record[i];
  }
  result[9] = 0;
}
