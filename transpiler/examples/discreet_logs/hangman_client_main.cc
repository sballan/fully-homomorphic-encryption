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




void readAll(
  char record[16+1], 
  int counter[1+1],
  char result[32+1]
) {
  // Return if record is blank
  if(record[0] == '\0') {
    record[16+1]++;
    counter[1]++;
    result[32+1]++;
    return;
  }

  for(int i=0; i<16; i++) {
    result[counter[0]+i] = record[i];
  }

  counter[0]+=16;
  counter[1]++;
}



int main() {
  std::cout << "Starting..." << std::endl;


  std::cout << "Starting initial database handshake" << std::endl;

  char demo_record[16+1] = {'T', 'i', 't', 'l', 'e', 0, 0, 0, 'C', 'o', 'n', 't', 'e', 'n', 't', 0, 0};
  char blank_record[16+1] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};


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
      char raw_result[32+1];
      for(int i=0;i<(32+1);i++) {
        raw_result[i] = 0;
      }


        readAll(demo_record, raw_counter, raw_result);
        readAll(blank_record, raw_counter, raw_result);


      auto output = raw_result;
      for (int i=0; i<32; i++){
        std::cout << output[i];
      }
      std::cout << "\n" << std::endl;      
    } 
      }


  return 0;
}
