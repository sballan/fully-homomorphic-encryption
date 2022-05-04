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

const int MAX_INCORRECT_ATTEMPTS = 6;
const int MAX_WORD_LENGTH = 7;
const int CORRECT_RESULT = 127;

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

  auto database = TfheString::Encrypt("ab", key);
  auto db_idx = TfheArray<int32_t>::Encrypt({1,2}, key);
  // auto query = TfheString::Encrypt("b", key);
  auto query_type = TfheInt::Encrypt(1, key);
  auto query_length = TfheInt::Encrypt(1, key);

  std::string input;
  getline(std::cin, input);

  char input_chars[2];
  for(int i=0; i<2 && i<input.size(); i++) {
    input_chars[i] = input[i];
  }

  auto query = TfheString::Encrypt(input_chars, key);

  TfheString cipher_result = {2, params};

  XLS_CHECK_OK(
    hangmanMakeMove(
      database, 
      db_idx, 
      query_type,
      query, 
      query_length,
      cipher_result, 
      key.cloud())
      );
 
  auto result = cipher_result.Decrypt(key);

  std::cout << result << "\n";
  std::cout << "\n";

  return 0;
}
