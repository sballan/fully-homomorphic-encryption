// Copyright 2021 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "hangman_api.h"

#pragma hls_top
void hangmanMakeMove(
  char db[MAX_ARRAY_SIZE], 
  int db_idx[MAX_ARRAY_SIZE], 
  char query[MAX_ARRAY_SIZE], 
  int query_params[3], 
  char result[MAX_ARRAY_SIZE]
) {
  return;
  if(query_params[0] == 1) {
    #pragma hls_unroll yes
    for(int i=0; i<MAX_ARRAY_SIZE; i++) {
      result[i] = db[i];
    }
  } else if(query_params[0] == 2) {
    // When we start, we know that the key starts at 0
    int key_start = 0;
    bool value_found = false;
    int value_end = 0;
    int cursor = 0;
    // Iterate over the db_idx, to look at keys
    #pragma hls_unroll yes
    for(int i=0; i<4; i+=2) {
      // We use -1 in the db_idx to indicate null values
      if(db_idx[i] == -1) break;

      int key_end = db_idx[i];
      int value_end = db_idx[i+1];
      int value_cursor = 0;
      cursor = key_start;

      // We use this loop to both match a key, and then also to copy the value elements
      #pragma hls_unroll yes
      for(int j=0; j<4; j+=1) {
        // Break if we know we'll read empty data (and seg fault)
        if (j >= query_params[1]) break;

        if(value_found) {
          result[value_cursor] = db[cursor];
          value_cursor++;
        } else if(query[j] != db[cursor]){
          // In this condition, a character didn't match the query.
          // Discard this key comparison, and go to the next key
          key_start = value_end;
          break;
        } else if(cursor == key_end-1) {
          value_found = true;
          break;
        }         
        cursor+=1;    
      }
      if(value_found) break;
    }

    // result[0] = cursor;

    
    // if(value_found){
    //   #pragma hls_unroll yes
    //   for(int i=0; i<MAX_ARRAY_SIZE; i+=1) {
    //     if(cursor == value_end) break;
    //     result[i] = db[cursor] ;
    //     cursor+=1;
    //   }
    // } 
  }
}
