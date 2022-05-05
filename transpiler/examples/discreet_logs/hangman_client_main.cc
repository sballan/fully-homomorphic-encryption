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

// The main method was split off from the rest of the code to allow testing.
#include <stdlib.h>

#include <bitset>
#include <cassert>
#include <codecvt>
#include <iostream>
#include <locale>
#include <string>

#include "transpiler/data/tfhe_data.h"
#include "transpiler/examples/discreet_logs/hangman_api_tfhe.h"
#include "transpiler/examples/discreet_logs/hangman_client.h"
#include "xls/common/logging/logging.h"


constexpr int kMainMinimumLambda = 120;

int main() {
  // generate a keyset
  TFHEParameters params(kMainMinimumLambda);

  // generate a random key
  // Note: In real applications, a cryptographically secure seed needs to be
  // used.
  std::array<uint32_t, 3> seed = {314, 1592, 657};
  TFHESecretKeySet key(params, seed);

  std::cout << "Welcome to Discreet Logs" << std::endl;

  char raw_db[MAX_ARRAY_SIZE];
  for(int i=0; i<MAX_ARRAY_SIZE; i++) {
    raw_db[i] = 0;
  }

  int raw_db_idx[MAX_ARRAY_SIZE];
  for(int i=0; i<MAX_ARRAY_SIZE; i++) {
    raw_db_idx[i] = -1;
  }

  int raw_query_params[3];
  for(int i=0; i<3; i++) {
    raw_query_params[i] = -1;
  }

  char raw_result[MAX_ARRAY_SIZE];
  for(int i=0; i<MAX_ARRAY_SIZE; i++) {
    raw_result[i] = 0;
  }

  auto db = TfheString::Encrypt(raw_db, key);
  auto db_idx = TfheArray<int32_t>::Encrypt(raw_db_idx, key);
  auto query_params = TfheArray<int32_t>::Encrypt(raw_query_params, key);
  auto result = TfheString::Encrypt(raw_result, key);

  while(true){
    std::string input;
    getline(std::cin, input);

    char input_chars[MAX_ARRAY_SIZE];
    for(int i=0; i<MAX_ARRAY_SIZE && i<input.size(); i++) {
      input_chars[i] = input[i];
    }

    auto query = TfheString::Encrypt(input_chars, key);

    // TfheString result = {MAX_ARRAY_SIZE, params};

    XLS_CHECK_OK(
      hangmanMakeMove(
        db, 
        db_idx, 
        query, 
        query_params,
        result, 
        key.cloud())
        );
  
    auto output = result.Decrypt(key);

    std::cout << output << "\n";
    std::cout << "\n";
  }


  return 0;
}
