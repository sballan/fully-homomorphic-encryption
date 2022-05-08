#include "discreet_logs_select_index.h"

#pragma hls_top
void selectIndex(
  char record[16+1], 
  char query[8], 
  char result[8+1]
) {
  // Return if record was already found
  if(result[0] != '\0'){
    result[8+1]++;         // This is meant to alter the ciphertext even though the plaintext doesn't change
    return;
  } 

  // Return if record is blank
  if(record[0] == '\0'){
    result[8+1]++;         
    return;
  } 

  #pragma hls_unroll yes
  for(int i=0; i<8; i++) {
    if(query[i] != record[i]) {
      result[8+1]++; 
      return;
    }
  }

  #pragma hls_unroll yes
  for(int i=0; i<8; i++) {
    result[i] = record[i+8];
  }
}
