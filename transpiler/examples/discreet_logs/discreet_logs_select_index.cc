#include "discreet_logs_select_index.h"

#pragma hls_top
void selectIndex(
  char record[SELECT_INDEX_MAX+1], 
  char query[SELECT_INDEX_MAX/2], 
  char result[SELECT_INDEX_MAX/2+1]
) {
  // Return if record was already found
  if(result[0] != '\0'){
    result[SELECT_INDEX_MAX/2+1]++;         // This is meant to alter the ciphertext even though the plaintext doesn't change
    return;
  } 

  // Return if record is blank
  if(record[0] == '\0'){
    result[SELECT_INDEX_MAX/2+1]++;         
    return;
  } 

  #pragma hls_unroll yes
  for(int i=0; i<SELECT_INDEX_MAX/2; i++) {
    if(query[i] != record[i]) {
      result[SELECT_INDEX_MAX/2+1]++; 
      return;
    }
  }

  #pragma hls_unroll yes
  for(int i=0; i<SELECT_INDEX_MAX/2; i++) {
    result[i] = record[i+SELECT_INDEX_MAX/2];
  }
}
