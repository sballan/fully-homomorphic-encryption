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
  db[0] = 'a';
  db_idx[0] = 0;
  query[0] = 'a';
  result[0] = 'k';
}
