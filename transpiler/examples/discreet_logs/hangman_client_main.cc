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

  std::string input;
  getline(std::cin, input);

  database_titles = ""
  index_titles = {}
  database_contents = ""
  index_contents = {}

  indexes = {0, 21, 31, 45}

  query = ["My new note", "This is my amazing note"]

  database_titles += query[0]
  index_title.insert(database_titles.length())

  // after
  database_titles = "My new note"
  index_titles = {11}




  // std::array<std::string, 2> input_array = {input, input};

  // std::vector<int32_t> int_vector = {1, 2, 3};
  // std::vector<std::vector<char>> string_vector = {"hello", "world"};

  // auto int_array0 = TfheArray<std::vector<char>>::Encrypt(string_vector, key);

  // auto int_array1 = TfheArray<int32_t>::Encrypt(int_vector, key);

  // auto int_array = TfheArray<int32_t>::Encrypt({1, 2, 3}, key);

  // auto ciphertext = TfheArray<int>::Encrypt(int_vector, key)


  // auto ciphertext = TfheStringArray::Encrypt(input_array, key);
  

  auto ciphertext = TfheString::Encrypt(input, key);
  // std::cout << "Encryption done" << std::endl;

  // std::cout << "Initial state check by decryption: " << std::endl;
  // std::cout << ciphertext.Decrypt(key) << "\n";

  // int32_t output[3] = {};
  auto int_array3 = TfheArray<int32_t>::Encrypt({1, 2, 3, 4}, key);
  auto decoded = int_array3.Decrypt(key);
  for (int i = 0; i < 4; i++) {
    std::cout << decoded[i] << "\n";
  }
  // std::cout << int_array1.Decrypt(key) << "\n";
  std::cout << "\n";

  // Make a move.
  // TfheInt cipher_result(params);

  // XLS_CHECK_OK(hangmanMakeMove(cipher_result, ciphertext, key.cloud()));

  // int move_result = cipher_result.Decrypt(key);


  return 0;
}
