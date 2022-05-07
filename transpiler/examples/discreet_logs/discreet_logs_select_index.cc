#include "discreet_logs_select_index.h"

#pragma hls_top
void selectIndex(
  char db[MAX_ARRAY_SIZE], 
  short db_idx[MAX_ARRAY_SIZE], 
  char query[MAX_ARRAY_SIZE], 
  short query_params[2], 
  short result[1]
) {
  // Which db index are we on
  short current_db_index=0;
  short cursor = 0;
  short key_end = db_idx[current_db_index];

  short found_index = -1;

  // Iterate over the db_idx, to look at keys
#pragma hls_unroll yes
  for(short i=0; i<MAX_ARRAY_SIZE; i++) {
    // We use -1 in the db_idx to indicate null values
    // if(db_idx[current_db_index] == -1) break;

    // Break if we know we'll read empty data (and seg fault)
    // if (cursor >= query_params[1]) break;

    // result[0] += 1;

    if(query[i] == 'a'){
      // In this condition, a character didn't match the query.
      // Discard this key comparison, and go to the next key
      // current_db_index += 2;
      // if(db_idx[current_db_index] == -1) break;

      // cursor = db_idx[current_db_index];
      // key_end = db_idx[current_db_index+1];
      result[0] = i;
      cursor++;
    } else if (query[i] == 'b'){
      break;
    }
    
  }

  // result[0] = cursor;
}
