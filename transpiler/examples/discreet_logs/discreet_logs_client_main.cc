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
#include "transpiler/examples/discreet_logs/discreet_logs_select_index.h"
#include "transpiler/examples/discreet_logs/discreet_logs_count.h"
#include "transpiler/examples/discreet_logs/discreet_logs_read_all.h"
#include "transpiler/examples/discreet_logs/discreet_logs_insert.h"
#include "xls/common/logging/logging.h"


// Length of the keys
constexpr int kMainMinimumLambda = 128;

int main() {
  std::cout << "Starting..." << std::endl;

  std::cout << "Setting key parameters" << std::endl;
  // generate a keyset
  TFHEParameters params(kMainMinimumLambda);

  // generate a random key
  // These 3 integers should be considered to have been selected psuedo-randomly
  std::array<uint32_t, 3> seed = {314, 1592, 657};
  TFHESecretKeySet key(params, seed);

  std::cout << "Starting initial database handshake" << std::endl;

  // char raw_demo_record[16+1] = {'T', 'i', 't', 'l', 'e', 0, 0, 0, 'C', 'o', 'n', 't', 'e', 'n', 't', 0, 0};
  // char raw_blank_record[16+1] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  char raw_demo_record[2+1] = {'T', 'C', 0};
  char raw_blank_record[2+1] = {0, 0, 0};


  // The order of these can be randomized so we don't know all blank ciphers are at the end.
  TfheString demo_record = TfheString::Encrypt(raw_demo_record, key);
  TfheString blank_record1 = TfheString::Encrypt(raw_blank_record, key);

  std::cout << "Initial database records have been created and encrypted.  Sending records and cloud key to the server..." << std::endl;
  
  std::vector<TfheString*> db;
  db.push_back(&demo_record);
  db.push_back(&blank_record1);

  std::cout << "Server has received and persisted the encrypted records and the cloud key." << std::endl;


  // records are seralized, sent to carol, and persisted.  all future accesses of records happen by carol
  std::cout << "Server handshake complete\n\n" << std::endl;

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
      char raw_result[READ_ALL_MAX*READ_ALL_COUNT+1];
      for(int i=0;i<(READ_ALL_MAX*READ_ALL_COUNT+1);i++) {
        raw_result[i] = 0;
      }

      auto counter = TfheArray<int>::Encrypt(raw_counter, key);
      auto result = TfheString::Encrypt(raw_result, key);

      std::cout << "\n-- READ_ALL operation, counter ciphertext, and result ciphertext, all sent to server" << std::endl;

      for(int i=0; i<db.size();i++) {
        XLS_CHECK_OK(readAll(*db[i], counter, result, key.cloud()));
      }

      std::cout << "-- Server returns altered result ciphertext\n" << std::endl;

      auto output = result.Decrypt(key);

      std::cout << "Response: \"";

      for(int i=0; i< READ_ALL_MAX*READ_ALL_COUNT;i++) {
        std::cout << output[i];
      }

      std::cout << "\"";
      std::cout << "\n" << std::endl;      
    } else if(op_type == 2) {
      char raw_result[SELECT_INDEX_MAX/2+1];
      for(int i=0;i<SELECT_INDEX_MAX/2+1;i++) {
        raw_result[i] = 0;
      }

      char input_chars[SELECT_INDEX_MAX/2];
      std::cout << "Please enter the query (max size " << SELECT_INDEX_MAX/2 << " chars): \n"  << std::endl;
      
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      std::string input;
      std::cin >> input;
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

      std::cout << "Entered \"" << input << "\"" << std::endl;

      for(int i=0; i<SELECT_INDEX_MAX/2; i++) {
        if(i < input.size() && input[i] != '\n') {
          input_chars[i] = input[i];
        } else {
          input_chars[i] = 0; 
        }      
      }
              
      auto query = TfheString::Encrypt(input_chars, key);
      auto result = TfheString::Encrypt(raw_result, key);

      std::cout << "\n-- SELECT operation, query ciphertext, and result ciphertext, all sent to server" << std::endl;

      for(int i=0; i<db.size();i++) {
        XLS_CHECK_OK(selectIndex(*db[i], query, result, key.cloud()));
      }

      std::cout << "-- Server returned altered result ciphertext\n" << std::endl;

      auto output = result.Decrypt(key);

      std::cout << "Response: \"";
      for(int i=0; i< SELECT_INDEX_MAX;i++) {
        std::cout << output[i];
      }
      std::cout << "\"";
      std::cout << "\n" << std::endl;
    } else if(op_type == 3) {
      int raw_result[1+1] = {0,0};
      char input_chars[INSERT_MAX];
      
      for(int i=0;i<INSERT_MAX;i++) {
        input_chars[i] = 0;
      }

      std::cout << "Please enter the title (max size "<< INSERT_MAX/2 <<" chars): \n"  << std::endl;
      
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      std::string input;
      std::cin >> input;
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      // std::cout << "*" << input << "*" << std::endl;

      for(int i=0; i<INSERT_MAX/2; i++) {
        if(i < input.size() && input[i] != '\n') {
          input_chars[i] = input[i];
        } else {
          input_chars[i] = 0; 
        }      
      }
      std::cout << "Entered \"" << input_chars << "*" << std::endl;

      std::cout << "\nPlease enter the content (max size "<< INSERT_MAX/2 <<" chars): \n"  << std::endl;
      
      std::string input2;
      // std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      std::cin >> input2;
      // std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

      for(int i=0; i<INSERT_MAX/2; i++) {
        if(i < input2.size() && input2[i] != '\n') {
          input_chars[i+INSERT_MAX/2] = input2[i];
        } else {
          input_chars[i+INSERT_MAX/2] = 0; 
        }      
      }

      std::cout << "Entered \"";
      for(int i=INSERT_MAX/2; i<INSERT_MAX; i++) {
        std::cout << input_chars[i];
      }
      std::cout << "\"";
      std::cout << std::endl;
      
      auto query = TfheString::Encrypt(input_chars, key);
      auto result = TfheArray<int>::Encrypt(raw_result, key);

      std::cout << "\n-- INSERT operation, query ciphertext, and result ciphertext, all sent to server" << std::endl;

      for(int i=0; i<db.size();i++) {
        XLS_CHECK_OK(insert(*db[i], query, result, key.cloud()));
      }

      std::cout << "-- Server returned altered result ciphertext\n" << std::endl;

      auto output = result.Decrypt(key);
      std::cout << "Response: ";
      std::cout << (output[0] ? "Success" : "Failure");
      std::cout << "\n" << std::endl;
    } else if(op_type == 4) {
      int raw_result[1+1] = {0, 0};

      char input_chars[COUNT_MAX];
      std::cout << "Please enter the query (max size "<< COUNT_MAX/2 <<" chars), use * for wildcard: \n"  << std::endl;
      
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      std::string input;
      std::cin >> input;
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

      std::cout << "Entered \"" << input << "\"" << std::endl;

      for(int i=0; i<COUNT_MAX; i++) {
        if(i < input.size() && input[i] != '\n') {
          input_chars[i] = input[i];
        } else {
          input_chars[i] = 0; 
        }      
      }
              
      auto query = TfheString::Encrypt(input_chars, key);
      auto result = TfheArray<int>::Encrypt(raw_result, key);

      std::cout << "\n-- COUNT operation, query ciphertext, and result ciphertext, all sent to server" << std::endl;

      for(int i=0; i<db.size();i++) {
        XLS_CHECK_OK(count(*db[i], query, result, key.cloud()));
      }

      std::cout << "-- Server returned altered result ciphertext\n" << std::endl;

      auto output = result.Decrypt(key);

      std::cout << "Response: ";
      std::cout << output[0];
      std::cout << "\n" << std::endl;
    }
  }

  return 0;
}
