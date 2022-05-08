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
#include "transpiler/examples/discreet_logs/discreet_logs_select_index_tfhe.h"
#include "transpiler/examples/discreet_logs/discreet_logs_count_tfhe.h"
#include "transpiler/examples/discreet_logs/discreet_logs_read_all_tfhe.h"
#include "transpiler/examples/discreet_logs/discreet_logs_insert_tfhe.h"
#include "transpiler/examples/discreet_logs/hangman_api_tfhe.h"
#include "transpiler/examples/discreet_logs/hangman_client.h"
#include "xls/common/logging/logging.h"


constexpr int kMainMinimumLambda = 120;

int main() {
  std::cout << "Starting..." << std::endl;

  std::cout << "Setting key parameters" << std::endl;
  // generate a keyset
  TFHEParameters params(kMainMinimumLambda);

  // generate a random key
  // Note: In real applications, a cryptographically secure seed needs to be
  // used.
  std::array<uint32_t, 3> seed = {314, 1592, 657};
  TFHESecretKeySet key(params, seed);

  std::cout << "Starting initial database handshake" << std::endl;

  char raw_record1[16] = {'K', 'e', 'y', 0, 0, 0, 0, 0, 'V', 'a', 'l', 0, 0, 0, 0, 0};
  char raw_record2[16] = {'M', 'y', 0, 0, 0, 0, 0, 0, 'C', 'o', 'n', 't', 'e', 'n', 't', 0};
  char raw_blank_record[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};


  // The order of these can be randomized so we don't know all blank ciphers are at the end.
  TfheString record1 = TfheString::Encrypt(raw_record1, key);
  TfheString record2 = TfheString::Encrypt(raw_record2, key);
  TfheString record3 = TfheString::Encrypt(raw_blank_record, key);
  TfheString record4 = TfheString::Encrypt(raw_blank_record, key);
  TfheString record5 = TfheString::Encrypt(raw_blank_record, key);
  
  
  std::vector<TfheString*> db;
  db.push_back(&record1);
  db.push_back(&record2);
  db.push_back(&record3);
  db.push_back(&record4);
  db.push_back(&record5);

  int db_size = 2;

  std::cout << "DB has " << db_size << " records." << std::endl;

  // records are seralized, sent to carol, and persisted.  all future accesses of records happen by carol

  std::cout << "Database handshake complete\n\n" << std::endl;

  std::cout << "Welcome to Discreet Logs!" << std::endl;
  
  int op_type;
  for(int i=0; i<5;i++) {
    op_type = -1;

    std::cout << "Please enter the operation type: \n" 
                 "1: Read all \n"
                 "2: Search \n"
                 "3: Insert \n"
                 "4: Count \n"
    << std::endl;

    std::cin >> op_type;

    if(op_type == 1) {
      int raw_counter[2] = {0, 0};
      char raw_result[32];
      for(int i=0;i<32;i++) {
        raw_result[i] = 0;
      }

      auto counter = TfheArray<int>::Encrypt(raw_counter, key);
      auto result = TfheString::Encrypt(raw_result, key);

      XLS_CHECK_OK(readAll(record1, counter, result, key.cloud()));
      XLS_CHECK_OK(readAll(record2, counter, result, key.cloud()));

      auto output = result.Decrypt(key);

      std::cout << output;
      std::cout << "\n" << std::endl;      
    } else if(op_type == 2) {
      char raw_result[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

      char input_chars[16];
      std::cout << "Please enter the query (max size 8 chars): \n"  << std::endl;
      
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      std::string input;
      std::cin >> input;
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

      std::cout << "*" << input << "*" << std::endl;

      for(int i=0; i<16; i++) {
        if(i < input.size() && input[i] != '\n') {
          input_chars[i] = input[i];
        } else {
          input_chars[i] = 0; 
        }      
      }
              
      auto query = TfheString::Encrypt(input_chars, key);
      auto result = TfheString::Encrypt(raw_result, key);

      // XLS_CHECK_OK(selectIndex(record1, query, result, key.cloud()));
      // XLS_CHECK_OK(selectIndex(record2, query, result, key.cloud()));

      for(int i=0; i<db_size;i++) {
        XLS_CHECK_OK(selectIndex(*db[i], query, result, key.cloud()));
      }

      auto output = result.Decrypt(key);

      std::cout << output;
      std::cout << "\n" << std::endl;
    } else if(op_type == 3) {
      int raw_result[2] = {0,0};
      char input_chars[16];
      std::cout << "Please enter the title (max size 8 chars): \n"  << std::endl;
      
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      std::string input;
      std::cin >> input;
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      // std::cout << "*" << input << "*" << std::endl;

      for(int i=0; i<8; i++) {
        if(i < input.size() && input[i] != '\n') {
          input_chars[i] = input[i];
        } else {
          input_chars[i] = 0; 
        }      
      }
      std::cout << "*" << input_chars << "*" << std::endl;

      std::cout << "Please enter the content (max size 8 chars): \n"  << std::endl;
      
      std::string input2;
      // std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      std::cin >> input2;
      // std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      // std::cout << "*" << input2 << "*" << std::endl;

      for(int i=0; i<8; i++) {
        if(i < input2.size() && input2[i] != '\n') {
          input_chars[i+8] = input2[i];
        } else {
          input_chars[i+8] = 0; 
        }      
      }

      std::cout << "*";
      for(int i=8; i<16; i++) {
        std::cout << input_chars[i];
      }
      std::cout << "*";
      std::cout << std::endl;
      
      auto query = TfheString::Encrypt(input_chars, key);
      auto result = TfheArray<int>::Encrypt(raw_result, key);

      XLS_CHECK_OK(insert(*db[db_size], query, result, key.cloud()));
      db_size++;

      auto output = result.Decrypt(key);
      std::cout << output[0];
      std::cout << output[1];
      std::cout << "\n" << std::endl;
    } else if(op_type == 4) {
      int raw_result[2] = {0, 0};

      char input_chars[16];
      std::cout << "Please enter the query (max size 8 chars), use * for wildcard: \n"  << std::endl;
      
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      std::string input;
      std::cin >> input;
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

      std::cout << "*" << input << "*" << std::endl;

      for(int i=0; i<16; i++) {
        if(i < input.size() && input[i] != '\n') {
          input_chars[i] = input[i];
        } else {
          input_chars[i] = 0; 
        }      
      }
              
      auto query = TfheString::Encrypt(input_chars, key);
      auto result = TfheArray<int>::Encrypt(raw_result, key);

      XLS_CHECK_OK(count(record1, query, result, key.cloud()));
      XLS_CHECK_OK(count(record2, query, result, key.cloud()));

      auto output = result.Decrypt(key);

      std::cout << output[0];
      std::cout << "\n" << std::endl;
    }
  }

  return 0;
}
