#include "discreet_logs_select_index.h"

#pragma hls_top
void selectIndex(
  char record[MAX_SIZE], 
  char query[MAX_SIZE], 
  int result[1]
) {
  if(result[0] == 1){
    result[0] = 1;    // This is meant to alter the ciphertext even though the plaintext doesn't change
    return;
  } 

#pragma hls_unroll yes
  for(int i=0; i<3; i++) {
    if(query[i] != record[i]) {
      result[0] = 0;
      return;
    }
  }

  result[0] = 1;
}
