#include "hangman_api.h"

// Rules:
// database contains title and contents in a continuous sequence of pairs. 
// idxs contain indexes of where the titles and note contents start and stop.
// The first integer in idxs indicates where the first title stops.  The second
// integer indicates where the first content stops.
//
// all arrays must have a fixed size known at compile time.  All loops must have
// a known number of iterations at compile time.

#pragma hls_top
void hangmanMakeMove(char database[8], int idxs[8], char query[8], char result[8]) {
  #pragma hls_unroll yes
  for (int i = 0; i < 8; i+=2) {
    // We use -1 to signify the end of this Array
    if(idxs[i] == -1) break;

    bool found = false;
    int current_idx = idxs[i];
    int end_idx = idxs[i+1];

    #pragma hls_unroll yes
    // for (int j = idxs[i]; j < idxs[i+1]; j+=1) {
    for (int j = 0; j < 8; j+=1) {
      if(current_idx >= end_idx) break;

      if (query[current_idx] == database[current_idx]) {
        if(current_idx==end_idx) {
          found = true;
          break;
        }
        else {
          current_idx+=1;
          continue;
        }
      } else break;    
    }

    if(found) {

    }

  }
}
