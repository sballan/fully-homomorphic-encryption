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
void hangmanMakeMove(char database[4], char query[1], char result[1]) {
#pragma hls_unroll yes
  for (int i = 0; i < 4; i+=2) {
    if(query[0] == database[i]) {
      result[0] = database[i+1];
    }
  }
}
