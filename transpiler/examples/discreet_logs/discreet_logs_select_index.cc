#include "discreet_logs_select_index.h"

#pragma hls_top
void selectIndex(
  char db[MAX_ARRAY_SIZE], 
  int db_idx[MAX_ARRAY_SIZE], 
  char query[MAX_ARRAY_SIZE], 
  int query_params[2], 
  int result[1]
) {
  // When we start, we know that the key starts at 0
  int key_start = 0;
  int value_end = 0;
  int cursor = 0;
  // Iterate over the db_idx, to look at keys
#pragma hls_unroll yes
  for(int i=0; i<MAX_ARRAY_SIZE; i+=2) {
    // We use -1 in the db_idx to indicate null values
    if(db_idx[i] == -1) break;

    int key_end = db_idx[i];
    int value_end = db_idx[i+1];
    cursor = key_start;

    #pragma hls_unroll yes
    for(int j=0; j<MAX_ARRAY_SIZE; j+=1) {
      // Break if we know we'll read empty data (and seg fault)
      // if (j >= query_params[1]) break;

      if(query[j] != db[cursor]){
        // In this condition, a character didn't match the query.
        // Discard this key comparison, and go to the next key
        key_start = value_end;
        break;
      } else if(cursor == key_end-1) {
        // result[0]=i;
        continue;
      }
      cursor+=1;    
    }
  }
}
